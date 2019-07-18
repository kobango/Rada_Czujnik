#include "main.h"
#include "sensor.h"
#include "flash.h"

#define CM_ROW_SIZE 8
#define CONFIG_WORD_SIZE 1

#define PM_ROW_ERASE 		0x4042
#define PM_ROW_WRITE 		0x4001
#define CONFIG_WORD_WRITE	0X4000

/////////////////////////////////////////////////////////////////
typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;

/////////////////////////////////////////////////////////////////
typedef union tuReg32
{
	UWord32 Val32;

	struct
	{
		UWord16 LW;
		UWord16 HW;
	} Word;

	char Val[4];
} uReg32;

	/////////////////////////////////////////////////////////////////
//#if (sizeof(SensorStruct) > PM_ROW_SIZE)
//#endif

unsigned int daneU16[PM_ROW_SIZE + 1];

const unsigned int __attribute__((address(0x1400), aligned(512)))ustawieniaU16[PM_ROW_SIZE + 1] = {
	0x05, 		//Dane->numerSeryjnyU16 nie uzywane - ustawiane w bootloaderze
	10,		//Dane->startupU16
	Dane->wersjaOprogramowaniaU16, //Dane->wersjaOprogramowaniaU16 (major, minor),
    0,              //Dane->PozycjaNaMapce->xU16
    0,              //Dane->PozycjaNaMapce->yU16
    0,              //Dane->wersjaPlytki (ASCII), wersjaHardware (hex)
    0,              //Dane->timerRysowaniaWykresuU16
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//Dane->sasiedzi - 8szt(strukctura 2*word)
    0,0,0,0,0,0,0,0,//rokU16, miesiacU16, dzienU16, godzinaU16, minutaU16;

    //ustawienia sensora
	1000,           //Sensor->pomiarTlaTimerU16		*10ms
	200,            //Sensor->timerWzbudzeniaU16	*10ms
	10,             //Sensor->czasZgloszeniaU16		*10ms
	0xFFFF,         //Sensor->czasUsrednianiaTlaU16 
    0x0000,         //Sensor->czasWyjsciaZeWzbudzeniaU16  
    10,             //Sensor->czasStabilizacjiSasiadaU16 
	0x40,           //Sensor->roznicaZgloszeniaMinU16
	0x80,           //Sensor->roznicaZgloszeniaMaxU16
        0,              //Sensor->obliczonaWynikowaRoznicaZgloszeniaU16
        0,              //Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16
        0,              //Sensor->aktualnaOsU16

        //os X struktury XYZStruct
        0,              //wartoscRoznicowaS16
        0,              //aktualneTloS16
        0,              //poziomTlaS16
        0,              //obliczonaRoznicaZgloszeniaU16;
        0,              //obliczonaRoznicaZgloszeniaMaxU16;
        1,              //mnoznikU16;
        0,0,              //analogowySetResetU16[2];

        //os Y struktury XYZStruct
        0,              //wartoscRoznicowaS16
        0,              //aktualneTloS16
        0,              //poziomTlaS16
        0,              //obliczonaRoznicaZgloszeniaU16;
        0,              //obliczonaRoznicaZgloszeniaMaxU16;
        1,              //mnoznikU16;
        0,0,              //analogowySetResetU16[2];

        //os Z struktury XYZStruct
        0,              //wartoscRoznicowaS16
        0,              //aktualneTloS16
        0,              //poziomTlaS16
        0,              //obliczonaRoznicaZgloszeniaU16;
        0,              //obliczonaRoznicaZgloszeniaMaxU16;
        1,              //mnoznikU16;
        0,0              //analogowySetResetU16[2];

	};

static void ReadPM(unsigned int * ptrData, uReg32 SourceAddr);
static void WritePM(unsigned int * ptrData, uReg32 SourceAddr);

