
#include "main.h"
#define FLASH_V1_4
#include <xc.h>
#include <plib.h>

#define PROGRAM_START (DWORD)0xC00      //start of application program, do not write below this address

#define VERSION_MAJOR 0x01       //this cannot be zero!
#define VERSION_MINOR 0x00

#define LED LATAbits.LATA5

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




// PIC18LF26K80 Configuration Bit Settings

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG    // SOSC Power Selection and mode Configuration bits (Digital (SCLKI) mode)
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config FOSC = HS1       // Oscillator (HS oscillator (Medium power, 4 MHz - 16 MHz))
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = ON      // Power Up Timer (Enabled)
#pragma config BOREN = ON       // Brown Out Detect (Controlled with SBOREN bit)
#pragma config BORV = 1         // Brown-out Reset Voltage bits (2.7V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = SWDTDIS  // Watchdog Timer (WDT enabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1024     // Watchdog Postscaler (1:1024)

// CONFIG3H
#pragma config CANMX = PORTC    // ECAN Mux bit (ECAN TX and RX pins are located on RC6 and RC7, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB1K     // Boot Block Size (1K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-03FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 04000-07FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 08000-0BFFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 0C000-0FFFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 0C000-0FFFF (Disabled)

// CONFIG6H
#pragma config WRTC = ON        // Config. Write Protect (Enabled)
#pragma config WRTB = ON        // Table Write Protect Boot (Enabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 0C000-0FFFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)



void ResetDevice(void);

uReg32 SourceAddr;
unsigned char buforCAN[65];
unsigned char counter = 0;
unsigned char ramekInicjalizacji = 0, iloscBajtow = 0;

#asm
PSECT intcode
GOTO 0xc08
PSECT intcodelo
GOTO 0xc18

#endasm

static void OdbierzRamke(void)
{
    volatile unsigned char *pointerU8;
    unsigned char i, k,tempCON = 0;
    unsigned int tempU16;

    k = CANCON&0x0F;
    //zmapmuj bufor odbiorczy na acces bank
    ECANCON = 0x90 | k;

    //errata workaround
    switch(k)
    {
        case 0:
            tempCON = RXB0CON;
            break;
        case 1:
            tempCON = RXB1CON;
            break;
        case 2:
            tempCON = B0CON;
            break;
        case 3:
            tempCON = B1CON;
            break;
        case 4:
            tempCON = B2CON;
            break;
        case 5:
            tempCON = B3CON;
            break;
        case 6:
            tempCON = B4CON;
            break;
        case 7:
            tempCON = B5CON;
            break;
    }
    //sprawdz czy cos w buforze jest
    if((tempCON & 0x80) > 0)
    {
        pointerU8 = &RXB0D0;
        for(i=0;i<8;i++)
        {
            buforCAN[iloscBajtow++]=(*(pointerU8+i));
        }
        if(ramekInicjalizacji < 10)
        {
            for(i=0; i<8; i++)
            {
                if(buforCAN[i] != ((ramekInicjalizacji*10)+i))
                {
                    break;
                }
            }
            if(i==8)
            {
                iloscBajtow = 0;
                ramekInicjalizacji++;
            }
            else
            {
                //cos nie tak z ramkami - wyjdz z bootloadera
                iloscBajtow = 0;
                ResetDevice();
            }
        }
        else if((iloscBajtow >= 64))
        {
            LED = !LED;
            EraseFlash(SourceAddr.Val32, SourceAddr.Val32+(unsigned long)64);
            WriteBytesFlash(SourceAddr.Val32,64,buforCAN);
            SourceAddr.Val32 += 64;
            iloscBajtow = 0;
        }
        TMR0 = 62500;
        counter = 0;
        ClrWdt();
        switch(k)
        {
            case 0:
                RXB0CON = 0;
                break;
            case 1:
                RXB1CON = 0;
                break;
            case 2:
                B0CON = 0;
                break;
            case 3:
                B1CON = 0;
                break;
            case 4:
                B2CON = 0;
                break;
            case 5:
                B3CON = 0;
                break;
            case 6:
                B4CON = 0;
                break;
            case 7:
                B5CON = 0;
                break;
        }
    }
}

