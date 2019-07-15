#include "main.h"
#include "CAN.h"
//#include "RamkiPrzycisku.h"


#define BITRATE 50000
#define NTQ 20 //20 time quanta in a bit time
#define BRP_VAL ((FCY/(2*NTQ*BITRATE))-1)

mID canRxMessage, canTxMessage;

static void SetupCanClockB(void);
static void SetupMaskB(void);


/*******************************************************************
Funkcja: void SetupCan(void)
Opis funkcji: inicjalizacja portu CAN - 100kbps
Data: 15.04.2011
*****************************************************************/
void SetupCanB(void)
{
    // Enter CAN module into config mode
    CANCON = 0x80;
    while((CANSTAT & 0xE0)!=0x80)
    {
        ;
    }

    // Enter CAN module into Mode 1
    ECANCON = 0x40;

    //ustawienie predkosci przesylu danych
    SetupCanClockB();

    //ustawienie maski
    SetupMaskB();

    BSEL0bits.B0TXEN = 0;
    BSEL0bits.B1TXEN = 0;
    BSEL0bits.B2TXEN = 1;
    TXB0CONbits.TXPRI = 0b11; //0b11; highest // Transmit Priority bits

    // Enter CAN module into normal mode
    CANCON = 0x00;
    while((CANSTAT & 0xE0) > 0)
    {
        ;
    }

    // Set Receive Mode for buffers
    RXB0CON = 0x00;
    RXB1CON = 0x00;
    B0CON = 0x00;
    B1CON = 0x00;

    // Enable transmit interrupt
    PIE5bits.TXBnIE = 0;
    IPR5bits.TXBnIP = 0;
    // kasowanie flag
    PIR5bits.TXBnIF = 0;

    // Enable ECAN1 receive Interrupt
    PIE5bits.RXBnIE = 0;
    IPR5bits.RXBnIP = 0;
    //kasowanie flag
    PIR5bits.RXBnIF = 0;

    BIE0bits.B0IE = 1;
    BIE0bits.RXB0IE = 1;

    CIOCON = 0x00; // CIOCON: CAN I/O CONTROL REGISTER str 437
    //bit 0 CLKSEL: CAN Clock Source Selection bit
    //1 = Use the oscillator as the source of the CAN system clock
    //0 = Use the PLL as the source of the CAN system clock
}

/*******************************************************************
Funkcja: static void SetupMask(void)
Opis funkcji: funkcja ustawiajaca wlasciwy adres i filtr danych
Data: 15.04.2011
*****************************************************************/
static void SetupMaskB(void)
{
    // Wybor maski 0
    MSEL0 = 0x00;
    RXM0SIDH = 0b00000000;
    RXM0SIDL = 0b00001011;
    RXM0EIDH = 0xFF;
    RXM0EIDL = 0xFF;

    // Konfiguracja filtra 0 - tylko 2.0B z ustawionym ostatnim bitem
    RXF0SIDH = 0x00;
    RXF0SIDL = 0x01;
    RXF0SIDLbits.EXIDEN = 1;
    RXF0EIDH = 0x40;
    RXF0EIDL = 0;

    // Uzyj bufora 0 do danych przychodzacych
    RXFBCON0 = 0xF0;    //Assign Filter 0 to RXB0 and F1 to RXB1
    RXFBCON1 = 0xFF;    //Assign the rest of the buffers with no filter
    RXFBCON2 = 0xFF;
    RXFBCON3 = 0xFF;
    RXFBCON4 = 0xFF;
    RXFBCON5 = 0xFF;
    RXFBCON6 = 0xFF;
    RXFBCON7 = 0xFF;
    
    // Wlacz tylko filtr 0
    RXFCON0 = 0x01;     //Enable Filter 0 and F1
    RXFCON1 = 0x00;     //Disable all others
}

/*******************************************************************
Funkcja: static void SetupCanClock(void)
Opis funkcji: funkcja ustawiajaca predkosc przesylu danych
Data: 15.04.2011
*****************************************************************/
static void SetupCanClockB(void)
{
    //  100 Kbps @ 64MHz
    BRGCON1bits.BRP = BRP_VAL;         // Sync Segment = 1 TQ (0b00)
    BRGCON2bits.SEG1PH = 0b111; // Phase Segment 1 = 8 TQ
    BRGCON2bits.PRSEG = 0b100; // Propagation Delay = 5 TQ
    BRGCON3bits.SEG2PH = 0b101; // Phase Segment 2 = 6 TQ

    BRGCON2bits.SAM = 1;
    BRGCON2bits.SEG2PHTS = 1;
}

void przerzucBufDoRamB(unsigned char* Ram, unsigned char* Buf)
{
    unsigned char i = 0;
    for(;i<8;++i)
    {
        Ram[i]=(*(Buf+i));
    }
}
/*******************************************************************
Funkcja: void OdbierzRamkeCan(mID * message)
Opis funkcji: funkcja konwertujaca bufor wejsciowy do przyjaznej formy
Data: 15.04.2011
*****************************************************************/
void OdbierzRamkeCanB(mID * message)
{
    switch(message->buffer)
    {
        case 1: // RXB0
        {
            przerzucBufDoRamB(message->data,&RXB0D0);
            message->data_length=RXB0DLC & 0x0F;
            ZapiszDane();
            break;
        }
        case 2: // RXB1
        {
            przerzucBufDoRamB(message->data,&RXB1D0);
            message->data_length=RXB1DLC & 0x0F;
            ZapiszDane();
            break;
        }
    }
	
}
