#include "main.h"
#include "TRM.h"
#include "CAN.h"
#include "FRAME.h"

#define BITRATE 50000
#define NTQ 20 //20 time quanta in a bit time
#define BRP_VAL ((FCY/(2*NTQ*BITRATE))-1)

static void CAN_SetupClock(void);
static void CAN_SetupMask(void);
void CAN_Setup(void);

/***************************************************************************************/
/**
* @author Pawe? Kasperek
* @date 15/04/2011
*
* @section Description
* Initialization of the CAN
*****************************************************************************************/
void CAN_Setup(void)
{
    
    
    // Enter CAN module into config mode
    CANCON = 0x80;
    while((CANSTAT & 0xE0)!=0x80)
    {
        ;
    }

    BSEL0 = 0;
    TXB0CON = 0;
    RXB0CON = 0;
    RXB1CON = 0;
    B0CON = 0;
    B1CON = 0;
    PIE3 = 0;
    IPR3 = 0xFF;
    PIR3 = 0x00;
    BIE0 = 0;

    // Enter CAN module into Mode 2
    ECANCON = 0x90;

    //ustawienie predkosci przesylu danych
    CAN_SetupClock();

    //ustawienie maski
    CAN_SetupMask();

    CIOCON = 0xff; // CIOCON: CAN I/O CONTROL REGISTER str 437
    //bit 0 CLKSEL: CAN Clock Source Selection bit
    //1 = Use the oscillator as the source of the CAN system clock
    //0 = Use the PLL as the source of the CAN system clock
        // Enter CAN module into normal mode
    
    CANCON = 0x00;
    while((CANSTAT & 0xE0) != 0x00)
    {
        ;
    }
}



/*******************************************************************
Funkcja: static void SetupMask(void)
Opis funkcji: funkcja ustawiajaca wlasciwy adres i filtr danych
Data: 15.04.2011
*****************************************************************/
/***************************************************************************************/
/**
* @author Pawe? Kasperek
* @date 15/04/2011
*
* @section Description
* Function setings correct address and data filter
*****************************************************************************************/
static void CAN_SetupMask(void)
{
    MSEL0 = 0x50; // filtry 0 i 1 do maski 0, filtry 2 i 3 do maski 1
    MSEL1 = 0x55; // filtry 4, 5, 6, 7 do maski 1
    MSEL2 = 0x55; // filtry 8, 9, 10, 11 do maski 1
    MSEL3 = 0x55; // filtry 12, 13, 14, 15 do maski 1

    // Konfiguracja maski 0 - filtrowanie pojedynczego adresu
    RXM0SIDH = 0b00000000;
    RXM0SIDL = 0b00001011; // tylko extended
    RXM0EIDH = 0xFF;
    RXM0EIDL = 0xFF;

    // Konfiguracja maski 1 - filtrowanie adresów czujniczków
    RXM1SIDH = 0xFF;
    RXM1SIDL = 0xFF; // tylko extended
    RXM1EIDH = 0;
    RXM1EIDL = 0;

    // Konfiguracja filtra 0 i 1 - ramki kart czujnikowych
    RXF0SIDH = 0x00;
    RXF0SIDL = 0x01;
    RXF0SIDLbits.EXIDEN = 1;
    RXF0EIDH = (BYTE)(DaneCan.adresCAN>>8);
	RXF0EIDH |= 0x40;
    RXF0EIDL = (BYTE)DaneCan.adresCAN;

    RXF1SIDH = 0x00;
    RXF1SIDL = 0x01;    //ramki kart czujnikowych
    RXF1SIDLbits.EXIDEN = 1;
    RXF1EIDH = 0x7F;//akceptuj broadcasty
    RXF1EIDL = 0xFF;//akceptuj broadcasty

    //Konfiguracja filtra 2 - ramki stanu czujnikow przewodowych i bezprzewodowych
    RXF2SIDH = 0;
    RXF2SIDL = 0x20;
    RXF2SIDLbits.EXIDEN = 1;
    RXF2EIDH = 0;
    RXF2EIDL = 0;

    //przypisz filtr 0 i 1 do RXB0
//    RXFBCON0 = 0x00;
    //przypisz filtr 2 do RXB1
//    RXFBCON1 = 0x01;

    // Wlacz filtr 0, 1 i 2
    RXFCON0 = 0x07;

}

/*******************************************************************
Funkcja: static void SetupCanClock(void)
Opis funkcji: funkcja ustawiajaca predkosc przesylu danych
Data: 15.04.2011
*****************************************************************/
static void CAN_SetupClock(void)
{
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
}

