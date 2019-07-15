#include "main.h"
#include "RamkiKarty.h"
#include "RamkiCzujnikow.h"
#include "transmisja.h"
#include "CAN.h"


DaneCanStruct DaneCan;
/*******************************************************************
Funkcja: void TransmisjaDanych(void)
Opis funkcji: funkcja sprawdzajaca zdarzenia dotyczace CANa
Data: 14.11.2013
Modyfikacje: Pawel kasperek
*****************************************************************/
void TransmisjaDanych(void)
{
    static BYTE weWyU8 = 0, czujnikWeWyU8 = 0;
    BYTE i;
    mID canMessage;

    // wyslij Stan wzbudzenia przycisku przy kazdej zmianie i co 2 sek
    if(DaneCan.Flagi.wyslijRamkeStanu)
    {
        //wyslij ramke stanu
        canMessage.message_type = CAN_MSG_RTR;
        canMessage.id.Val = 0;
        canMessage.id.v[2] = 0x09*4;
        ObsluzRamkeCan(&canMessage);
        for(i=0; i<WEWY; i++)
        {
            if(kartaCzujnikowa.weWy[i].Flagi.error)
            {
                canMessage.message_type = CAN_MSG_RTR;
                canMessage.id.Val = 0;
                canMessage.id.w[1] = (WORD)((i/2)+0x0A)*(WORD)4;
                ObsluzRamkeCan(&canMessage);
                //zabezpieczenie przed podwojnym wysylaniem tej samej ramki
                if((i&1)==0)
                {
                    i++;
                }
            }
        }
        DaneCan.Flagi.wyslijRamkeStanu = 0;
    }

    //wyslij uczenie tla do bezprzewodowych czujnikow
    if(DaneCan.Flagi.wyslijRamkeUczeniaTla)
    {
        if(weWyU8<WEWY)
        {
            if(kartaCzujnikowa.weWy[weWyU8].Flagi.uczenieTla && (czujnikWeWyU8<DETEKTOROW_NA_WEWY))
            {
                WyslijRamkeUczeniaTlaMiWi(&canMessage,
                    kartaCzujnikowa.weWy[weWyU8].czujnik[czujnikWeWyU8].adresCzujnikaU16);
                czujnikWeWyU8++;

            }
            else
            {
                czujnikWeWyU8 = 0;
                weWyU8++;
            }
        }
        else
        {
            weWyU8 = 0;
            czujnikWeWyU8 = 0;
            DaneCan.Flagi.wyslijRamkeUczeniaTla = 0;
        }
    }

    //sprawdz czy jest cos na CANie
    if(OdbierzRamkeCan(&canMessage))
    {
        if((canMessage.buffer == 0) || (canMessage.buffer == 1))
        {
            ObsluzRamkeCan(&canMessage);
        }
        else
        {
            ObsluzRamkeCzujnika(&canMessage);
        }
    }
}