#include "main.h"
#include "sensor.h"
#include "CAN.h"

#define OS_X BufferA[(4*i)+1]
#define OS_Y BufferA[(4*i)+2]
#define OS_Z BufferA[(4*i)+3]

SensorStruct *Sensor;

//static void UsrednianieTla(void);
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
Funkcja: static void UsrednianieTla(void)
Opis funkcji: funkcja usredniania tla w trakcie pracy
Data: 15.04.2011
*****************************************************************/
/*
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
*/
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



