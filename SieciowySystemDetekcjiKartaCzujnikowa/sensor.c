#include "main.h"
#include "analog.h"
#include "sensor.h"
#include "flash.h"
#include "CAN.h"

#define OS_X BufferA[(4*i)+1]
#define OS_Y BufferA[(4*i)+2]
#define OS_Z BufferA[(4*i)+3]

SensorStruct *Sensor = &daneU16[31];

static void UsrednianieTla(void);
//static unsigned int UtworzPoziomStartowy(struct SRStruct *Polaryzacja);
static unsigned int ObliczRoznice( int liczba1U16,  int liczba2U16);

/*******************************************************************
Funkcja: void Sensor(void)
Opis funkcji: funkcja obliczajaca sumaryczna wartosc wzbudzenia
Data: 21.12.2012
*****************************************************************/
static void SumaryczneWzbudzenie(void)
{
    unsigned int i;


    Sensor->obliczonaWynikowaRoznicaZgloszeniaU16 = 0;
    for(i=0; i<3; i++)
    {
        //oblicz sumaryczne wzbudzenie
        Sensor->obliczonaWynikowaRoznicaZgloszeniaU16 += Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaU16;

    }
    //zapamietaj maksymalna wartosc od ostatniej ramki CAN
    if(Sensor->obliczonaWynikowaRoznicaZgloszeniaU16 > Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16)
    {
        Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16 = Sensor->obliczonaWynikowaRoznicaZgloszeniaU16;
    }

}

/*******************************************************************
Funkcja: void Sensor(void)
Opis funkcji: funkcja ustawiajaca flage detekcji
Data: 21.12.2012
*****************************************************************/
static void Detekcja(void)
{
    static WORD timerWyjsciaZeWzbudzeniaU16;
    
    if(Flagi.detekcja)
    {
        if(Sensor->obliczonaWynikowaRoznicaZgloszeniaU16 < Sensor->roznicaZgloszeniaMinU16)
        {
            //odliczaj czas detekcji
            if(++timerWyjsciaZeWzbudzeniaU16 >= Sensor->czasWyjsciaZeWzbudzeniaU16)
            {
                Flagi.detekcja = 0;
                timerWyjsciaZeWzbudzeniaU16 = 0;
            }
        }
        else
        {
            timerWyjsciaZeWzbudzeniaU16 = 0;
        }
    }
    else
    {
        if(Sensor->obliczonaWynikowaRoznicaZgloszeniaU16 > Sensor->roznicaZgloszeniaMaxU16)
        {
            Flagi.detekcja = 1;
        }
    }
}

/*******************************************************************
Funkcja: static void ObliczWartoscRoznicowa(void)
Opis funkcji: funkcja obliczajaca wartosc roznicowa odczytow set i reset
Data: 15.07.2013
*****************************************************************/
static void ObliczWartoscRoznicowa(void)
{
    unsigned int i;

    for(i=0; i<3; i++)
    {
        //oblicz wartosc roznicowa pomiedzy odczytem set i reset
        Sensor->OsXYZ[i].wartoscRoznicowaS16 = (int)Sensor->OsXYZ[i].analogowySetResetU16[0] -
                    (int)Sensor->OsXYZ[i].analogowySetResetU16[1];
        //oblicz roznice w kazdej z osi pomnozona przez mnoznik
        Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaU16 = ObliczRoznice(Sensor->OsXYZ[i].wartoscRoznicowaS16,
            Sensor->OsXYZ[i].aktualneTloS16) * Sensor->OsXYZ[i].mnoznikU16;

        //zapamietaj maksymalna wartosc od ostatniej ramki CAN
        if(Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaU16 > Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16)
        {
            Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16 = Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaU16;
        }

    }
}

/*******************************************************************
Funkcja: void Sensor(void)
Opis funkcji: funkcja glównej detekcji sensora
Data: 14.04.2011
*****************************************************************/
void DaneSensor(unsigned int polaryzacjaU16)
{
    WORD i;
    
    Sensor->OsXYZ[0].analogowySetResetU16[polaryzacjaU16] = 0;
    Sensor->OsXYZ[1].analogowySetResetU16[polaryzacjaU16] = 0;
    Sensor->OsXYZ[2].analogowySetResetU16[polaryzacjaU16] = 0;
    for(i=0; i<4; i++)
    {
        Sensor->OsXYZ[0].analogowySetResetU16[polaryzacjaU16] += OS_X;
        Sensor->OsXYZ[1].analogowySetResetU16[polaryzacjaU16] += OS_Y;
        Sensor->OsXYZ[2].analogowySetResetU16[polaryzacjaU16] += OS_Z;
    }
    //pomiary sa 12 bitowe, 4 razy zmierzone
    Sensor->OsXYZ[0].analogowySetResetU16[polaryzacjaU16] = Sensor->OsXYZ[0].analogowySetResetU16[polaryzacjaU16]>>4;
    Sensor->OsXYZ[1].analogowySetResetU16[polaryzacjaU16] = Sensor->OsXYZ[1].analogowySetResetU16[polaryzacjaU16]>>4;
    Sensor->OsXYZ[2].analogowySetResetU16[polaryzacjaU16] = Sensor->OsXYZ[2].analogowySetResetU16[polaryzacjaU16]>>4;
    
    if(polaryzacjaU16 == 0)
    {
        ObliczWartoscRoznicowa();

        SumaryczneWzbudzenie();

        //sprawdz detekcje z histerez?
        Detekcja();
        //uaktualnij tlo
        UsrednianieTla();

    }
}


