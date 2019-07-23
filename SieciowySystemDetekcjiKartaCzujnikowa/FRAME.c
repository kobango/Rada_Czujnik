#include "CAN.h"
#include "main.h"
#include "LED.h"
#include "FRAME.h"
#include "MOC_Funct.h"
#include "TRM.h"

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
BYTE IsInNeighbors(UINT message_adress);

volatile UINT NeightAdress[MAX_SASIADOW] = {0, 0, 0, 0, 0, 0, 0, 0};

/*******************************************************************
Funkcja: void StatusWzbudzeniaCzujnika(mID *message);
Opis funkcji: Czujnik cyklicznie wysyla swoj stan
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_SensorExcitationStatus(mID *message) // id = 0x01
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 6;
        //message->data[0] = Flagi.zgloszenie;
        //message->data[1] = (unsigned char)(Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16 >> 8);
        //message->data[2] = (unsigned char)Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16;
        //message->data[3] = Flagi.frameCounterU8++;
        //message->data[4] = (unsigned char)Thermistor->temperaturaS8;
        message->data[0] = LOCK_Get();
        message->data[1] = MOC_Wynikowa_wartosc_roznicowa() >> 8 ;
        message->data[2] = MOC_Wynikowa_wartosc_roznicowa() & 0x00FF;
        message->data[3] = MOC_Frame_Counter();
        message->data[4] = MOC_Aktualna_Temperatura();
        message->data[5] = MOC_NOTWORK();
        //message->data[6] = MOC_RSSI_ramki();
        //message->data[7] = MOC_LQI_ramki();
        //message->data[0] = 0x00;// MOC_StanWzbudzenia();
        //message->data[1] = 0x88;//MOC_Wynikowa_wartosc_roznicowa() & 0x00FF;
        //message->data[2] = 0x88;//MOC_Wynikowa_wartosc_roznicowa()>>8;
        //message->data[3] = 0x00;//MOC_Frame_Counter();
        //message->data[4] = 0x00;
        //wyczysc maksymalna wartosc odczytu
        //Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16 = 0;
    }
    else
    {
        
        WORD iterator_beta;
        int Saturn  = NeightAdress[0];
        
        if(Saturn == 12)
        {
            int Satrun_prime = Saturn;
            
            for(iterator_beta=0;iterator_beta<400;iterator_beta++)
            {
            LED_Clear();
            }
            int terra =Satrun_prime;
            
        }
        else
        {
            int Satrun_prime = Saturn;
            for(iterator_beta=0;iterator_beta<400;iterator_beta++)
            {
            LED_Error();
            }       
            int terra =Satrun_prime;
        }    
        
        if(0x12b==message->id.w[0])
        {
        LOCK_Set(message->data[0]);
        }
    }
}

BYTE IsInNeighbors(UINT message_adress)
{
    WORD i;
    //0x10
    for(i=0; i<8; i++)
        {
            if(message_adress == NeightAdress[i] )
            {
                return 1;
                
            }
            //zapisz adres struktury
            
        } 
    return 0;
}

/*******************************************************************
Funkcja: void StatusPrzyspieszeniomierza(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_AccelerometerStatus(mID *message) // id = 0x02
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 4;
        //message->data[0] = Accelerometer->przyspieszenieXU8;
        //message->data[1] = Accelerometer->przyspieszenieYU8;
        //message->data[2] = Accelerometer->przyspieszenieZU8;
        message->data[0] = MOCK_PrzyspieszenieX();
        message->data[1] = MOCK_PrzyspieszenieY();
        message->data[2] = MOCK_PrzyspieszenieZ();
        message->data[3] = MOCK_Klikniecie_Spadek();
    }
    else
    {

    }
}

/*******************************************************************
Funkcja: void WartosciWzbudzenia(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_ExcitationValue(mID *message) // id = 0x03
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        //message->data[0] = (unsigned char)(Sensor->roznicaZgloszeniaMinU16 >> 8);
        //message->data[1] = (unsigned char)Sensor->roznicaZgloszeniaMinU16;
        //message->data[2] = (unsigned char)(Sensor->roznicaZgloszeniaMaxU16 >> 8);
        //message->data[3] = (unsigned char)Sensor->roznicaZgloszeniaMaxU16;
        message->data[0] = 0xFF;
    }
    else
    {
        //Sensor->roznicaZgloszeniaMinU16 = (unsigned int)message->data[0]<<8 | (unsigned int)message->data[1];
        //Sensor->roznicaZgloszeniaMaxU16 = (unsigned int)message->data[2]<<8 | (unsigned int)message->data[3];
    }
}

/*******************************************************************
Funkcja: void MnoznikiWzbudzenia(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_ExcitationMultiplier(mID *message) // id = 0x04
{
    WORD i;
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        for(i=0; i<3; i++)
        {
            //message->data[2*i] = (unsigned char)(Sensor->OsXYZ[i].mnoznikU16 >> 8);
            //message->data[(2*i)+1] = (unsigned char)Sensor->OsXYZ[i].mnoznikU16;
        }
        message->data[0] = 0xFF;
    }
    else
    {
        //Sensor->OsXYZ[0].mnoznikU16 = (unsigned int)message->data[0]<<8 | (unsigned int)message->data[1];
        //Sensor->OsXYZ[1].mnoznikU16 = (unsigned int)message->data[2]<<8 | (unsigned int)message->data[3];
        //Sensor->OsXYZ[2].mnoznikU16 = (unsigned int)message->data[4]<<8 | (unsigned int)message->data[5];
    }
}

/*******************************************************************
Funkcja: void CzasyUsredniania(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_AveragingTimes(mID *message) // id = 0x05
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        //message->data[0] = (unsigned char)(Sensor->czasUsrednianiaTlaU16 >> 8);
        //message->data[1] = (unsigned char)Sensor->czasUsrednianiaTlaU16;
        //message->data[2] = (unsigned char) (Sensor->czasZgloszeniaU16);
        //message->data[3] = (unsigned char) (Sensor->czujnikZliczajacy);
        //message->data[4] = (unsigned char) (Sensor->czasWyjsciaZeWzbudzeniaU16>>8);
        //message->data[5] = (unsigned char) (Sensor->czasWyjsciaZeWzbudzeniaU16);
        //message->data[6] = (unsigned char) (Sensor->czasStabilizacjiSasiadaU16);
        message->data[0] = 0xFF;
    }
    else
    {
        /*
        Sensor->czasUsrednianiaTlaU16 = (unsigned int)message->data[0]<<8 | (unsigned int)message->data[1];
        Sensor->czasZgloszeniaU16 = (unsigned int)message->data[2];
        if((message->data[3] & 0x01) == 1)
        {
            Sensor->czujnikZliczajacy = 1;
        }
        else
        {
            Sensor->czujnikZliczajacy = 0;
        }
        if(message->data_length > 4)
        {
            Sensor->czasWyjsciaZeWzbudzeniaU16 = (unsigned int)message->data[4]<<8 | (unsigned int)message->data[5];
        }
        else
        {
            Sensor->czasWyjsciaZeWzbudzeniaU16 = 0;
        }
        if(message->data_length > 6)
        {
            if(message->data[6] < HISTORIA_SASIADA_SIZE)
            {
                Sensor->czasStabilizacjiSasiadaU16 = (unsigned int)message->data[6];
            }
            else
            {
                Sensor->czasStabilizacjiSasiadaU16 = HISTORIA_SASIADA_SIZE;
            }
        }
        else 
        {
            Sensor->czasWyjsciaZeWzbudzeniaU16 = 0;
        }*/
    }
}

