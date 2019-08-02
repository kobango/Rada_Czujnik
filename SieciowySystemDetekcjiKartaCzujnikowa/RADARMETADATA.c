/*
 * File:   RADARMETADATA.c
 * Author: hp
 *
 * Created on 1 sierpnia 2019, 09:19
 */



#include <xc.h>
#include "main.h"
#include "RADARMETADATA.h"



void RADAR_Use(void)
{
    if(Flagi.pomiarTla == 1)
    {
        Flagi.detekcja = 0;  // TODO Radar function
        Flagi.pomiarTla = 0;
    }
}