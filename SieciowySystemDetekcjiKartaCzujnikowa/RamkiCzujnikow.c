#include "main.h"
#include "CAN.h"


/*******************************************************************
Funkcja: static void RamkaStanuCzujnika(mID *message)
Opis funkcji: Ramka obrabiajaca dane z czujnika na stan wewy
Data: 11.07.2013
Autor: Pawel Kasperek
*****************************************************************/
static void RamkaStanuCzujnika(mID *message)
{
    BYTE stanU8, frameCounterU8;
    WORD adresU16;

    if(message->message_type == CAN_MSG_DATA)
    {
        adresU16 = message->id.w[0];
        //jaki adres ramki i jaki stan czujnika
        stanU8 = message->data[0];
        //jaki aktualny licznik czujnika - czy na pewno wszystkie ramki doszly
        frameCounterU8 = message->data[3];
        //przekaz do funkcji WeWy
        AktualizujDaneCzujnika(adresU16, stanU8, frameCounterU8);
    }
}

/*******************************************************************
Funkcja: void ObsluzRamkeCan(void)
Opis funkcji: obsluga rozpakowanej ramki CAN
Data: 20.08.2012
Autor: Mariusz Chrobak
*****************************************************************/
void ObsluzRamkeCzujnika(mID *message)
{
    unsigned char id_ramki;

    id_ramki = message->id.v[2]/4;
    switch(id_ramki)
    {
        case 0x01:
            RamkaStanuCzujnika(message);
            break;
        default:
            Nop();
            break;
    }
}


