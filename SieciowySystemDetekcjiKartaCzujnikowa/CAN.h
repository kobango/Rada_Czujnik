#ifndef CAN_H
	#define CAN_H
#include "GenericTypeDefs.h"

    /* ECAN message buffer length */
    #define RX_BUF_SIZE 5

    #define CAN_MSG_DATA 0x01
    #define CAN_MSG_RTR 0x02
    #define CAN_FRAME_EXT 0x03
    #define CAN_FRAME_STD 0x04
    #define CAN_BUF_FULL 0x05
    #define CAN_BUF_EMPTY 0x06

    typedef enum{
        KARTA,
        CZUJNIK
    }TARGET_ENUM;
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

    void CAN_Setup(void);
    BOOL CAN_TakeFrame(mID * message);
    void CAN_SendFrame(mID * message);
    void CAN_GenID(mID * message,BYTE frameID);
#endif
