#include "main.h"
#include "transmisja.h"
#include "EEPROM.h"
#include "FRAME.h"

WORD        ustawieniaKarty;
WORD        neighborsAdress;
WORD        czujnikiNaMapie;

WORD nextEEPosition;

/***************************************************************************************/
/**
* @author Mariusz Chrobak
* @date 01/08/2012
*
* @return 1 when succes
* @param *location Pointer ino EEPROM location
* @param size  Data count
* 
* @section Description
* Function to location position into EEPROM.
*****************************************************************************************/
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

/***************************************************************************************/
/**
* @author Mariusz Chrobak
* @date 01/08/2012
*
* @return 1 when succes
* 
* @section Description
* EEPROM save init
*****************************************************************************************/

BOOL NVMInit(void)
{
    BOOL result = TRUE;
    UINT NeightAdress_Example = 0x0000;
    nextEEPosition = 0;

    result &= NVMalloc(sizeof(KartaStruct), &ustawieniaKarty);
    result &= NVMalloc(8*sizeof(NeightAdress_Example), &neighborsAdress);
    

    return result;
}

/***************************************************************************************/
/**
* @author Mariusz Chrobak
* @date 01/08/2012
*
* @param Address Addres of data in EEPROM memory
* @param Data Data to save in EEPROM memory
*   
* @section Description
* Byte Read in to descripted adres.
*****************************************************************************************/
BYTE ReadfromEEPROM(WORD Address)
{
    EEADRH = (BYTE)(Address>>8); // Load the high byte of the EEPROM address
    EEADR = (BYTE)Address; // Load the low byte of the EEPROM address
    EECON1bits.RD = 1; // Do the read
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    return EEDATA; // Return with the data
}

UINT EEPROM_Adres_Readfrom(UINT StartEEPROMAdres, UINT MinAdres, UINT MaxAdres)
{
 // Void value in EEPROM is 0xFF
    UINT ReadAdressOfNeighbor = 0;
    ReadAdressOfNeighbor = ReadfromEEPROM(StartEEPROMAdres);
    ReadAdressOfNeighbor = ((ReadfromEEPROM(StartEEPROMAdres+1)<<8)&0xFF00)| ReadAdressOfNeighbor;
    if(ReadAdressOfNeighbor==0xFFFF || (MinAdres>ReadAdressOfNeighbor)||(ReadAdressOfNeighbor>MaxAdres))
    {
        ReadAdressOfNeighbor=0;
    }
    return ReadAdressOfNeighbor;
}
/***************************************************************************************/
/**
* @author Mariusz Chrobak
* @date 01/08/2012
*
* @param Address Addres of data in EEPROM memory
* @param Data Data to save in EEPROM memory
* 
* @section Description
* Byte save in to descripted adres.
*****************************************************************************************/
void WritetoEEPROM(WORD Address, BYTE Data)
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

void EEPROM_Adres_Writeto(UINT StartEEPROMAdres, UINT AdressOfNeighbor, UINT MinAdres, UINT MaxAdres)
{
if((MinAdres>AdressOfNeighbor) || (AdressOfNeighbor>MaxAdres)) // Void value in EEPROM is 0xFF
    {
        AdressOfNeighbor=0;
    }
    WritetoEEPROM(StartEEPROMAdres,AdressOfNeighbor&0xFF);
    WritetoEEPROM(StartEEPROMAdres+1,((AdressOfNeighbor>>8)&0x00FF));
}
/***************************************************************************************/
/**
* @author Mariusz Chrobak
* @date 01/08/2012
*
* @param *dest Pointer ino target location
* @param addr  Addres into EEPROM
* @param count  Data count
* 
* @section Description
* Function to Write data from EEPROM
*****************************************************************************************/
void NVMRead(BYTE *dest, WORD addr, WORD count)
{
    while( count > 0 )
    {
        while(EECON1bits.WR);
        *dest++ = ReadfromEEPROM(addr++);
        count--;
    }
}


/***************************************************************************************/
/**
* @author Mariusz Chrobak
* @date 01/08/2012
*
* @param *source Pointer ino source location
* @param addr  Addres into EEPROM
* @param count  Data count
* 
* @section Description
* Function to read data from EEPROM
*****************************************************************************************/
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

