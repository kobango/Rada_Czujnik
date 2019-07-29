/* 
 * File:   FLASH.h
 * Author: Kamil Szkaradnik
 *
 * Created on 29 lipca 2019, 15:24
 */

#ifndef FLASH_H
#define	FLASH_H
#include "main.h"

UINT FLASH_Read(long int addr);
void FLASH_Write(long int addr,UINT val);



#endif	/* RAMKICZUJNIKA_H */