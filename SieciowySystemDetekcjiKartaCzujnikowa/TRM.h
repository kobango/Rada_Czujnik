/* 
 * File:   transmisja.h
 * Author: Pawe³
 *
 * Created on 14 listopad 2013, 12:34
 */

#ifndef TRANSMITION_H
#define	TRANSMITION_H

    typedef struct
    {
        union
        {
            WORD FlagiU16;

            struct
            {
                unsigned wyslijRamkeStanu : 1;
                unsigned wyslijRamkeUczeniaTla : 1;
                unsigned wyslijRamkeResetuCzujnikow : 1;

            };
        }Flags;

        WORD adresCAN;

    }DaneCanStruct;
    extern DaneCanStruct DaneCan;

    void TRM_DataTransmition(void);

#endif	/* TRANSMISJA_H */

