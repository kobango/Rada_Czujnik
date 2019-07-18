/* 
 * File:   DetekcjaSasiadow.h
 * Author: Pawe?
 *
 * Created on 24 listopada 2016, 12:22
 */

#ifndef DETEKCJASASIADOW_H
#define	DETEKCJASASIADOW_H

    #define MAX_SASIADOW 8
    #define HISTORIA_SASIADA_SIZE 25


    typedef struct{
        SHORT pointerU16;
        SHORT aktualnaWartoscSredniaS16, poprzedniaWartoscSredniaS16;
        SHORT wartosciHistoryczneS16[HISTORIA_SASIADA_SIZE];
    }historiaStruct;    

    typedef struct{
        WORD aktualnaWartoscU16, aktualnyStanU16, poprzedniStanU16;
        SHORT przesuniecieTlaS16[3];
        historiaStruct historia;
    }wartosciSasiadaStruct;

    typedef struct{
        WORD adres;
        wartosciSasiadaStruct *pointerNaSasiada;
    }sasiadStruct;



    void AktualizacjaTlaOdSasiadow(void);

    
    extern wartosciSasiadaStruct wartosciSasiada[MAX_SASIADOW];

#ifdef	__cplusplus
}
#endif

#endif	/* DETEKCJASASIADOW_H */

