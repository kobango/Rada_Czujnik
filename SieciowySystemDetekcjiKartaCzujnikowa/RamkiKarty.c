#include "main.h"
#include "RamkiKarty.h"
#include "EEPROM.h"
#include "CAN.h"
#include "transmisja.h"
#include "WeWy.h"

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;

typedef union tuReg32 {
  UWord32 Val32;

  struct {
    UWord16 LW;
    UWord16 HW;
  } Word;

  char Val[4];
} uReg32;

mID ramkaCanRxKarty[RX_BUF_SIZE], ramkaCanTxKarty;


/*******************************************************************
Funkcja: static void RamkaPrzypisanAdresow(mID *message)
Opis funkcji: Przypisanie czujników do danego wewy
Data: 13.09.2013
Autor: Pawel Kasperek
*****************************************************************/
static void RamkaPrzypisanAdresow(mID *message, BYTE nrWeWyU8)
{
    BYTE i;

    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 8;
        for(i=0; i<(message->data_length/2); i++)
        {
            message->data[2*i] = (BYTE)(kartaCzujnikowa.weWy[nrWeWyU8].czujnik[i].adresCzujnikaU16>>8);
            message->data[(2*i)+1] = (BYTE)kartaCzujnikowa.weWy[nrWeWyU8].czujnik[i].adresCzujnikaU16;

        }
    }
    else
    {
        for(i=0; i<(message->data_length/2); i++)
        {
            kartaCzujnikowa.weWy[nrWeWyU8].czujnik[i].adresCzujnikaU16 = 
                    ((WORD)message->data[2*i]<<8) | (WORD)message->data[(2*i)+1];

            //odznacz ze jest przypisany
            if(kartaCzujnikowa.weWy[nrWeWyU8].czujnik[i].adresCzujnikaU16 > 0)
            {
                kartaCzujnikowa.weWy[nrWeWyU8].czujnik[i].Flagi.przypisanyCzujnik = 1;
            }
            else
            {
                kartaCzujnikowa.weWy[nrWeWyU8].czujnik[i].Flagi.przypisanyCzujnik = 0;
            }
        }
        nvmPutUstawieniaKarty(&kartaCzujnikowa);
    }
}
/*******************************************************************
Funkcja: static void RamkaStanBledowCzujniczkow(mID *message)
Opis funkcji: Zapis i odczyt nazw wlasnych czujniczkow
Data: 13.09.2013
Autor: Pawel Kasperek
*****************************************************************/
static void RamkaStanBledowCzujniczkow(mID *message, BYTE nrWeWy)
{
    BYTE i;

    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 8;
        for(i=0; i<message->data_length; i++)
        {
            if(i<4)
            {
                message->data[i] = kartaCzujnikowa.weWy[2*nrWeWy].czujnik[i].Flagi.nieGada;
            }
            else
            {
                message->data[i] = kartaCzujnikowa.weWy[2*nrWeWy+1].czujnik[i-4].Flagi.nieGada;
            }
        }
    }
}

/*******************************************************************
Funkcja: static void RamkaNazwaWlasnaCzujnika(mID *message)
Opis funkcji: Zapis i odczyt nazw wlasnych czujniczkow
Data: 13.09.2013
Autor: Pawel Kasperek
*****************************************************************/
static void RamkaNazwaWlasnaCzujnika(mID *message, BYTE nrCzujnika)
{

    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 8;
        nvmGetNazweCzujnika(&message->data[0], nrCzujnika);
    }
    else
    {
        nvmPutNazweCzujnika(&message->data[0], nrCzujnika);
    }
}

