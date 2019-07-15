

#ifndef TRM1_H
#define	TRM1_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "GenericTypeDefs.h"

UINT8 INI_Timer(void);
UINT8 TMR1_Timer_reset(void);
void TMR1_Update_flag_Set(UINT a);
UINT TMR1_Update_flag_Get(void);


#endif	/* Timer1_H */

