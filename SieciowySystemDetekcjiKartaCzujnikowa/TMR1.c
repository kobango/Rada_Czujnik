/*
 * File:   Timer1.c
 * Author: hp
 *
 * Created on July 8, 2019, 4:35 PM
 */
#define TIMER_WECT (0xFFFF - (((8000000/8)/4)/10))  /**< Wector od Timer interrupt register */

#include <xc.h>
#include "TMR1.h"
#include "LED.h"
#include "GenericTypeDefs.h"


UINT8 INI_Timer(void);
void TMR1_Update_flag_Set(UINT a);
UINT TMR1_Update_flag_Get(void);

static UINT TMR1_Update_flag = 0; /**< Global flag for Timer1 (1 or 0) */

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @param a  Global flag for LEDs (1 or 0)
*
*
* @section Description
* The flag function is set when you need to change the position of the leds
*****************************************************************************************/
void TMR1_Update_flag_Set(UINT a)
{
    TMR1_Update_flag = a;
}
/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @return  Global flag for LEDs (1 or 0)
*
*
* @section Description
* The flag function, if on 1 change the position of the leds
*****************************************************************************************/
UINT TMR1_Update_flag_Get(void)
{
    return TMR1_Update_flag;
}


/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @return  Returns 1 if successful
* 
* @section Description
* The function resets the value of the timer registers.
*****************************************************************************************/
UINT8 TMR1_Timer_reset(void)
{
    //TMR1H= (UINT8)(TIMER_WECT >> 8);     // Load the time value(0xBDC) for 100ms delay
    //TMR1L=(UINT8)(TIMER_WECT & 0xFF);
    
   
    volatile BYTE t1L = 0, t1H = 0;
    t1L = TMR1L;
    t1H = TMR1H;
    Nop();
    t1H += (UINT8)(TIMER_WECT >> 8);
    t1L += (UINT8)(TIMER_WECT & 0xFF);
    TMR1H = t1H;
    TMR1L = 0x5B;
    
    
    return 1;
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @return  Returns 1 if successful
* 
* @section Description
* Timer setting function
*****************************************************************************************/
UINT8 INI_Timer(void)
{
    
    
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF0 = 1;
    T1CON = 0b00111101;
    TMR1H= (UINT8)(TIMER_WECT >> 8);     
    TMR1L=(UINT8)(TIMER_WECT & 0xFF);    //TMR1H and TMR1L load
    TMR1IE=1;       //Enable timer interrupt bit in PIE1 register      
    TMR1ON = 1; 
    return 1;
}