/*******************************************************************
Funkcja: static void przerzucBufDoRam(BYTE* Ram, BYTE* Buf)
Opis funkcji: funkcja kopiujaca paczke danych do bufora wyjsciowego
Data: 15.04.2011
*****************************************************************/
static void CAN_MoveBuffIntoFrame(BYTE* Ram, BYTE* Buf)
{
    BYTE i;
    for(i=0; i<8; ++i)
    {
        Ram[i]=(*(Buf+i));
    }
}

/*******************************************************************
Funkcja: void WyslijRamkeCan(mID * message)
Opis funkcji: funkcja kopiujÄ?ca paczkÄ? danych do bufora wyj??ciowego
Data: 15.04.2011
*****************************************************************/
void CAN_SendFrame(mID * message)
{
    //znajdz wolny bufor do wyslania danych
    //poczekaj az poprzednia ramka zostanie wyslana
    for(;;)
    {
        ECANCON = 0x83;
        if(!RXB0CONbits.FILHIT3)//!TXB0CONbits.TXREQ
        {
            //TXB0 wolny - ustaw pointer
            break;
        }
        ECANCON = 0x84;
        if(!RXB0CONbits.FILHIT3)//!TXB1CONbits.TXREQ
        {
            //TXB1 wolny - ustaw pointer
            break;
        }
        ECANCON = 0x85;
        if(!RXB0CONbits.FILHIT3)//!TXB2CONbits.TXREQ
        {
            //TXB2 wolny - ustaw pointer
            break;
        }
    }
    //zatrzymaj transmisje
    while(COMSTATbits.TXWARN)
    {
        Reset();
    }
    DetectorLedRadar.timerRamkiTxCANU8 = 0;
    RXB0EIDL = message->id.v[0]; //
    RXB0EIDH = message->id.v[1]; //
    RXB0SIDL = ((message->id.v[2]&0xFC)*8) | (message->id.v[2] & (0x03)); //
    RXB0SIDLbits.EXID = 1;
    RXB0SIDH = (BYTE)(message->id.w[1]/32); //

    RXB0DLC = 0; //
    // check to see if the message is an RTR message 
    if(message->message_type==CAN_MSG_RTR)
    {
        RXB0DLCbits.RXRTR = 1;
    }
    else
    {
        RXB0DLCbits.DLC0 = (message->data_length & 0x0F); //
        RXB0DLCbits.DLC1 = ((message->data_length & 0x0F)>> 1); //
        RXB0DLCbits.DLC2 = ((message->data_length & 0x0F)>> 2); //
        RXB0DLCbits.DLC3 = ((message->data_length & 0x0F)>> 3); //
        // fill the data 
        CAN_MoveBuffIntoFrame(&RXB0D0, message->data);
    }

    RXB0CONbits.FILHIT3 = 1; 
    //TXB0CONbits.TXREQ = 1;
    DetectorLedRadar.Flags.ramkaTx = 1;
  
}

/*******************************************************************
Funkcja: void OdbierzRamkeCan(mID * message)
Opis funkcji: funkcja konwertujaca bufor wejsciowy do przyjaznej formy
Data: 15.04.2011
*****************************************************************/
BOOL CAN_TakeFrame(mID * message)
{
    BYTE k, tempCON;
    
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
        message->buffer = RXB0CON & 0x1F;
        message->id.v[0] = RXB0EIDL;
        message->id.v[1] = RXB0EIDH;
        message->id.v[2] = (RXB0SIDL/8) | (RXB0SIDL&0x03);
        message->id.w[1] |= (WORD)RXB0SIDH*32;
        message->frame_type=CAN_FRAME_EXT;

        // check to see what type of message it is
        // normal message
        if(RXB0DLCbits.RXRTR == 0)
        {
            message->message_type = CAN_MSG_DATA;
            message->data_length= RXB0DLC & 0x0F;
            if(message->data_length > 8)
            {
                //cos jest nie tak z ramka - wywal
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
                return FALSE;
            }
            CAN_MoveBuffIntoFrame(message->data, &RXB0D0);
        }
        // RTR message
        else
        {
            message->message_type=CAN_MSG_RTR;
        }

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
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*******************************************************************
Funkcja: void UzupelnijRamkeDanychCan(mID * message, unsigned int identyfikator)
Opis funkcji: funkcja przygotowujaca ramke danych do przeslania
Data: 20.08.2012
Modyfikacje: Mariusz Chrobak
*****************************************************************/
void CAN_GenID(mID * message, BYTE frameID)
{
    
    message->frame_type = CAN_FRAME_EXT;
    message->message_type = CAN_MSG_DATA;
    message->id.w[1] = 0x0025;//(WORD)frameID * (WORD)4;
    message->id.w[0] = 0x8006;//DaneCan.adresCAN;
    message->id.v[2] |= 0x01; //0x01;
    message->id.v[1] |= 0x00;//0x40;
     
}