void ResetDevice(void) {

  asm(" goto 0xC00");
}

int main(void)
{
    unsigned char Delay;

    //jezeli nie bylo resetu zasilania - idz od razu do programu glownego
    if((RCON&0x3F)!=0x3C)
    {
        ResetDevice();
    }

    Delay = 5;

    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISAbits.TRISA5 = 0;
    LED = 0;
    TRISC = 0xFF;
    TRISCbits.TRISC6 = 0;


    T0CON = 0x00;
    T0CONbits.T0PS = 0x05; // presc 64, osc FCY/4, 16bit
    
    INTCONbits.TMR0IE = 0;
    INTCONbits.TMR0IF = 0;

    TMR0 = 62500; // flaga co 1s

    /* Enable Timer */
    T0CONbits.TMR0ON = 1;

    // Enter CAN module into config mode
    CANCON = 0x80;
    while((CANSTAT & 0xE0)!=0x80)
    {
        ;
    }

    // Enter CAN module into Mode 2
    ECANCON = 0x90;

    //ustawienie predkosci przesylu danych
     //  50 Kbps @ 8MHz
    BRGCON1bits.BRP = BRP_VAL;         // Sync Segment = 1 TQ (0b00)
    BRGCON1bits.SJW = 0x3;
   BRGCON2bits.SEG1PH = 0b111; // Phase Segment 1 = 8 TQ
    BRGCON2bits.PRSEG = 0b100; // Propagation Delay = 5 TQ
    BRGCON3bits.SEG2PH = 0b101; // Phase Segment 2 = 6 TQ

    BRGCON2bits.SAM = 1;
    BRGCON2bits.SEG2PHTS = 1;


    //ustawienie maski
   // Wybor maski 0
    MSEL0 = 0x00;
    RXM0SIDH = 0b11100000;
    RXM0SIDL = 0b00001011;
    RXM0EIDH = 0xFF;
    RXM0EIDL = 0xFF;

    // Konfiguracja filtra 0 - tylko 2.0B z ustawionym ostatnim bitem ramki 0x3xx
    RXF0SIDH = 0x60;
    RXF0SIDL = 0x01;
    RXF0SIDLbits.EXIDEN = 1;
    RXF0EIDH = 0x40;
    RXF0EIDL = 0;


    // Wlacz tylko filtr 0
    RXFCON0 = 0x01;     //Enable Filter 0
    RXFCON1 = 0x00;     //Disable all others

//    CIOCON = 0x00; // CIOCON: CAN I/O CONTROL REGISTER str 437
    //bit 0 CLKSEL: CAN Clock Source Selection bit
    //1 = Use the oscillator as the source of the CAN system clock
    //0 = Use the PLL as the source of the CAN system clock

    //wyczysc bufory przychodzace
    RXB0CON = 0;
    RXB1CON = 0;
    B0CON = 0;
    B1CON = 0;
    B2CON = 0;
    B3CON = 0;
    B4CON = 0;
    B5CON = 0;

    // Enter CAN module into normal mode
    CANCON = 0x00;
    while((CANSTAT & 0xE0) > 0)
    {
        ;
    }
    SourceAddr.Val32 = PROGRAM_START;
    counter = 0;
    iloscBajtow = 0;
    ramekInicjalizacji = 0;
    for(;;)
    {
        OdbierzRamke();


        if(INTCONbits.TMR0IF == 1)
        {
            INTCONbits.TMR0IF = 0;
            ClrWdt();

            if(++counter >= Delay)
            {
                ResetDevice();
            }
        }
    }
}


/*void ResetDeviceasPOR(void) {

  RCONbits.POR = 1;//_POR = 1;
  asm(" goto 0xC00");
//  asm(" goto 0x1000");

}*/