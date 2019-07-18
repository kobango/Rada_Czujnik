#ifndef SENSOR_H
	#define SENSOR_H
        #define ROZDZIELCZOSC_TLA 1024 //512

        typedef struct
        {
            int wartoscRoznicowaS16, aktualneTloS16, poziomTlaS16;
            unsigned int obliczonaRoznicaZgloszeniaU16;
            unsigned int obliczonaRoznicaZgloszeniaMaxU16;
            unsigned int mnoznikU16;
            unsigned int analogowySetResetU16[2];
        }XYZStruct;

        typedef struct
	{
            unsigned int pomiarTlaTimerU16;
            unsigned int timerWzbudzeniaU16;
            unsigned int czasZgloszeniaU16;
            unsigned int czasUsrednianiaTlaU16;
            unsigned int czasWyjsciaZeWzbudzeniaU16;
            unsigned int czasStabilizacjiSasiadaU16;
            unsigned int roznicaZgloszeniaMinU16, roznicaZgloszeniaMaxU16;
            unsigned int obliczonaWynikowaRoznicaZgloszeniaU16, obliczonaWynikowaRoznicaZgloszeniaMaxU16;
            unsigned int aktualnaOsU16;
            XYZStruct OsXYZ[3];
            unsigned czujnikZliczajacy : 1;
	}SensorStruct;

	extern SensorStruct *Sensor;

	void DaneSensor(unsigned int polaryzacjaU16);
	void Zgloszenie(void);
	void StanZgloszenia(void);
#endif
