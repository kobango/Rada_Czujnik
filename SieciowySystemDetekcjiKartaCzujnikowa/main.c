#include "main.h"
#include "TRM.h"
//#include "EEPROM.h"
#include "ISR.h"
#include "TMR1.h"
#include "LED.h"
#include "INI.h"
#include "FLASH.h"


// PIC18F26K80 Configuration Bit Settings

// CONFIG1L
// PIC18F4680 Configuration Bit Settings

// 'C' source line config statements
//#pragma config IDLOC0 =   100u;

// CONFIG1H
#pragma config OSC = IRCIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

/*
#pragma romdata cb=Const_ADR
    rom WORD czas_bootldr = 0x05;
    rom WORD wersjaOprogramowania = 0x012E;
#pragma romdata
*/
KartaStruct DetectorLedRadar;

struct DaneStruct *Dane ;
struct FlagStruct Flagi;

 //extern const unsigned char userID[8] @ 0x200000;



/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 11/07/2019
*
* @section Description
* Program main loop
*****************************************************************************************/



void main(void)
{
   
    int adr = FLASH_Read(0x200000);
     adr |= FLASH_Read(0x200001)<<8;
     
     
     
     FLASH_Write(0x200000,0x76);
     FLASH_Write(0x200001,0x00);
    
     adr = FLASH_Read(0x200000);
     adr |= FLASH_Read(0x200001)<<8;
     
    DaneCan.adresCAN  = adr;
    INI_All();
    //po inicjalizacji kasuj RCON
    RCON = 0xFF;
    //petla glowna
    for(;;)
    {
        if(DetectorLedRadar.Flags.obsluzWeWy == 1)
        {
            DetectorLedRadar.Flags.obsluzWeWy = 0;
//            ObsluzWeWy();
        }
        
        if(TMR1_Update_flag_Get()==1)
        {
            LED_Update(); 
            TMR1_Update_flag_Set(0);
            
        }
        
        
        TRM_DataTransmition();
        ClrWdt();
    }
}