/*******************************************************************
Funkcja: void ReadPM(char * ptrData, uReg32 SourceAddr)
Opis funkcji: Funkcja odczytuj?ca 1 stron? wew FLASH danymi
Data: 04.10.10
*****************************************************************/
static void ReadPM(unsigned int * ptrData, uReg32 SourceAddr)
{
	int    Size;
	uReg32 Temp;

	for(Size = 0; Size < PM_ROW_SIZE; Size++)
	{
		Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

		*ptrData = Temp.Val[1]&0xFF;
		*ptrData <<= 8;
		*ptrData |= Temp.Val[0]&0xFF;
		ptrData++;

		SourceAddr.Val32 = SourceAddr.Val32 + 2;
	}
}
/*******************************************************************
Funkcja: void WritePM(char * ptrData, uReg32 SourceAddr)
Opis funkcji: Funkcja zapisuj?ca 1 stron? wew FLASH danymi
Data: 07.10.08
*****************************************************************/
static void WritePM(unsigned int * ptrData, uReg32 SourceAddr)
{
	int    Size;
	uReg32 Temp;
	uReg32 TempAddr;
	uReg32 TempData;

	for(Size = 0; Size < PM_ROW_SIZE; Size++)
	{
		
		Temp.Val[0] = (unsigned char)*ptrData;
		Temp.Val[1] = (unsigned char)(*ptrData >> 8);
		Temp.Val[2]=0;
		Temp.Val[3]=0;
		ptrData++;

	   WriteLatch(SourceAddr.Word.HW, SourceAddr.Word.LW,Temp.Word.HW,Temp.Word.LW);

		/* Device ID errata workaround: Save data at any address that has LSB 0x18 */
		if((SourceAddr.Val32 & 0x0000001F) == 0x18)
		{
			TempAddr.Val32 = SourceAddr.Val32;
			TempData.Val32 = Temp.Val32;
		}

		if((Size !=0) && (((Size + 1) % 64) == 0))
		{
			/* Device ID errata workaround: Reload data at address with LSB of 0x18 */
	      WriteLatch(TempAddr.Word.HW, TempAddr.Word.LW,TempData.Word.HW,TempData.Word.LW);

			//WriteMem(PM_ROW_WRITE);
		}

		SourceAddr.Val32 = SourceAddr.Val32 + 2;
	}

   
}

/*******************************************************************
Funkcja: void InicjalizacjaZamiennych(void)
Opis funkcji: Funkcja inicjalizujaca zmienne globalne
Data: 14.04.2011
*****************************************************************/
void InicjalizacjaZmiennych(void)
{
	uReg32 SourceAddr;

	//ustaw adres w wew FLASH
	SourceAddr.Val[0] = 0;
	SourceAddr.Val[1] = 0x14;
	SourceAddr.Val[2] = 0;
	SourceAddr.Val[3] = 0;
	
	ReadPM(daneU16, SourceAddr);
        //numer seryjny
        SourceAddr.Val32 = 0x400;
        daneU16[0] = (unsigned int)ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);
        //wersja sprzetu
        SourceAddr.Val32 = 0x402;
        daneU16[5] = (unsigned int)ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);
}
/*******************************************************************
Funkcja: void ZapisZmiennychDoFLASH(void)
Opis funkcji: Funkcja zapisujaca zmienne globalne do FLASH
Data: 29.06.2011
*****************************************************************/
void ZapisZmiennychDoFLASH(void)
{
	uReg32 SourceAddr;

	//ustaw adres w wew FLASH
	SourceAddr.Val[0] = 0;
	SourceAddr.Val[1] = 0x14;
	SourceAddr.Val[2] = 0;
	SourceAddr.Val[3] = 0;
	
	ClrWdt();
	//wyczysc FLASH
	Erase(SourceAddr.Word.HW,SourceAddr.Word.LW,PM_ROW_ERASE);
	
	ClrWdt();
	//zapisz nowe ustawienia
	WritePM(daneU16, SourceAddr);
        Flagi.wykonanoZapisDoFlash = 1;

}
