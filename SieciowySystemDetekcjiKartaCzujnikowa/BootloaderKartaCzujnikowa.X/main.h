/*
 * File:   main.h
 * Author: Pawel
 *
 * Created on 1 kwiecie? 2012, 14:21
 */

#ifndef main_H
#define main_H

    #include <p18f26k80.h>
#include "GenericTypeDefs.h"

    #define FCY 16000000//8000000/2
#define BITRATE 50000
    #define NTQ 20 //20 time quanta in a bit time
    #define BRP_VAL ((FCY/(2*NTQ*BITRATE))-1)
	/* ECAN message type identifiers */
	#define CAN_MSG_DATA 0x01
	#define CAN_MSG_RTR 0x02
	#define CAN_FRAME_EXT 0x03
	#define CAN_FRAME_STD 0x04
	#define CAN_BUF_FULL 0x05
	#define CAN_BUF_EMPTY 0x06


        /* ECAN message buffer length */
        #define ECAN1_MSG_BUF_LENGTH 4
//        typedef unsigned int ECAN1MSGBUF[ECAN1_MSG_BUF_LENGTH][8];

	/* message structure in RAM */
	typedef struct{
		/* Keep track of the buffer status */
		unsigned char buffer_status;
		/* RTR message or data message */
		unsigned char message_type;
		/* Frame type extended or standard */
		unsigned char frame_type;
		/* Buffer being used to send and receive messages */
		unsigned char buffer;
		/* 29 bit ID max of 0x1FFF FFFF */
		/* 11 bit ID max of 0x7FF */
		DWORD_VAL id;
		unsigned char data[8];
		unsigned char data_length;
	}mID;

#endif


