#ifndef FLASH_H
	#define FLASH_H

	#define PM_ROW_SIZE 64 * 8
#include "sensor.h"
	extern unsigned int daneU16[PM_ROW_SIZE + 1];
	
	void InicjalizacjaZmiennych(void);
	void ZapisZmiennychDoFLASH(void);

        void Erase(unsigned short HW, unsigned short LW, unsigned short comand);
	unsigned long ReadLatch(unsigned short addrhi, unsigned short addrlo);
	void WriteLatch(unsigned short addrhi1, unsigned short addrlo1,unsigned short addrhi2,unsigned short addrlo2);

	/////////////////////////////////////////////////////////////////
#endif