/*******************************************************************
Funkcja: void StatusWzbudzeniaOsi(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_AxisStatus(mID *message) //0x06
{
    WORD i;
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        /*
        message->data[0] = Flagi.zgloszenie;
        for(i=0; i<3; i++)
        {
            message->data[(2*i)+1] = (unsigned char)(Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16 >> 8);
            message->data[(2*i)+2] = (unsigned char)Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16;
            //wyczysc maksymalne stany odczytow
            Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16 = 0;
        }
         */
        message->data[0] = 0xFF;
    }
    else
    {

    }
}

/*******************************************************************
Funkcja: void ZaktualizujDateZmianyUstawien(mID *message)
Opis funkcji:
Data: 05.01.2017
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_DataUpdateAndChangeOption(mID *message)
{
    /*
    Dane->rokU16 = message->data[4];
    Dane->miesiacU16 = message->data[5];
    Dane->dzienU16 = message->data[6];
    Dane->godzinaU16 = message->data[7];
     */
}

/*******************************************************************
Funkcja: void ResetUrzadzenia(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_DeviceReset(mID *message) //0x07
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        
        message->data[0] = (RCON>>6) & 0x01; //stan software reset
        message->data[1] = Flagi.wykonanoZapisDoFlash;
        message->data[2] = Flagi.pomiarTla;
        message->data[3] = 0;
        message->data[4] = Dane->rokU16;
        message->data[5] = Dane->miesiacU16;
        message->data[6] = Dane->dzienU16;
        message->data[7] = Dane->godzinaU16;
        RCON &= ~(1<<6); //kasowanie software reset po odczycie ramki
        Flagi.wykonanoZapisDoFlash = 0;
        
        
        //message->data[0] = MOCK_SoftwareReset();//stan software reset
        //message->data[1] = Flags.wykonanoZapisDoFlash;
        //message->data[2] = MOCK_Background();
        //message->data[3] = 0;
        //message->data[4] = DaneCan->rokU16;
        //message->data[5] = DaneCan->miesiacU16;
        //message->data[6] = DaneCan->dzienU16;
        //message->data[7] = DaneCan->godzinaU16; 
        //RCON &= ~(1<<6); //kasowanie software reset po odczycie ramki
        //Flags.wykonanoZapisDoFlash = 0;
        
    }
    else
    {
        message->data_length = 1;
        /*
        if(message->data[2])
        {
            Flagi.pomiarTla = 1;
            if(message->data[1])
            {
                Flagi.zapisDoFlash = 1;
            }
        }
        else
        {
            if(message->data[1])
            {
                ZaktualizujDateZmianyUstawien(message);
                ZapisZmiennychDoFLASH();
            }
            if(message->data[0])
            {
                Reset();
            }
        }*/
        message->data[0] = 0xFF;
    }
}

