#include "main.h"
#include "ISR.h"
#include "TRM.h"
#include "TMR1.h"
#include "LED.h"


/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 10/07/2019
*
* @section Description
* Low priority interrupt. This function should set flags to main loop
*****************************************************************************************/
void __interrupt(low_priority) ISR_low (void)
{ 
/**************************************
/         Timer1 - 100ms               /
**************************************/
    if(TMR1IF==1)
        {
            TMR1_Timer_reset();
    
            
            static BYTE ramkaStanuU8=0;
            
            // flag LED
            TMR1_Update_flag_Set(1);
            
            DetectorLedRadar.Flags.obsluzWeWy = 1;
        
            if(++ramkaStanuU8 > TIMER_RAMKI_STANU)
                {
                    ramkaStanuU8 = 0;
                    DaneCan.Flags.wyslijRamkeStanu = 1;
                }
            
            // Timer reset
            // interrupt flag reset
            TMR1IF=0; 
        } 
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 10/07/2019
*
* @section Description
* Low priority interrupt. This function should set flags to main loop
*****************************************************************************************/
void __interrupt(high_priority) ISR_high (void)
{ 
        // EMPTY
    
}