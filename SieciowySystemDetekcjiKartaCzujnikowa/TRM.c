#include "main.h"
#include "FRAME.h"
#include "TRM.h"
#include "CAN.h"


DaneCanStruct DaneCan;
// Ustawi? DaneCan.adresCAN  = Adres // 0x4001


/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/11/2013 
*
* @section Description
* Function that checks events related to CAN
*****************************************************************************************/
void TRM_DataTransmition(void)
{
    static BYTE weWyU8 = 0, czujnikWeWyU8 = 0;
    BYTE i;
    mID canMessage;

    // wyslij Stan wzbudzenia przycisku przy kazdej zmianie i co 5 sek
    if(DaneCan.Flags.wyslijRamkeStanu)
    {
        //wyslij ramke stanu
        canMessage.message_type = CAN_MSG_RTR;
        canMessage.id.Val = 0; //
        canMessage.id.v[2] = 0x01   *4; 
        FRAME_HandleCanFrame(&canMessage);
/*        for(i=0; i<WEWY; i++)
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
 * */
        DaneCan.Flags.wyslijRamkeStanu = 0;
    }

    //wyslij uczenie tla do bezprzewodowych czujnikow
    if(DaneCan.Flags.wyslijRamkeUczeniaTla)
    {
           // EMPTY
    }

    //sprawdz czy jest cos na CANie
    if(CAN_TakeFrame(&canMessage))
    {
        
        
        if((canMessage.buffer == 0) || (canMessage.buffer == 1))
        {
            
            FRAME_HandleCanFrame(&canMessage);
        }
        else
        {
            FRAME_HandleCanFrame(&canMessage);
        }
    }
}