/*******************************************************************
Funkcja: void RysowanieWykresu(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_Plot(mID *message) //id = 0x09
{
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data[0] = (BYTE)(Dane->timerRysowaniaWykresuU16/10);
    }
    else
    {
        Dane->timerRysowaniaWykresuU16 = (WORD)message->data[0]*10;
    }
     
    
}

/*******************************************************************
Funkcja: void PozycjaCzujnikaNaMapie(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_MapPosition(mID *message) //id = 0x0A
{ /*
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 4;
        message->data[0] = (unsigned char)(Dane->PozycjaNaMapce.xU16 >> 8);
        message->data[1] = (unsigned char)Dane->PozycjaNaMapce.xU16;
        message->data[2] = (unsigned char)(Dane->PozycjaNaMapce.yU16 >> 8);
        message->data[3] = (unsigned char)Dane->PozycjaNaMapce.yU16;
    }
    else
    {
        Dane->PozycjaNaMapce.xU16 = (unsigned int)message->data[0] << 8
                | (unsigned int)message->data[1];
        Dane->PozycjaNaMapce.yU16 = (unsigned int)message->data[2] << 8
                | (unsigned int)message->data[3];
    } */
}

/*******************************************************************
Funkcja: void WersjaOprogramowania(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_SoftwareVersion(mID *message) //id = 0x0B
{
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 4;
        message->data[0] = (unsigned char)(Dane->wersjaOprogramowaniaU16 >> 8);
        message->data[1] = (unsigned char)Dane->wersjaOprogramowaniaU16;
        message->data[2] = (unsigned char)(Dane->wersjaSprzetuU16 >> 8);
        message->data[3] = (unsigned char)Dane->wersjaSprzetuU16;
    }
    else
    {

    } 
}

/*******************************************************************
Funkcja: void WartosciAnalogowe(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_AnalogValue(mID *message, WORD set) //id = 0x0B
{
    WORD i;
    /*
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 6;
        for(i=0; i<3; i++)
        {
            message->data[2*i] = (unsigned char)(Sensor->OsXYZ[i].analogowySetResetU16[set] >> 8);
            message->data[(2*i)+1] = (unsigned char)Sensor->OsXYZ[i].analogowySetResetU16[set];
        }
    }
    else
    {

    }
    */
}

