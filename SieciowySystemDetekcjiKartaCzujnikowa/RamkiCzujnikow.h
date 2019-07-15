
#ifndef RAMKICZUJNIKOW_H
    #define	RAMKICZUJNIKOW_H
    #include "CAN.h"

    extern mID ramkaCanRxCzujnika[RX_BUF_SIZE];
    
    void ObsluzRamkeCzujnika(mID *message);


#endif	/* RAMKICZUJNIKOW_H */

