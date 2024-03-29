#ifndef MAIN_H
    #define MAIN_H
    /////////////////////////////////////////////////////////////////
#include <xc.h>
//    #include <p18f26k80.h>
#include "GenericTypeDefs.h"
#include "DetekcjaSasiadow.h"

    #define FLASH_V1_4
//    #include <flash.h>
 //   #include "WeWy.h"

    #define FCY (32000000/4)  //14745600

/*
    #define	USERID_READ(addr) (	\
    EEADRH = 0, EEADR = addr,	\
    EECON1 |= 0x40,	\
    RD = 1,	NOP(), NOP(),	\
    (WORD)EEDATA)   
 * */

    #define ResetDevice()  {_asm goto 0xC00 _endasm}
//mm	#define Nop() {__asm__ volatile ("nop");}
//mm	#define ClrWdt() {__asm__ volatile ("clrwdt");}
//mm	#define Sleep() {__asm__ volatile ("pwrsav #0");}
    //#define Idle() {__asm__ volatile ("pwrsav #1");}
    #define _ISR __attribute__((interrupt))

    #define EEPROM_OFFSET 0x100

    #define CZAS_OCZEKIWANIA_NA_RAMKE_CAN 1//100 //razy 100ms  //aktualni 100 ms
    #define TIMER_RAMKI_STANU   (2*10)//(5*10)//(20*10)   //aktualni 200ms      //razy 100ms - 5 sek
//#define PARKINGI
#ifdef PARKINGI
//    #define TIMER_RAPORTOWANIA_CZUJNICZKOW ((WORD)20*(WORD)60*(WORD)15)    //razy 50ms - 15min
#else
    #define TIMER_RAPORTOWANIA_CZUJNICZKOW    (WORD)((WORD)20*(WORD)45)        //razy 50ms - 30 sek
#endif


    

    // define dla timerow
    #define b100ms_5sek 50
    #define b100ms_2_5sek 25
    #define b100ms_2sek 20
    #define b100ms_10sek 100

    #define Const_ADR 0x9FFE

     /////////////////////////////////////////////////////////////////////
    struct PozycjaNaMapceStruct
    {
        WORD xU16 ;
        WORD yU16 ;
    }; 

    struct DaneStruct
        {
            WORD numerSeryjnyU16 ;
            WORD startupU16 ;
            WORD wersjaOprogramowaniaU16 ;
            struct PozycjaNaMapceStruct PozycjaNaMapce;
            WORD wersjaSprzetuU16 ;
            WORD timerRysowaniaWykresuU16 ;
            sasiadStruct sasiedzi[MAX_SASIADOW];
            WORD rokU16, miesiacU16, dzienU16, godzinaU16, minutaU16;
            WORD NrKarty;
            WORD Nr_WeWy;
        };	
        
    struct FlagStruct{
		unsigned pomiarTla 	:1;
		unsigned detekcja	:1;
		unsigned zgloszenie	:1;
		unsigned zapisDoFlash   :1;
		unsigned wykonanoZapisDoFlash :1;
        unsigned pomiarAccelerometer :1;
        unsigned wykonanoReset :1;
        unsigned aktualizacjaSasiadow : 1;

        struct CANStruct{
            unsigned wyslijRamkeDanych :1;
            unsigned odebranoDane :1;
            unsigned CanAktywny : 1;
            WORD identyfikatorU16;
            }CAN;
            
            BYTE frameCounterU8;
		};
        
    
    /////////////////////////////////////////////////////////////////


    typedef struct{
         union
        {
            BYTE flagiU8;

            struct{
                unsigned obsluzWeWy : 1;
                unsigned error : 1;
                unsigned uczenieTla : 1;
                unsigned inicjalizacja : 1;
                unsigned ramkaTx : 1;
                unsigned wykonanoZapisDoFlash: 1;
                unsigned pomiarTla: 8;
            };
        }Flags;
        
        BYTE timerRamkiTxCANU8;
    }KartaStruct;
	/////////////////////////////////////////////////////////////////



    extern KartaStruct DetectorLedRadar;
    
    extern struct DaneStruct *Dane;
	extern struct FlagStruct Flagi;

    extern void _startup (void);
    void WylaczPrzerwania(void);
    void WlaczPotwierdzenie(void);
    void zapisUstawienDoEEPROM(void);
    void InterruptHandlerHigh(void);
    void INI_All(void);
/////////////////////////////////////////////////////////////////
#endif
