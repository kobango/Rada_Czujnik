
#ifndef MOC H
#define	MOC H

#include <xc.h> // include processor files - each processor file is guarded.  


UINT MOC_StanWzbudzenia(void);
UINT MOC_Wynikowa_wartosc_roznicowa(void);
UINT MOC_Frame_Counter(void);
UINT MOC_Aktualna_Temperatura(void);
UINT MOC_NOTWORK(void);
UINT MOC_RSSI_ramki(void);
UINT MOC_LQI_ramki(void);

#endif	/* MOC H */

