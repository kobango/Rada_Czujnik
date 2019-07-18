#include "main.h"
#include "DetekcjaSasiadow.h"
#include "sensor.h"

wartosciSasiadaStruct wartosciSasiada[MAX_SASIADOW];


/*******************************************************************
Funkcja: static WORD AktualizacjaHistorii(wartosciSasiadaStruct *pointer)
Opis funkcji: sprawdzenie stanu ustalonego odczytow
Data: 24.11.2016
*****************************************************************/
static void AktualizujTlo(WORD nrSasiadaU16, WORD stanSasiadaU16, historiaStruct *historiaPointer )
{
    WORD j;
    
    //jezeli aktualny stan sasiada jest 0, 
    if(stanSasiadaU16 == 0)
    {
        //cofnij swoja korekte tla
        for(j=0; j<3; j++)
        {
            Sensor->OsXYZ[j].aktualneTloS16 -= wartosciSasiada[nrSasiadaU16].przesuniecieTlaS16[j];
        }            
    }
    else
    {
        for(j=0; j<3; j++)
        {
            //oblicz przesuniecie
            wartosciSasiada[nrSasiadaU16].przesuniecieTlaS16[j] = historiaPointer->aktualnaWartoscSredniaS16 - //Sensor->OsXYZ[j].wartoscRoznicowaS16 - 
                    historiaPointer->poprzedniaWartoscSredniaS16;//Sensor->OsXYZ[j].aktualneTloS16;
            historiaPointer++;
            //uaktualnij poziom tla
            Sensor->OsXYZ[j].aktualneTloS16 += wartosciSasiada[nrSasiadaU16].przesuniecieTlaS16[j];
        }
    }
}

/*******************************************************************
Funkcja: static WORD AktualizacjaHistorii(wartosciSasiadaStruct *pointer)
Opis funkcji: sprawdzenie stanu ustalonego odczytow
Data: 24.11.2016
*****************************************************************/
static WORD AktualizacjaHistorii(historiaStruct *pointer, SHORT aktualnaWartoscS16)
{
    WORD i, minU16=0xFFFF, maxU16=0;
    
    if(pointer->pointerU16 >= Sensor->czasStabilizacjiSasiadaU16)
    {
        pointer->pointerU16 = 0;
    }
    pointer->wartosciHistoryczneS16[pointer->pointerU16++] =
            aktualnaWartoscS16;
    for(i=0; i<Sensor->czasStabilizacjiSasiadaU16; i++)
    {
        if(pointer->wartosciHistoryczneS16[i]<minU16)
        {
            minU16 = pointer->wartosciHistoryczneS16[i];
        }
        if(pointer->wartosciHistoryczneS16[i]>maxU16)
        {
            maxU16 = pointer->wartosciHistoryczneS16[i];
        }
    }
    return (maxU16-minU16);
}