/*******************************************************************
Funkcja: void PrzypisanieDokarty(mID *message);
Opis funkcji:
Data: 14.03.2014
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_PrzypisanieDokarty(mID *message) // 0x0F
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        message->data[0] = 0xFF;
    }
    else
    {
           // NONE
    }
}
/*******************************************************************
Funkcja: void AdresySasiadow(mID *message, WORD nrRamki);
Opis funkcji:
Data: 17.11.2016
Autor: Pawel Kasperek
*****************************************************************/
static void FRAME_AdressOfNeighbors(mID *message, WORD nrRamki)  //0x10
{
    WORD iterator_alfa;
    
    
    WORD kier = (nrRamki-0x10);
    if(message->message_type == CAN_MSG_RTR)
    {
        
        message->data_length = 8;
        for(iterator_alfa=0; iterator_alfa<4; iterator_alfa++)
        {
            message->data[(2*iterator_alfa)] = ((BYTE)NeightAdress[(iterator_alfa+(4*kier))] >> 8);
            message->data[((2*iterator_alfa)+1)] = (BYTE)NeightAdress[(iterator_alfa+(4*kier))];
        }
        
    }
    else
    {
        WORD uranos = (message->data[2] << 8 ) | message->data[3];
        WORD zeta =(WORD) uranos; 
         
        WORD kier = (nrRamki-0x10);
         
         WORD gaja = message->data[0] << 8 | message->data[1];
         WORD zeta_secodus =(WORD) gaja; 
         
         /*
        for(i=0; i<4; i++)
        {
            NeightAdress[i+(4*kier)] =  ((WORD)message->data[2*i]) |
                    (WORD)message->data[(2*i)+1];
            //zapisz adres struktury
            
        } 
          * */
         
         for(iterator_alfa=0; iterator_alfa<4;iterator_alfa++)
         {
             NeightAdress[(iterator_alfa+(4*kier))] =  (message->data[2*iterator_alfa] << 8)| message->data[((2*iterator_alfa)+1)];
         }
         
         
                 
    } 
}

/*******************************************************************
Funkcja: void ObsluzRamkeCan(mID * message, unsigned int identyfikator)
Opis funkcji: funkcja przygotowujaca dane do przeslania
Data: 15.04.2011
*****************************************************************/
void FRAME_HandleCanFrame(mID * message)
{
    BYTE identyfikator = (BYTE) message->id.v[2]/4;
   
    
    if(identyfikator != 0x01)
    {
        WORD ident = identyfikator;
        int alfa = ident;
    }
    
    switch(identyfikator)
    {
        case 0x01:
            FRAME_SensorExcitationStatus(message);
            break;
        case 0x02:
            FRAME_AccelerometerStatus(message);
            break;
        case 0x03:
            FRAME_ExcitationValue(message);
            break;
        case 0x04:
            FRAME_ExcitationMultiplier(message);
            break;
        case 0x05:
            FRAME_AveragingTimes(message);
            break;
        case 0x06:
            FRAME_AxisStatus(message);
            break;
        case 0x07:
            FRAME_DeviceReset(message);
            break;
        case 0x09:
            FRAME_Plot(message);
            break;
        case 0x0A:
            FRAME_MapPosition(message);
            break;
        case 0x0B:
            FRAME_SoftwareVersion(message);
            break;
        case 0x0C:
        case 0x0D:
            FRAME_AnalogValue(message, identyfikator - 0x0C);
            break;
        case 0x0F:
            FRAME_PrzypisanieDokarty(message);
            break;
        case 0x10:
            FRAME_AdressOfNeighbors(message, identyfikator - 0x10);
            break;
        case 0x11:
            FRAME_AdressOfNeighbors(message, identyfikator - 0x11);
            break;
         
            /*
        default:
          FRAME_AdressOfNeighbors(message, identyfikator - 0x10);

            break;
             * */
    }
    if(message->message_type == CAN_MSG_RTR)
    {
        
        message->frame_type = CAN_FRAME_EXT;
        message->message_type = CAN_MSG_DATA;
        message->id.Val = 0;
        
        message->id.v[2] = identyfikator*4;
        CAN_GenID(message,identyfikator);
        CAN_SendFrame(message);
        //poczekaj z wysylaniem ramki az sie skonczy probkowanie
        /*
        while(SENSOR == 0)
        {
            ;
        }
         * */
//        TXB0CONbits.TXREQ = 1;  
        
        
       while(RXB0CONbits.FILHIT3) // Poczekaj a? wysle wiadomo?c
       {
           if(TXB0CONbits.TXERR == 1){ // Je?li b??d wyswietl 
               LED_Error();
           }
           else
           {
               //LED_Clear();
           }    
           
       };
        
        //bits.TXREQ0 = 1;
         
    }
}