/*******************************************************************
Funkcja: static void LokalizacjaNaMapie(mID *message, BYTE nrRamki)
Opis funkcji: Zapis i odczyt lokalizacji przyciskow na mapie
Data: 04.08.2014
Autor: Pawel Kasperek
*****************************************************************/
static void LokalizacjaNaMapie(mID *message, BYTE nrRamki)
{
    CzujnikNaMapieStruct czujnik;

    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 8;
        
        nvmGetUstawieniaMapyCzujnika(&czujnik, (nrRamki*2));
        message->data[0] = czujnik.pozycjaXY.x.byte.HB;
        message->data[1] = czujnik.pozycjaXY.x.byte.LB;
        message->data[2] = czujnik.pozycjaXY.y.byte.HB;
        message->data[3] = czujnik.pozycjaXY.y.byte.LB;

        nvmGetUstawieniaMapyCzujnika(&czujnik, ((nrRamki*2)+1));
        message->data[4] = czujnik.pozycjaXY.x.byte.HB;
        message->data[5] = czujnik.pozycjaXY.x.byte.LB;
        message->data[6] = czujnik.pozycjaXY.y.byte.HB;
        message->data[7] = czujnik.pozycjaXY.y.byte.LB;
    }
    else
    {
        nvmGetUstawieniaMapyCzujnika(&czujnik, (nrRamki*2));
        czujnik.pozycjaXY.x.byte.HB = message->data[0];
        czujnik.pozycjaXY.x.byte.LB = message->data[1];
        czujnik.pozycjaXY.y.byte.HB = message->data[2];
        czujnik.pozycjaXY.y.byte.LB = message->data[3];
        nvmPutUstawieniaMapyCzujnika(&czujnik, (nrRamki*2));

        nvmGetUstawieniaMapyCzujnika(&czujnik, ((nrRamki*2)+1));
        czujnik.pozycjaXY.x.byte.HB = message->data[4];
        czujnik.pozycjaXY.x.byte.LB = message->data[5];
        czujnik.pozycjaXY.y.byte.HB = message->data[6];
        czujnik.pozycjaXY.y.byte.LB = message->data[7];
        nvmPutUstawieniaMapyCzujnika(&czujnik, ((nrRamki*2)+1));
    }
}

/*******************************************************************
Funkcja: static void LokalizacjaNaMapie(mID *message, BYTE nrRamki)
Opis funkcji: Zapis i odczyt lokalizacji przyciskow na mapie
Data: 04.08.2014
Autor: Pawel Kasperek
*****************************************************************/
static void SkalaNaMapie(mID *message, BYTE nrRamki)
{
    BYTE i;
    CzujnikNaMapieStruct czujnik;

    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 8;
        for(i=0; i<message->data_length; i++)
        {
            nvmGetUstawieniaMapyCzujnika(&czujnik, ((nrRamki*8)+i));
            message->data[i] = czujnik.skalaU8;
        }
    }
    else
    {
        for(i=0; i<message->data_length; i++)
        {
            nvmGetUstawieniaMapyCzujnika(&czujnik, ((nrRamki*8)+i));
            czujnik.skalaU8 = message->data[i];
            nvmPutUstawieniaMapyCzujnika(&czujnik, ((nrRamki*8)+i));
        }
    }
}


/*******************************************************************
Funkcja: static void RamkaStanuWeWy(mID *message)
Opis funkcji: 0x08 aktualny stan karty
Data: 05.11.2013
Autor: Pawel Kasperek
*****************************************************************/
static void RamkaStanuWeWy(mID *message)
{
    BYTE i;

    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 5;
        message->data[0] = kartaCzujnikowa.Flagi.error;
        message->data[1] = 0;
        message->data[2] = 0;
        message->data[3] = 0;
        message->data[4] = 0;
        for(i=0; i<8; i++)
        {
            if(kartaCzujnikowa.weWy[i].Flagi.stan)
            {
                message->data[1] |= 1<<i;
            }
            if(kartaCzujnikowa.weWy[i].Flagi.reset)
            {
                message->data[2] |= 1<<i;
            }
            if(kartaCzujnikowa.weWy[i].Flagi.uczenieTla)
            {
                message->data[3] |= 1<<i;
            }
            if(kartaCzujnikowa.weWy[i].Flagi.error)
            {
                message->data[4] |= 1<<i;
            }
        }
    }
    else
    {
        if((message->data[0]&0x04) == 0x04)
        {
            Reset();
        }
        for(i=0; i<8; i++)
        {
            //reset grupy
            if((message->data[2] & (1<<i)) > 0)
            {
                DaneCan.Flagi.wyslijRamkeStanu = 1;
                kartaCzujnikowa.weWy[i].Flagi.reset = 1;
            }
            //uczenie tla grupy
            if((message->data[3] & (1<<i)) > 0)
            {
                DaneCan.Flagi.wyslijRamkeStanu = 1;
                DaneCan.Flagi.wyslijRamkeUczeniaTla = 1;
                kartaCzujnikowa.weWy[i].timerUczeniaTlaU8 = TIMER_WYKONANIA_UCZENIA_TLA;
                kartaCzujnikowa.weWy[i].Flagi.uczenieTla = 1;
            }
        }
    }
}