WORD UsrednijSwojeTlo(WORD *wartosciHistoryczneU16)
{
    WORD i; 
    DWORD tempU32;
    
    tempU32 = 0;
    //aktualizuj tlo tylko jezeli jest stabilne
    for(i=0; i<Sensor->czasStabilizacjiSasiadaU16; i++)
    {
        tempU32 += wartosciHistoryczneU16[i];
    }
    return (WORD)(tempU32/i);
}
/*******************************************************************
Funkcja: void AktualizacjaTlaOdSasiadow(void)
Opis funkcji: sprawdzenie wszystkich sasiadow i wprowadzenie odpowiednich zmian
Data: 24.11.2016
*****************************************************************/
void AktualizacjaTlaOdSasiadow(void)
{
    WORD i, j, zmianaU16, brakSasiadowU16=0, mojSkokTlaU16;
    DWORD tempU32;
//    static wartosciSasiadaStruct mojeTlo;
    static historiaStruct mojaHistoria[4];
    

    //aktualizuj historie osi i glowna
    for(j=0; j<3; j++)
    {
        AktualizacjaHistorii(&mojaHistoria[j], 
            Sensor->OsXYZ[j].wartoscRoznicowaS16);
    }
    zmianaU16 = AktualizacjaHistorii(&mojaHistoria[3], Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16);
    
    for(i=0; i<MAX_SASIADOW; i++)
    {
        //sprawdz czy u sasiada nastapila zmiana stanu
        if(wartosciSasiada[i].poprzedniStanU16 != wartosciSasiada[i].aktualnyStanU16)
        {
            break;
        }
    }
    //jezeli nie, uaktualnij swoje tlo
    if(i==MAX_SASIADOW)
    {
        //usredniaj tlo tylko jak tlo sie nie rusza
        if(zmianaU16 < Sensor->roznicaZgloszeniaMinU16)
        {
            for(j=0; j<4; j++)
            {
                mojaHistoria[j].poprzedniaWartoscSredniaS16 = 
                        UsrednijSwojeTlo(&mojaHistoria[j].wartosciHistoryczneS16[0]);
            }
            
        }
    }
    //jezeli ktorys zmienil stan - zapamietaj tlo
    for(i=0; i<MAX_SASIADOW; i++)
    {
        //sprawdz czy u sasiada nastapila zmiana stanu
        if(wartosciSasiada[i].poprzedniStanU16 != wartosciSasiada[i].aktualnyStanU16)
        {
            //jezeli sasiad odjechal - odejmij tlo
            if(wartosciSasiada[i].aktualnyStanU16 == 0)
            {
                AktualizujTlo(i, wartosciSasiada[i].aktualnyStanU16, mojaHistoria);
                wartosciSasiada[i].poprzedniStanU16 = wartosciSasiada[i].aktualnyStanU16;
            }
            //sprawdz czy byla u sasiada wszystko sie uspokoilo
            else if((zmianaU16 = AktualizacjaHistorii(&wartosciSasiada[i].historia,
                    wartosciSasiada[i].aktualnaWartoscU16)) < Sensor->roznicaZgloszeniaMinU16)
            {
                //usrednij odczyty
                for(j=0; j<4; j++)
                {
                    mojaHistoria[j].aktualnaWartoscSredniaS16 = 
                        UsrednijSwojeTlo(&mojaHistoria[j].wartosciHistoryczneS16[0]);
                }
                //oblicz roznice zaburzenia tla
                mojSkokTlaU16 = (mojaHistoria[3].aktualnaWartoscSredniaS16 > mojaHistoria[3].poprzedniaWartoscSredniaS16) ? 
                    mojaHistoria[3].aktualnaWartoscSredniaS16 - mojaHistoria[3].poprzedniaWartoscSredniaS16 :
                    mojaHistoria[3].poprzedniaWartoscSredniaS16 - mojaHistoria[3].aktualnaWartoscSredniaS16;
                
                //jezeli sasiad odjechal - odejmij tlo
                if(wartosciSasiada[i].aktualnyStanU16 == 0)
                {
                    AktualizujTlo(i, wartosciSasiada[i].aktualnyStanU16, mojaHistoria);
                }
                //sprawdz czy u sasiada jest wieksze wzbudzenie niz u nas
                else if(wartosciSasiada[i].aktualnaWartoscU16 > mojSkokTlaU16)
                {
                    AktualizujTlo(i, wartosciSasiada[i].aktualnyStanU16, mojaHistoria);
                }
                wartosciSasiada[i].poprzedniStanU16 = wartosciSasiada[i].aktualnyStanU16;
            }
        }
    }
    //sprawdz czy wszystkie sasiady sa 0
    for(i=0; i<MAX_SASIADOW; i++)
    {
        if(wartosciSasiada[i].aktualnyStanU16)
        {
            break;
        }
    }
    //wtedy przywroc oryginalne tlo
    if(i == MAX_SASIADOW)
    {
        for(j=0; j<3; j++)
        {
            Sensor->OsXYZ[j].aktualneTloS16 = Sensor->OsXYZ[j].poziomTlaS16;
        }
        //wyczysc poprzednie stany
        for(i=0; i<MAX_SASIADOW; i++)
        {
            wartosciSasiada[i].poprzedniStanU16 = 0;
        }
        
    }
}
