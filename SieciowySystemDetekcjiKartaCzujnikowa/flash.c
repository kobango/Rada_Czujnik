/*
 * File:   FLASH.c
 * Author: Kamil Szkaradnik
 *
 * Created on 29 lipca 2019, 15:22
 */


#include <xc.h>
#include "FLASH.h"
#include "main.h"

UINT FLASH_Read(long int addr);
void FLASH_Write(long int addr,UINT val);

UINT FLASH_Read(long int addr)
{
    TBLPTR = addr;
    asm("TBLRD;");
    return TABLAT;
}

void FLASH_Write(long int addr,UINT val)
{
    UINT *flashWrBufPtr = &val;
    int i;
   UINT GIEBitValue = INTCONbits.GIE;     // Save interrupt enable


    TBLPTR = addr; 
        
    // Write 3 bytes which uses 6 LOC cells (1 nibble each).

    TABLAT = (flashWrBufPtr[0] >> 8);
    asm("TBLWT*");
    TABLAT = (flashWrBufPtr[0] & 0xff);
    asm("TBLWT*");
    
   
    TBLPTR = addr;  
    
    EECON1bits.EEPGD = 1;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    
    INTCONbits.GIE = 0; // Disable interrupts
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;  // Write like the wind!
    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable
    EECON1bits.WREN = 0;    // Disable writes to memory    
}