/*******************************************************************
Funkcja: static void RamkaWersjiOprogramowania(mID *message)
Opis funkcji: Informacja o wersji oprogramowania i hardwareu
Data: 11.07.2013
Autor: Pawel Kasperek
*****************************************************************/
static void RamkaWersjiOprogramowania(mID *message)
{
    WORD_VAL wersjaOprogramowaniaU16;
    uReg32 wersjaSprzetuAdres, wersjaSprzetu;

    wersjaOprogramowaniaU16.Val = wersjaOprogramowania;
    wersjaSprzetuAdres.Val32 = 0x0402;
    ReadFlash(wersjaSprzetuAdres.Val32,2,wersjaSprzetu.Val);

    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 4;
        message->data[0] = wersjaOprogramowaniaU16.v[1];
        message->data[1] = wersjaOprogramowaniaU16.v[0];
        message->data[2] = wersjaSprzetu.Val[1];
        message->data[3] = wersjaSprzetu.Val[0];
    }
}

/*******************************************************************
Funkcja: void ObsluzRamkeCan(void)
Opis funkcji: obsluga rozpakowanej ramki CAN
Data: 20.08.2012
Autor: Mariusz Chrobak
*****************************************************************/
void ObsluzRamkeCan(mID *message)
{
    BYTE id_ramki;

    id_ramki = (BYTE)(message->id.w[1]/4);

    if(id_ramki < 0x09)
    {
        RamkaPrzypisanAdresow(message, id_ramki-1);
    }
    else if((id_ramki>0x09) && (id_ramki<0x0E))
    {
        RamkaStanBledowCzujniczkow(message, id_ramki-0x0A);
    }
    else if((id_ramki>0x10) && (id_ramki<0x31))
    {
        RamkaNazwaWlasnaCzujnika(message, id_ramki-0x11);
    }
    else if((id_ramki>0x30) && (id_ramki<0x42))
    {
        LokalizacjaNaMapie(message, id_ramki-0x31);
    }
    else
    {
	    switch(id_ramki)
	    {
	        case 0x09:
	            RamkaStanuWeWy(message);
	            break;
            case 0x42:
            case 0x43:
            case 0x44:
            case 0x45:
                SkalaNaMapie(message,id_ramki-0x42);
                break;
	        case 0x83:
	            RamkaWersjiOprogramowania(message);
	            break;
                default:
                    message->message_type = CAN_MSG_DATA;
                    break;
	    }
    }
    if(message->message_type == CAN_MSG_RTR)
    {
        GenerujCAN_ID(message,id_ramki);
        WyslijRamkeCan(message);
    }
}

/*******************************************************************
Funkcja: void WyslijRamkeUczeniaTlaMiWi(mID* canMessage, WORD adresCzujnikaMiWi)
Opis funkcji: wysylanie do czujnikow bezprzewodowych ramki uczenia tla (czujniki
 * nie obsluguja broadcastow)
Data: 13.02.2014
Autor: Pawel Kasperek
*****************************************************************/
void WyslijRamkeUczeniaTlaMiWi(mID* message, WORD adresCzujnikaMiWi)
{
    message->frame_type = CAN_FRAME_EXT;
    message->message_type = CAN_MSG_DATA;
    message->id.v[2] = 0x07 * 4;
    message->id.w[0] = adresCzujnikaMiWi;
    message->data_length = 4;
    message->data[0] = 0;
    message->data[1] = 1;
    message->data[2] = 1;
    message->data[3] = 0;
    WyslijRamkeCan(message);
}
