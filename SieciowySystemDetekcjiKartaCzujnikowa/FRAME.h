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



#endif	/* RAMKICZUJNIKA_H */