/*******************************************************************
Funkcja: static void UsrednianieTla(void)
Opis funkcji: funkcja usredniania tla w trakcie pracy
Data: 15.04.2011
*****************************************************************/
static void UsrednianieTla(void)
{
    unsigned int i, iloscOdczytowU16, czasUczeniaTlaU16;
    int wartoscOdczytuS16, wartoscRoznicowaTempS16;
    static unsigned int oldPomiarTla = 0, czasUsrednianiaTlaU16 = 0, tablicaZliczenU16[ROZDZIELCZOSC_TLA];
    static int tablicaPomiarowS16[ROZDZIELCZOSC_TLA];

    if(Sensor->czasUsrednianiaTlaU16 >= 200)
    {
        czasUczeniaTlaU16 = Sensor->czasUsrednianiaTlaU16 - 200;
    }
    else
    {
        czasUczeniaTlaU16 = 0;
    }
    if(Flagi.pomiarTla && (oldPomiarTla ==0))
    {
        czasUsrednianiaTlaU16 =  czasUczeniaTlaU16;
        Sensor->aktualnaOsU16 = 0;

        for(i=0; i<ROZDZIELCZOSC_TLA; i++)
        {
            tablicaPomiarowS16[i] = 0;
            tablicaZliczenU16[i] = 0;
        }
        oldPomiarTla = 1;
    }
    if(czasUsrednianiaTlaU16 < Sensor->czasUsrednianiaTlaU16)
    {
        wartoscRoznicowaTempS16 = Sensor->OsXYZ[Sensor->aktualnaOsU16].wartoscRoznicowaS16/2;
        for(i=0; i<ROZDZIELCZOSC_TLA; i++)
        {
            //sprawdz czy dany odczyt juz istnieje w bazie
            if(tablicaPomiarowS16[i] == wartoscRoznicowaTempS16)
            {
                //zwieksz ilosc odczytow
                tablicaZliczenU16[i]++;
                break;
            }
            //jezeli nie znaleziono wpisow
            else if(tablicaPomiarowS16[i] == 0)
            {
                //dodaj nowy pomiar do tablicy
                tablicaPomiarowS16[i] = wartoscRoznicowaTempS16;
                break;
            }
        }
        czasUsrednianiaTlaU16++;
        //jezeli tablica sie przepelnila - stworz od poczatku
        if(i == ROZDZIELCZOSC_TLA)
        {
            czasUsrednianiaTlaU16 = 0;
            for(i=0; i<ROZDZIELCZOSC_TLA; i++)
            {
                tablicaPomiarowS16[i] = 0;
                tablicaZliczenU16[i] = 0;
            }
        }
    }
    else
    {
        iloscOdczytowU16 = 0;
        wartoscOdczytuS16 = 0;
        for(i=0; i<ROZDZIELCZOSC_TLA; i++)
        {
            if(tablicaZliczenU16[i] > iloscOdczytowU16)
            {
                wartoscOdczytuS16 = tablicaPomiarowS16[i];
                iloscOdczytowU16 = tablicaZliczenU16[i];
            }
            tablicaPomiarowS16[i] = 0;
            tablicaZliczenU16[i] = 0;
        }
        //dla aplikacji parkingowych - nie usredniaj tla
        if(Sensor->czasUsrednianiaTlaU16 != 0xFFFF)
        {
            Sensor->OsXYZ[Sensor->aktualnaOsU16].poziomTlaS16 = wartoscOdczytuS16<<1;
            Sensor->OsXYZ[Sensor->aktualnaOsU16].aktualneTloS16 = 
                    Sensor->OsXYZ[Sensor->aktualnaOsU16].poziomTlaS16;
        }

        if(Flagi.pomiarTla)
        {
            Sensor->OsXYZ[Sensor->aktualnaOsU16].poziomTlaS16 = wartoscOdczytuS16<<1;
            Sensor->OsXYZ[Sensor->aktualnaOsU16].aktualneTloS16 = 
                    Sensor->OsXYZ[Sensor->aktualnaOsU16].poziomTlaS16;
            czasUsrednianiaTlaU16 = czasUczeniaTlaU16;
        }
        else
        {
            czasUsrednianiaTlaU16 = 0;

        }

        //przerzuc sie na nastepna os
        Sensor->aktualnaOsU16++;
        if(Sensor->aktualnaOsU16 > 2)
        {
            Flagi.pomiarTla = 0;
            oldPomiarTla = 0;
            Sensor->aktualnaOsU16 = 0;
            //po inicjalizacji tla zobacz czy ma byc zapis do FLASH
            if(Flagi.zapisDoFlash)
            {
                ZapisZmiennychDoFLASH();
                Flagi.zapisDoFlash = 0;
                //Reset();
            }
        }
    }

}

