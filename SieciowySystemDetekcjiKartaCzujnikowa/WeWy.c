#include "main.h"
#include "transmisja.h"
//#include "WeWy.h"

#pragma romdata eedata_nazwy=0xf00000
    rom BYTE nazwyWlasneCzujnikow[(WORD)WEWY*(WORD)DETEKTOROW_NA_WEWY*(WORD)BAJTOW_NAZWY] = {0};
#pragma romdata
/*******************************************************************
Funkcja: static void SprawdzWejscia(void)
Opis funkcji: Funkcja sprawdzajaca stan wejsc
Data: 13.09.13
Autor: Pawel Kasperek
*****************************************************************/
static void SprawdzWejscia(void)
{
    BYTE i, k, wejsciaU8=0;

    wejsciaU8 = PORTC&0x3F;
    wejsciaU8 |= (PORTA<<4)&0xC0;

    for(i=0; i<8; i++)
    {
        //sprawdz czy przycisk nacisniety i jak dlugo
        if((wejsciaU8 & (1<<i)) == 0)
        {
            if(kartaCzujnikowa.weWy[i].timerResetuU8 <= TIMER_UCZENIA_TLA)
            {
                kartaCzujnikowa.weWy[i].timerResetuU8++;
            }
        }
        else
        {
            if((kartaCzujnikowa.weWy[i].timerResetuU8 > 0) &&
                    (kartaCzujnikowa.weWy[i].timerResetuU8 < TIMER_UCZENIA_TLA))
            {
                //wyslij ramke resetu
                kartaCzujnikowa.weWy[i].Flagi.reset = 1;
                DaneCan.Flagi.wyslijRamkeStanu = 1;
            }
            else
            {
                kartaCzujnikowa.weWy[i].Flagi.reset = 0;
            }
            kartaCzujnikowa.weWy[i].timerResetuU8 = 0;
        }

        //jezeli przytrzymany wystarczajaco dlugo - wyslij uczenie tla
        if(kartaCzujnikowa.weWy[i].timerResetuU8 == TIMER_UCZENIA_TLA)
        {
            if(!kartaCzujnikowa.weWy[i].Flagi.uczenieTla)
            {
                DaneCan.Flagi.wyslijRamkeStanu = 1;
                DaneCan.Flagi.wyslijRamkeUczeniaTla = 1;
                kartaCzujnikowa.weWy[i].timerUczeniaTlaU8 = TIMER_WYKONANIA_UCZENIA_TLA;
            }
            kartaCzujnikowa.weWy[i].Flagi.uczenieTla = 1;
        }
        else
        {
            kartaCzujnikowa.weWy[i].Flagi.uczenieTla = 0;
        }
    }
}

/*******************************************************************
Funkcja: static void UstawWyjscia(void)
Opis funkcji: Funkcja ustawiajaca stany wyjsc
Data: 13.09.13
Autor: Pawel Kasperek
*****************************************************************/
static void UstawWyjscia(void)
{
    BYTE i, k, wyjsciaU8 = 0, erroryU8 = 0;
    static BYTE mruganieU8, stareWyjsciaU8, initWyjscia = 0, noWyjscia;

    mruganieU8 = !mruganieU8;

    if(kartaCzujnikowa.Flagi.inicjalizacja)
    {
        if(initWyjscia > (7*5))
        {
            kartaCzujnikowa.Flagi.inicjalizacja = 0;
        }
        else
        {
            noWyjscia = initWyjscia/5;
            kartaCzujnikowa.weWy[noWyjscia].czujnik[0].timerRamkiRxCanU16 =
                        (WORD)((WORD)3*TIMER_RAPORTOWANIA_CZUJNICZKOW);
            LATB = 1<<(noWyjscia);
            if((noWyjscia)>5)
            {
                LATA = 1<<((noWyjscia)-6);
            }
            else
            {
                LATA = 0;
            }
        }
        initWyjscia++;
    }
    else
    {
        for(i=0; i<WEWY; i++)
        {
            for(k=0; k<DETEKTOROW_NA_WEWY; k++)
            {
                if(kartaCzujnikowa.weWy[i].czujnik[k].Flagi.przypisanyCzujnik)
                {
                    //sprawdz stany czujnikow
                    if(kartaCzujnikowa.weWy[i].czujnik[k].Flagi.stan)
                    {
                        wyjsciaU8 |= 1<<i;
                    }
                    //ustaw wyjscie jezeli nie gada
                    if(kartaCzujnikowa.weWy[i].czujnik[k].Flagi.nieGada)
                    {
                        wyjsciaU8 |= 1<<i;
                        erroryU8 = 1;
                    }
                }
                //sprawdz stany przyciskow - przy przycisnietym zacznij zaswiec
                if(kartaCzujnikowa.weWy[i].timerResetuU8 > 0)
                {
                    wyjsciaU8 |= 1<<i;
                }
                if(kartaCzujnikowa.weWy[i].timerUczeniaTlaU8 > 0)
                {
                    //przy uczeniu tla zacznij mrugac
                    if(mruganieU8)
                    {
                        kartaCzujnikowa.weWy[i].timerUczeniaTlaU8--;
                        wyjsciaU8 |= 1<<i;
                    }
                    else
                    {
                        wyjsciaU8 &= ~(1<<i);
                    }
                }
            }
            //sprawdz czy wyjscie nie ma bledu
            if(erroryU8 > 0)
            {
                kartaCzujnikowa.weWy[i].Flagi.error = 1;
            }
            else
            {
                kartaCzujnikowa.weWy[i].Flagi.error = 0;
            }
            erroryU8 = 0;
            //ustaw flagi stanow wyjsc
            if((wyjsciaU8&(1<<i))>0)
            {
                kartaCzujnikowa.weWy[i].Flagi.stan = 1;
            }
            else
            {
                kartaCzujnikowa.weWy[i].Flagi.stan = 0;
            }

        }

        //ustaw fizycznie wyjscia na diodach
        if((wyjsciaU8&0x40) > 0)
        {
            LATAbits.LATA0 = 1;
        }
        else
        {
            LATAbits.LATA0 = 0;
        }
        if((wyjsciaU8&0x80) > 0)
        {
            LATAbits.LATA1 = 1;
        }
        else
        {
            LATAbits.LATA1 = 0;
        }
        LATB = wyjsciaU8;
    }
}

