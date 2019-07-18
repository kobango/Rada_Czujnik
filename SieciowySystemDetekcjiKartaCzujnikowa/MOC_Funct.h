
#ifndef MOC_H
#define	MOC_H

#include <xc.h> // include processor files - each processor file is guarded.  


UINT MOC_StanWzbudzenia(void);
UINT MOC_Wynikowa_wartosc_roznicowa(void);
UINT MOC_Frame_Counter(void);
UINT MOC_Aktualna_Temperatura(void);
UINT MOC_NOTWORK(void);
UINT MOC_RSSI_ramki(void);
UINT MOC_LQI_ramki(void);
UINT MOCK_PrzyspieszenieX(void);
UINT MOCK_PrzyspieszenieY(void);
UINT MOCK_PrzyspieszenieZ(void);
UINT MOCK_Klikniecie_Spadek(void);
UINT MOCK_Background(void);

UINT MOCK_SoftwareReset(void);
        
#endif	/* MOCK H */

