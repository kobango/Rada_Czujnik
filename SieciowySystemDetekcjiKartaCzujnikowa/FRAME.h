/* 
 * File:   RamkiCzujnika.h
 * Author: Pawel Kasperek <p.kasperek at ksk-dev.com>
 *
 * Created on 14 marzec 2014, 15:41
 */

#ifndef FRAME_H
#define	FRAME_H
#include "CAN.h"

extern mID ramkaCanRxCzujnika[RX_BUF_SIZE];
void FRAME_HandleCanFrame(mID * message);
void ReadDataToEEPROM(void);
void WriteDataToEEPROM(void);

volatile UINT NeightAdress1;
volatile UINT NeightAdress2;
volatile UINT NeightAdress3;
volatile UINT NeightAdress4;
volatile UINT NeightAdress5;
volatile UINT NeightAdress6;
volatile UINT NeightAdress7;
volatile UINT NeightAdress8;

#endif	/* RAMKICZUJNIKA_H */

