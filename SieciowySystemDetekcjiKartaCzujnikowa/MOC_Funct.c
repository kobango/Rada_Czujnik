/*
 * File:   MOC_Funct.c
 * Author: hp
 *
 * Created on 15 lipca 2019, 14:26
 */


#include <xc.h>
#include "CAN.h"
#include "main.h"
#include "LED.h"
#include "MOC_Funct.h"
UINT MOC_StanWzbudzenia(void);
UINT MOC_Wynikowa_wartosc_roznicowa(void);
UINT MOC_Frame_Counter(void);
UINT MOC_Aktualna_Temperatura(void);
UINT MOC_NOTWORK(void);
UINT MOC_RSSI_ramki(void);
UINT MOC_LQI_ramki(void);


UINT MOC_StanWzbudzenia(void)
{
    return 1;
}

UINT MOC_Wynikowa_wartosc_roznicowa(void)
{
    return 0x0220;
}

UINT MOC_Frame_Counter(void)
{
    return 0x47;
}
UINT MOC_Aktualna_Temperatura(void)
{
    return 0x1B;
}
UINT MOC_NOTWORK(void)
{
    return 0xFF;
}
UINT MOC_RSSI_ramki(void)
{
    return 0xEE;
}

UINT MOC_LQI_ramki(void)
{
    return 0xE1;
}

