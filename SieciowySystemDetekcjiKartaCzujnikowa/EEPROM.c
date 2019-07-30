#include "main.h"
#include "transmisja.h"
#include "EEPROM.h"
#include "FRAME.h"

WORD        ustawieniaKarty;
WORD        neighborsAdress;
WORD        czujnikiNaMapie;

WORD nextEEPosition;
BOOL NVMalloc(WORD size, WORD *location)
{
    //WORD retval;

    if ((nextEEPosition + (WORD)size) > TOTAL_NVM_BYTES)
    {
        return FALSE;
    }

    *location = nextEEPosition;
    nextEEPosition += size;
    return TRUE;
}


BOOL NVMInit(void)
{
    BOOL result = TRUE;
    UINT NeightAdress_Example = 0xFFFF;
    nextEEPosition = 0;

    result &= NVMalloc(sizeof(KartaStruct), &ustawieniaKarty);
    result &= NVMalloc(8*sizeof(NeightAdress_Example), &neighborsAdress);
    

    return result;
}

/*******************************************************************
Funkcja: unsigned char ReadfromEEPROM(unsigned int Address)
Opis funkcji: Czytanie pojedynczego bajtu ze wskazanego adresu
Data: sierpien 2012
Autor: Mariusz Chrobak
*****************************************************************/
static BYTE ReadfromEEPROM(WORD Address)
{
    EEADRH = (BYTE)(Address>>8); // Load the high byte of the EEPROM address
    EEADR = (BYTE)Address; // Load the low byte of the EEPROM address
    EECON1bits.RD = 1; // Do the read
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    return EEDATA; // Return with the data
}

/*******************************************************************
Funkcja: void WritetoEEPROM(unsigned int Address, unsigned char Data)
Opis funkcji: Zapis pojedynczego bajtu pod wskazany adres
Data: sierpien 2012
Autor: Mariusz Chrobak
*****************************************************************/
static void WritetoEEPROM(WORD Address, BYTE Data)
{
    static BYTE GIE_Status; // Variable to save Global Interrupt Enable bit

    EEADRH = (BYTE)(Address>>8); // Load the high byte of the EEPROM address
    EEADR = (BYTE)Address; // Load the low byte of the EEPROM address
    EEDATA = Data; // Load the EEPROM data
    EECON1bits.WREN = 1; // Enable EEPROM writes
    GIE_Status = INTCONbits.GIE; // Save the Global Interrupt Enable bit
    INTCONbits.GIE = 0; // Disable global interrupts
    EECON2 = 0x55; // Required sequence to start the write cycle
    EECON2 = 0xAA; // Required sequence to start the write cycle
    EECON1bits.WR = 1; // Required sequence to start the write cycle
    INTCONbits.GIE = GIE_Status; // Restore the Global Interrupt Enable bit
    EECON1bits.WREN = 0; // Disable EEPROM writes
    while (EECON1bits.WR); // Wait for the write cycle to complete
}

void NVMRead(BYTE *dest, WORD addr, WORD count)
{
    while( count > 0 )
    {
        while(EECON1bits.WR);
        *dest++ = ReadfromEEPROM(addr++);
        count--;
    }
}


void NVMWrite(BYTE *source, WORD addr, WORD count)
{
    while(count > 0)
    {
        while(EECON1bits.WR);      // wait for errase to complete
        WritetoEEPROM(addr++,*source);
        count--;//=count-2;
        source++;
    }
}

/*******************************************************************
Funkcja: void UstawFlagi(void)
Opis funkcji: Odczytywanie z wew FLASH ustawien przycisku
Data: sierpien 2012
Autor: Mariusz Chrobak
*****************************************************************/

/*
void UstawFlagi(void)
{
    WORD temp[4];
    BYTE i,k;

    nvmGetUstawieniaKarty(&kartaCzujnikowa);

    for(i=0; i<WEWY; i++)
    {
        for(k=0; k<DETEKTOROW_NA_WEWY; k++)
        {
            if(kartaCzujnikowa.weWy[i].czujnik[k].adresCzujnikaU16 > 0)
            {
                kartaCzujnikowa.weWy[i].czujnik[k].Flagi.przypisanyCzujnik = 1;
                kartaCzujnikowa.weWy[i].czujnik[k].timerRamkiRxCanU16 = 0;
                kartaCzujnikowa.weWy[i].czujnik[k].Flagi.stan = 0;
            }
        }
    }
    ReadFlash(0x400,4,temp);
    //TODO opanowac dwubajtowy adres
//    DaneCan.adresCAN = ((unsigned int)temp[2]<<8) | (unsigned int)temp[0];
    DaneCan.adresCAN = (unsigned int)temp[0];
}
 * */

