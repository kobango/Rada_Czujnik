/* 
 * File:   transmisja.h
 * Author: Pawe�
 *
 * Created on 14 listopad 2013, 12:34
 */

#ifndef TRANSMISJA_H
#define	TRANSMISJA_H

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
        }Flagi;

        WORD adresCAN;

    }DaneCanStruct;
    extern DaneCanStruct DaneCan;

    void TransmisjaDanych(void);

#endif	/* TRANSMISJA_H */

