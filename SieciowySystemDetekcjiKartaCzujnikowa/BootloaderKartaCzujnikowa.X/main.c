
#include "main.h"
#define FLASH_V1_4
#include <xc.h>
#include <plib.h>
#include <pic18f4680.h>
#include "LED.h"

#define PROGRAM_START (DWORD)0xC00      //start of application program, do not write below this address

#define VERSION_MAJOR 0x01       //this cannot be zero!
#define VERSION_MINOR 0x00



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
            LED_Error();
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
    LED_Clear();
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
    BRGCON1bits.BRP0 = BRP_VAL & 0b000001;         // Sync Segment = 1 TQ (0b00)  // 0b001111; // 2x16
    BRGCON1bits.BRP1 = (BRP_VAL >> 1) & 0b000001;  
    BRGCON1bits.BRP2 = (BRP_VAL >> 2) & 0b000001;  
    BRGCON1bits.BRP3 = (BRP_VAL >> 3) & 0b000001;  
    BRGCON1bits.BRP4 = (BRP_VAL >> 4) & 0b000001;  
    BRGCON1bits.BRP5 = (BRP_VAL >> 5) & 0b000001;  
    
    BRGCON1bits.SJW0 = 1;
    BRGCON1bits.SJW1 = 1;
    
    BRGCON2bits.SEG1PH0 = 1; // Phase Segment 1 = 8 TQ
    BRGCON2bits.SEG1PH1 = 1;
    BRGCON2bits.SEG1PH2 = 1;
    
    BRGCON2bits.PRSEG2 = 1; // Propagation Delay = 5 TQ
    BRGCON2bits.PRSEG1 = 0;
    BRGCON2bits.PRSEG0 = 0;
    
    BRGCON3bits.SEG2PH0 = 1; // Phase Segment 2 = 6 TQ
    BRGCON3bits.SEG2PH1 = 0;
    BRGCON3bits.SEG2PH2 = 1;

    BRGCON2bits.SAM = 1;
    BRGCON2bits.SEG2PHTS = 1;

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