/*******************************************************************
Funkcja: static int ObliczRoznice(unsigned int liczba1U16, unsigned int liczba2U16)
Opis funkcji: funkcja detekcji pojazdu
Data: 14.04.2011
*****************************************************************/
static unsigned int ObliczRoznice(int liczba1S16, int liczba2S16)
{
    unsigned int roznicaU16;

    if(liczba1S16 > liczba2S16)
    {
        roznicaU16 = (unsigned int)(liczba1S16 - liczba2S16);
    }
    else
    {
        roznicaU16 = (unsigned int)(liczba2S16 - liczba1S16);
    }

    return roznicaU16;
}	

/*******************************************************************
Funkcja: void StanZgloszenia(void)
Opis funkcji: funkcja zg³oszenia pojazdu
Data: 21.06.2011
*****************************************************************/
void StanZgloszenia(void)
{
    static unsigned int timerWzbudzeniaU16=0, zgloszenieU16 = 0;

    if(Dane->timerRysowaniaWykresuU16 == 0)
    {
        //jezeli poszla ramka stanu bo wzbudzil sie czujnik - resetuj czas raportowania
        if(Flagi.zgloszenie && (zgloszenieU16 == 0))
        {
            zgloszenieU16 = 1;
            timerWzbudzeniaU16 = Sensor->timerWzbudzeniaU16;
        }
        else if(!Flagi.zgloszenie && (zgloszenieU16 == 1))
        {
            zgloszenieU16 = 0;
            timerWzbudzeniaU16 = Sensor->timerWzbudzeniaU16;
        }
    }
    //jezeli uplynal czas raportowania - wyslij ramke stanu
    if(timerWzbudzeniaU16 == 0)
    {
        Flagi.CAN.identyfikatorU16 = 1;
        //wyslij ramke stanu ale nie podczas uczenia tla
        if(!Flagi.pomiarTla)
        {
            Flagi.CAN.wyslijRamkeDanych = 1;
        }
        if(Flagi.zgloszenie)
        {
            RELAY = 1;
        }
        else
        {
            RELAY = 0;
        }
        if(Dane->timerRysowaniaWykresuU16 == 0)
        {
            timerWzbudzeniaU16 = Sensor->timerWzbudzeniaU16;
        }
        else
        {
            Dane->timerRysowaniaWykresuU16--;
            timerWzbudzeniaU16 = 4; //wysylanie ramki co 50ms przy rysowaniu wykresu
        }
    }
    else
    {
        timerWzbudzeniaU16--;
    }
}	

/*******************************************************************
Funkcja: void Zgloszenie(void)
Opis funkcji: funkcja zg³oszenia pojazdu
Data: 15.04.2011
*****************************************************************/
void Zgloszenie(void)
{
    static unsigned int timerZgloszeniaU16;

    if(Flagi.detekcja)
    {
        timerZgloszeniaU16 = Sensor->czasZgloszeniaU16;
//#ifndef PARKINGI_CONF
        if(!Flagi.zgloszenie)
        {
            Flagi.CAN.identyfikatorU16 = 1;
            Flagi.CAN.wyslijRamkeDanych = 1;
        }
        RELAY = 1;
//#endif
        Flagi.zgloszenie = 1;
    }
    else if(timerZgloszeniaU16 > 0)
    {
        timerZgloszeniaU16--;
    }
    else
    {
        if(Flagi.zgloszenie)
        {
            Flagi.CAN.identyfikatorU16 = 1;
            Flagi.CAN.wyslijRamkeDanych = 1;
        }
        Flagi.zgloszenie = 0;
        RELAY = 0;
    }
}				

