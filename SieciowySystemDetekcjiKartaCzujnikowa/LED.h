
 
#ifndef LED_H
#define	LED_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "GenericTypeDefs.h"




void INI_LED_Start(void);
UINT8 LED_Update(void);
void Fulfillment_Lvl_Set(UINT a);
UINT Fulfillment_Lvl_Get(void);
UINT LED_Error(void);
UINT LED_Clear(void);

void LOCK_Set(BYTE k);
BYTE LOCK_Get(void);

#endif	/* LED_H */

