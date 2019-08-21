/* 
 * File:   EEPROM.h
 * Author: Pawe³
 *
 * Created on 5 lipiec 2013, 09:29
 */

#ifndef EEPROM_H
    #define	EEPROM_H
    #include "main.h"
//    #include "WeWy.h"
    #define TOTAL_NVM_BYTES     (WORD)0x400

    BOOL NVMInit(void);
    void NVMRead(BYTE *dest, WORD addr, WORD count);
    void NVMWrite(BYTE *source, WORD addr, WORD count);
    BYTE ReadfromEEPROM(WORD Address);
    void WritetoEEPROM(WORD Address, BYTE Data);
    UINT EEPROM_Adres_Readfrom(UINT StartEEPROMAdres, UINT MinAdres, UINT MaxAdres);
    void EEPROM_Adres_Writeto(UINT StartEEPROMAdres, UINT AdressOfNeighbor, UINT MinAdres, UINT MaxAdres);
    
    void UstawFlagi(void);

    extern WORD        ustawieniaKarty;
    extern WORD        nazwyPrzyciskow;
    extern WORD        czujnikiNaMapie;

    #define nvmGetUstawieniaKarty(x)    NVMRead((BYTE *)x, ustawieniaKarty, sizeof(KartaStruct))
    #define nvmPutUstawieniaKarty(x)    NVMWrite((BYTE *)x, ustawieniaKarty, sizeof(KartaStruct))
    #define nvmGetNazweCzujnika(x, y)    NVMRead((BYTE *)x, nazwyPrzyciskow+((WORD)y * (WORD)BAJTOW_NAZWY), BAJTOW_NAZWY)
    #define nvmPutNazweCzujnika(x, y)    NVMWrite((BYTE *)x, nazwyPrzyciskow+((WORD)y * (WORD)BAJTOW_NAZWY), BAJTOW_NAZWY)
    #define nvmGetUstawieniaMapyCzujnika(x, y)    NVMRead((BYTE *)x, czujnikiNaMapie+((WORD)y * (WORD)sizeof(CzujnikNaMapieStruct)), sizeof(CzujnikNaMapieStruct))
    #define nvmPutUstawieniaMapyCzujnika(x, y)    NVMWrite((BYTE *)x, czujnikiNaMapie+((WORD)y * (WORD)sizeof(CzujnikNaMapieStruct)), sizeof(CzujnikNaMapieStruct))

#endif	/* EEPROM_H */

