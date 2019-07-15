/*
 * File:   INI.c
 * Author: hp
 *
 * Created on 10 lipca 2019, 11:26
 */


#include <xc.h>
#include "main.h"
#include "CAN.h"
#include "LED.h"
#include "TMR1.h"

void INI_GlobalInterrupt(void);
static void INI_OUTPUT(void);
void INI_All(void);

/***************************************************************************************/
/**
* @author Mariusz Chrobak
* @date 07/10/2008
*
* @section Description
* Function that initializes the periphery
*****************************************************************************************/
static void INI_OUTPUT(void)
{
    //ustawienie stanow i kierunkow portow
    
    LATA = 0;
    LATB = 0;
    LATC = 0;
    TRISA = 0x0C;
    TRISB = 0b10001000; //0x80;
    TRISC = 0xBF;
    RCONbits.IPEN = 1; // !! wazne
    //TMR0 uzywany do MiWi
    
    DetectorLedRadar.Flags.inicjalizacja = 1;
    
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 07/10/2008
*
* @section Description
* Function that initiates the program, collective function
*****************************************************************************************/
void INI_All(void)
{

    WDTCONbits.SWDTEN = 1;

    //odczytaj zane z wewnetrznego EEPROM
//    NVMInit();
//    UstawFlagi();

    //inicjalizacja zmiennych i peryferii
    
    ClrWdt();
    INI_OUTPUT();

    CAN_Setup();
    INI_GlobalInterrupt();
    //Start Timer1       
    INI_Timer();
        // LED Init
    INI_LED_Start();

}
void INI_GlobalInterrupt(void)
{
INTCONbits.GIEH=1;                          //Enabling High priority interrupts
INTCONbits.GIEL=1;                          //Enabling Low priority interrupts
RCONbits.IPEN=1; 
IPEN =1;
         //Enable Global Interrupt
PEIE=1; 
TMR1IP = 0; // Timer1 low priority interrupt
}