/*******************************************************************
Funkcja: void SprawdzCzasyRaportowaniaCzujnikow(void)
Opis funkcji: Funkcja sprawdzajaca czy czujniki jeszcze gadaja
Data: 18.11.13
Autor: Pawel Kasperek
*****************************************************************/
void SprawdzCzasyRaportowaniaCzujnikow(void)
{
    BYTE i, k, blad = 0;

    for(i=0; i<WEWY; i++)
    {
        for(k=0; k<DETEKTOROW_NA_WEWY; k++)
        {
            if(kartaCzujnikowa.weWy[i].czujnik[k].Flagi.przypisanyCzujnik)
            {
                kartaCzujnikowa.weWy[i].czujnik[k].timerRamkiRxCanU16++;
                if(kartaCzujnikowa.weWy[i].czujnik[k].timerRamkiRxCanU16 >
                        (WORD)((WORD)3*TIMER_RAPORTOWANIA_CZUJNICZKOW))
                {
                    kartaCzujnikowa.weWy[i].czujnik[k].Flagi.nieGada = 1;
                    kartaCzujnikowa.Flagi.error = 1;
                    kartaCzujnikowa.weWy[i].czujnik[k].timerRamkiRxCanU16 =
                        (WORD)((WORD)3*TIMER_RAPORTOWANIA_CZUJNICZKOW);
                    blad = 1;
                }
                else
                {
                    kartaCzujnikowa.weWy[i].czujnik[k].Flagi.nieGada = 0;
                }
            }
        }
    }
    if(blad==0)
    {
        kartaCzujnikowa.Flagi.error = 0;
    }
}

/*******************************************************************
Funkcja: void StatusLed(void)
Opis funkcji: Funkcja aktualizujaca diode stanu
Data: 19.11.13
Autor: Pawel Kasperek
*****************************************************************/
void StatusLed(void)
{
    static BYTE timerLedU8 = 0;

    if(kartaCzujnikowa.Flagi.error)
    {
        timerLedU8++;

        if(timerLedU8 < 10)
        {
            STATUS_LED = LED_ON;
        }
        else if(timerLedU8 < 20)
        {
            STATUS_LED = LED_OFF;
        }
        else
        {
            timerLedU8 = 0;
        }
    }
    else
    {
        if(kartaCzujnikowa.statusLed.Flagi.ramkaRx)
        {
            kartaCzujnikowa.statusLed.Flagi.ramkaRx = 0;
            STATUS_LED = LED_ON;
        }
        else if(kartaCzujnikowa.statusLed.Flagi.ramkaTx)
        {
            kartaCzujnikowa.statusLed.Flagi.ramkaTx = 0;
            STATUS_LED = LED_ON;
        }
        else
        {
            STATUS_LED = LED_OFF;
        }
    }
}

/*******************************************************************
Funkcja: void ObsluzWeWy(void)
Opis funkcji: Funkcja aktualizujaca stan wejsc wyjsc
Data: 13.09.13
Autor: Pawel Kasperek
*****************************************************************/
void ObsluzWeWy(void)
{
    SprawdzCzasyRaportowaniaCzujnikow();
    SprawdzWejscia();
    UstawWyjscia();
    StatusLed();
}

/*******************************************************************
Funkcja: void AktualizujDaneCzujnika(WORD adresU16, BYTE stanU8)
Opis funkcji: Funkcja aktualizujaca stan czujnika
Data: 13.09.13
Autor: Pawel Kasperek
*****************************************************************/
BYTE AktualizujDaneCzujnika(WORD adresU16, BYTE stanU8, BYTE frameCounterU8)
{
    BYTE i, k;

    for(i=0; i<WEWY; i++)
    {
        for(k=0; k<DETEKTOROW_NA_WEWY; k++)
        {
            if(kartaCzujnikowa.weWy[i].czujnik[k].adresCzujnikaU16 == adresU16)
            {
                //przyszla ramka od czujniczka przypisanego do karty - zamrugaj dioda
                kartaCzujnikowa.statusLed.Flagi.ramkaRx = 1;
                kartaCzujnikowa.weWy[i].czujnik[k].timerRamkiRxCanU16 = 0;
                if(stanU8 == 0)
                {
                    kartaCzujnikowa.weWy[i].czujnik[k].Flagi.stan = 0;
                }
                else
                {
                    kartaCzujnikowa.weWy[i].czujnik[k].Flagi.stan = 1;
                }
                
                //sprawdz czy licznik sie zgadza, jak nie - wystaw stan zajetosci
                //pozwalaj na zdublowane ramki
                if((kartaCzujnikowa.weWy[i].czujnik[k].frameCounterU8 == frameCounterU8) ||
                        ((kartaCzujnikowa.weWy[i].czujnik[k].frameCounterU8+1) == frameCounterU8))
                {
                    Nop();
                }
                else
                {
                    kartaCzujnikowa.weWy[i].czujnik[k].Flagi.stan = 1;
                }
                kartaCzujnikowa.weWy[i].czujnik[k].frameCounterU8 = frameCounterU8;
                return 1;
            }
        }
    }
    return 0;
}
