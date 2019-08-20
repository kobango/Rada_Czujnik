#include "CAN.h"
#include "main.h"
#include "LED.h"
#include "FRAME.h"
#include "MOC_Funct.h"
#include "TRM.h"
#include "EEPROM.h"
#include "FLASH.h"

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;

typedef union tuReg32 {
  UWord32 Val32;

  struct {
    UWord16 LW;
    UWord16 HW;
  } Word;

  char Val[4];
} uReg32;

mID ramkaCanRxKarty[RX_BUF_SIZE], ramkaCanTxKarty;
UINT IsInNeighbors(UINT message_adress);
void ReadDataToEEPROM(void);
void WriteDataToEEPROM(void);

volatile UINT MinAdres = 0;
volatile UINT MaxAdres = 0xFFFF;

volatile UINT Init_Data = 0xFFFF;
volatile UINT NeightAdress1 = 0;
volatile UINT NeightAdress2 = 298;
volatile UINT NeightAdress3 = 0;
volatile UINT NeightAdress4 = 0;
volatile UINT NeightAdress5 = 0;
volatile UINT NeightAdress6 = 0;
volatile UINT NeightAdress7 = 0;
volatile UINT NeightAdress8 = 0;

volatile UINT MinRange = 0;
volatile UINT MaxRange = 0xFFFF;
volatile UINT MinPower = 0;
volatile UINT MaxPower = 0xFFFF;





/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame who given status of sensor into message.
*****************************************************************************************/
static void FRAME_SensorExcitationStatus(mID *message) // id = 0x01
{
    
    static UINT statwect = 0b00000000;
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 6;
        //message->data[0] = Flagi.zgloszenie;
        //message->data[1] = (unsigned char)(Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16 >> 8);
        //message->data[2] = (unsigned char)Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16;
        //message->data[3] = Flagi.frameCounterU8++;
        //message->data[4] = (unsigned char)Thermistor->temperaturaS8;
        message->data[0] = LOCK_Get();
        message->data[1] = MOC_Wynikowa_wartosc_roznicowa() >> 8 ;
        message->data[2] = MOC_Wynikowa_wartosc_roznicowa() & 0x00FF;
        message->data[3] = MOC_Frame_Counter();
        message->data[4] = MOC_Aktualna_Temperatura();
        message->data[5] = MOC_NOTWORK();
        //message->data[6] = MOC_RSSI_ramki();
        //message->data[7] = MOC_LQI_ramki();
        //message->data[0] = 0x00;// MOC_StanWzbudzenia();
        //message->data[1] = 0x88;//MOC_Wynikowa_wartosc_roznicowa() & 0x00FF;
        //message->data[2] = 0x88;//MOC_Wynikowa_wartosc_roznicowa()>>8;
        //message->data[3] = 0x00;//MOC_Frame_Counter();
        //message->data[4] = 0x00;
        //wyczysc maksymalna wartosc odczytu
        //Sensor->obliczonaWynikowaRoznicaZgloszeniaMaxU16 = 0;
    }
    else
    {
        
        UINT tru = IsInNeighbors(message->id.w[0]);
       
        
        if(message->data[0]==0)
        {
          
            statwect = statwect &( ~tru );
        }
        else
        {
            statwect = statwect| tru;
        }
        
        if(statwect>0)
        {
            if(1 != LOCK_Get())
            {
                DaneCan.Flags.wyslijRamkeStanu = 1;
                LOCK_Set(1);
            }
        
        }
        else
        {
        
            if(1 == LOCK_Get())
            {
                DaneCan.Flags.wyslijRamkeStanu = 1;
                LOCK_Set(0);
            }
        }
    }
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
* @param message_adress adress ID of message
*
*
* @section Description
* Function uses to create wector with position of sending device in Neighbors.
* When sending adress is't in Neighbors send wector of 0.  Theoretically it is not possible but used as an additional security
*****************************************************************************************/

UINT IsInNeighbors(UINT message_adress)
{
    WORD i;
       
    if(NeightAdress1==message_adress)
    {
        return 0b1;
    }
    if(NeightAdress2==message_adress)
    {
        return 0b10;
    }
    if(NeightAdress3==message_adress)
    {
        return 0b100;
    }
    if(NeightAdress4==message_adress)
    {
        return 0b1000;
    }
    if(NeightAdress5==message_adress)
    {
        return 0b10000;
    }
    if(NeightAdress6==message_adress)
    {
        return 0b100000;
    }
    if(NeightAdress7==message_adress)
    {
        return 0b1000000;
    }
    if(NeightAdress8==message_adress)
    {
        return 0b10000000;
    }
    
    //0x10
    /*
    for(i=0; i<8; i++)
        {
            if(message_adress == NeightAdress[i] )
            {
                return 1;
                
            }
            //zapisz adres struktury
            
        } 
    */
    return 0;
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame who given MOCK status of Accelerometer who is not instaled. 
*****************************************************************************************/
static void FRAME_AccelerometerStatus(mID *message) // id = 0x02
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 4;
        //message->data[0] = Accelerometer->przyspieszenieXU8;
        //message->data[1] = Accelerometer->przyspieszenieYU8;
        //message->data[2] = Accelerometer->przyspieszenieZU8;
        message->data[0] = MinRange/0x100;
        message->data[1] = MinRange & 0xFF;
        message->data[2] = MaxRange/0x100;
        message->data[3] = MaxRange & 0xFF;
    }
    else
    {
        MinRange = ((message->data[0])*0x100) | ((message->data[1]) % 0xFF);
        MaxRange = ((message->data[2])*0x100) | ((message->data[3]) % 0xFF);
    }
}


/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame who shoud given ExcitationValue, actually not in use.
*****************************************************************************************/
static void FRAME_ExcitationValue(mID *message) // id = 0x03
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 6;
        //message->data[0] = (unsigned char)(Sensor->roznicaZgloszeniaMinU16 >> 8);
        //message->data[1] = (unsigned char)Sensor->roznicaZgloszeniaMinU16;
        //message->data[2] = (unsigned char)(Sensor->roznicaZgloszeniaMaxU16 >> 8);
        //message->data[3] = (unsigned char)Sensor->roznicaZgloszeniaMaxU16;
        message->data[0] = MinPower/0x100;
        message->data[1] = MinPower & 0xFF;
        message->data[2] = MaxPower/0x100;
        message->data[3] = MaxPower & 0xFF;
        message->data[4] = 0;
        message->data[5] = 0;
    }
    else
    {
        //Sensor->roznicaZgloszeniaMinU16 = (unsigned int)message->data[0]<<8 | (unsigned int)message->data[1];
        //Sensor->roznicaZgloszeniaMaxU16 = (unsigned int)message->data[2]<<8 | (unsigned int)message->data[3];
        MinPower = ((message->data[0])*0x100) | ((message->data[1]) % 0xFF);
        MaxPower = ((message->data[2])*0x100) | ((message->data[3]) % 0xFF);
    }
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame who shoud given Excitation Multiplier of sensor, actually not in use.
*****************************************************************************************/
static void FRAME_ExcitationMultiplier(mID *message) // id = 0x04
{
    WORD i;
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        for(i=0; i<3; i++)
        {
            //message->data[2*i] = (unsigned char)(Sensor->OsXYZ[i].mnoznikU16 >> 8);
            //message->data[(2*i)+1] = (unsigned char)Sensor->OsXYZ[i].mnoznikU16;
        }
        message->data[0] = 0xFF;
    }
    else
    {
        //Sensor->OsXYZ[0].mnoznikU16 = (unsigned int)message->data[0]<<8 | (unsigned int)message->data[1];
        //Sensor->OsXYZ[1].mnoznikU16 = (unsigned int)message->data[2]<<8 | (unsigned int)message->data[3];
        //Sensor->OsXYZ[2].mnoznikU16 = (unsigned int)message->data[4]<<8 | (unsigned int)message->data[5];
    }
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame who shoud given Averaging Times of sensors, actually not in use.
*****************************************************************************************/
static void FRAME_AveragingTimes(mID *message) // id = 0x05
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        //message->data[0] = (unsigned char)(Sensor->czasUsrednianiaTlaU16 >> 8);
        //message->data[1] = (unsigned char)Sensor->czasUsrednianiaTlaU16;
        //message->data[2] = (unsigned char) (Sensor->czasZgloszeniaU16);
        //message->data[3] = (unsigned char) (Sensor->czujnikZliczajacy);
        //message->data[4] = (unsigned char) (Sensor->czasWyjsciaZeWzbudzeniaU16>>8);
        //message->data[5] = (unsigned char) (Sensor->czasWyjsciaZeWzbudzeniaU16);
        //message->data[6] = (unsigned char) (Sensor->czasStabilizacjiSasiadaU16);
        message->data[0] = 0xFF;
    }
    else
    {
        /*
        Sensor->czasUsrednianiaTlaU16 = (unsigned int)message->data[0]<<8 | (unsigned int)message->data[1];
        Sensor->czasZgloszeniaU16 = (unsigned int)message->data[2];
        if((message->data[3] & 0x01) == 1)
        {
            Sensor->czujnikZliczajacy = 1;
        }
        else
        {
            Sensor->czujnikZliczajacy = 0;
        }
        if(message->data_length > 4)
        {
            Sensor->czasWyjsciaZeWzbudzeniaU16 = (unsigned int)message->data[4]<<8 | (unsigned int)message->data[5];
        }
        else
        {
            Sensor->czasWyjsciaZeWzbudzeniaU16 = 0;
        }
        if(message->data_length > 6)
        {
            if(message->data[6] < HISTORIA_SASIADA_SIZE)
            {
                Sensor->czasStabilizacjiSasiadaU16 = (unsigned int)message->data[6];
            }
            else
            {
                Sensor->czasStabilizacjiSasiadaU16 = HISTORIA_SASIADA_SIZE;
            }
        }
        else 
        {
            Sensor->czasWyjsciaZeWzbudzeniaU16 = 0;
        }*/
    }
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame who shoud given AxisStatus, actually not in use.
*****************************************************************************************/
static void FRAME_AxisStatus(mID *message) //0x06
{
    WORD i;
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 1;
        /*
        message->data[0] = Flagi.zgloszenie;
        for(i=0; i<3; i++)
        {
            message->data[(2*i)+1] = (unsigned char)(Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16 >> 8);
            message->data[(2*i)+2] = (unsigned char)Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16;
            //wyczysc maksymalne stany odczytow
            Sensor->OsXYZ[i].obliczonaRoznicaZgloszeniaMaxU16 = 0;
        }
         */
        message->data[0] = 0xFF;
    }
    else
    {

    }
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Data of Last Change of device.
*****************************************************************************************/
static void FRAME_DataUpdateAndChangeOption(mID *message)
{
    
    Dane->rokU16 = message->data[4];
    Dane->miesiacU16 = message->data[5];
    Dane->dzienU16 = message->data[6];
    Dane->godzinaU16 = message->data[7];
    
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame of device Reset. 
*****************************************************************************************/
static void FRAME_DeviceReset(mID *message) //0x07
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 8;
        
        message->data[0] = (RCON>>6) & 0x01; //stan software reset
        message->data[1] = Flagi.wykonanoZapisDoFlash;
        message->data[2] = Flagi.pomiarTla;
        message->data[3] = 0;
        message->data[4] = Dane->rokU16;
        message->data[5] = Dane->miesiacU16;
        message->data[6] = Dane->dzienU16;
        message->data[7] = Dane->godzinaU16;
        RCON &= ~(1<<6); //kasowanie software reset po odczycie ramki
        Flagi.wykonanoZapisDoFlash = 0;
        
        
        //message->data[0] = MOCK_SoftwareReset();//stan software reset
        //message->data[1] = Flags.wykonanoZapisDoFlash;
        //message->data[2] = MOCK_Background();
        //message->data[3] = 0;
        //message->data[4] = DaneCan->rokU16;
        //message->data[5] = DaneCan->miesiacU16;
        //message->data[6] = DaneCan->dzienU16;
        //message->data[7] = DaneCan->godzinaU16; 
        //RCON &= ~(1<<6); //kasowanie software reset po odczycie ramki
        //Flags.wykonanoZapisDoFlash = 0;
        
    }
    else
    {
        message->data_length = 1;
        
        if(message->data[2])
        {
            Flagi.pomiarTla = 1;
            if(message->data[1])
            {
                Flagi.zapisDoFlash = 1;
                WriteDataToEEPROM();
            }
        }
        else
        {
            if(message->data[1])
            {
                FRAME_DataUpdateAndChangeOption(message);
                WriteDataToEEPROM();
                //ZapisZmiennychDoFLASH();
            }
            if(message->data[0])
            {
                Reset();
            }
        }
        message->data[0] = 0xFF;
    }
}


/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
*
* @section Description
* Uses to read important data from EEPROM. Actualy import adress of neighbors to program global data.
*****************************************************************************************/
void ReadDataToEEPROM(void)
{
    
    NVMRead(&Init_Data,110,2); // Uses to init EEPROM
    NVMRead(&NeightAdress1,113,2);
    if(NeightAdress1==0xFFFF || (MinAdres>NeightAdress1>MaxAdres)) // Void value in EEPROM is 0xFF
    {
        NeightAdress1=0x0000;
    }
    NVMRead(&NeightAdress2,116,2);
    if(NeightAdress2==0xFFFF || (MinAdres>NeightAdress2>MaxAdres))
    {
        NeightAdress2=0x0000;
    }
    NVMRead(&NeightAdress3,119,2);
    if(NeightAdress3==0xFFFF || (MinAdres>NeightAdress3>MaxAdres))
    {
        NeightAdress3=0x0000;
    }
    NVMRead(&NeightAdress4,122,2);
    if(NeightAdress4==0xFFFF || (MinAdres>NeightAdress4>MaxAdres))
    {
        NeightAdress4=0x0000;
    }
    NVMRead(&NeightAdress5,125,2);
    if(NeightAdress5==0xFFFF || (MinAdres>NeightAdress5>MaxAdres))
    {
        NeightAdress5=0x0000;
    }
    NVMRead(&NeightAdress6,128,2);
    if(NeightAdress6==0xFFFF || (MinAdres>NeightAdress6>MaxAdres))
    {
        NeightAdress6=0x0000;
    }
    NVMRead(&NeightAdress7,131,2);
    if(NeightAdress7==0xFFFF || (MinAdres>NeightAdress7>MaxAdres))
    {
        NeightAdress7=0x0000;
    }
    NVMRead(&NeightAdress8,134,2);
    if(NeightAdress8==0xFFFF || (MinAdres>NeightAdress8>MaxAdres))
    {
        NeightAdress8=0x0000;
    }
    
    MinRange = ReadfromEEPROM((WORD)1400);
    MaxRange = ReadfromEEPROM((WORD)1440);
    MinPower = ReadfromEEPROM((WORD)1480);
    MaxPower = ReadfromEEPROM((WORD)1520);
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
*
* @section Description
* Uses to Write important data from EEPROM. Actualy export adress of neighbors to program global data.
*****************************************************************************************/

void WriteDataToEEPROM(void)
{
    NVMWrite(&Init_Data,110,2); // Uses to init EEPROM
    if(MinAdres>(UINT)NeightAdress1>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress1=0x0000;
    }
    NVMWrite(&NeightAdress1,113,2);
    if(MinAdres>(UINT)NeightAdress2>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress2=0x0000;
    }
    NVMWrite(&NeightAdress2,116,2);
    if(MinAdres>(UINT)NeightAdress3>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress3=0x0000;
    }
    NVMWrite(&NeightAdress3,119,2);
    if(MinAdres>(UINT)NeightAdress4>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress4=0x0000;
    }
    NVMWrite(&NeightAdress4,122,2);
    if(MinAdres>(UINT)NeightAdress5>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress5=0x0000;
    }
    NVMWrite(&NeightAdress5,125,2);
    if(MinAdres>(UINT)NeightAdress6>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress6=0x0000;
    }
    NVMWrite(&NeightAdress6,128,2);
    if(MinAdres>(UINT)NeightAdress7>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress7=0x0000;
    }
    NVMWrite(&NeightAdress7,131,2);
    if(MinAdres>(UINT)NeightAdress8>MaxAdres) // Void value in EEPROM is 0xFF
    {
        NeightAdress8=0x0000;
    }
    NVMWrite(&NeightAdress8,134,2);
    
    WritetoEEPROM(1400,MinRange & 0xFF);
    WritetoEEPROM(1401,MinRange >> 8);
    WritetoEEPROM(1440,MaxRange & 0xFF);
    WritetoEEPROM(1441,MaxRange >> 8);
    WritetoEEPROM(1480,MinPower);
    WritetoEEPROM(1520,MaxPower);
    
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame to plot status. Not in use. 
*****************************************************************************************/
static void FRAME_Plot(mID *message) //id = 0x09
{
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data[0] = (BYTE)(Dane->timerRysowaniaWykresuU16/10);
    }
    else
    {
        Dane->timerRysowaniaWykresuU16 = (WORD)message->data[0]*10;
    }
     
    
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame to mark position on map. Not in use. 
*****************************************************************************************/
static void FRAME_MapPosition(mID *message) //id = 0x0A
{ /*
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 4;
        message->data[0] = (unsigned char)(Dane->PozycjaNaMapce.xU16 >> 8);
        message->data[1] = (unsigned char)Dane->PozycjaNaMapce.xU16;
        message->data[2] = (unsigned char)(Dane->PozycjaNaMapce.yU16 >> 8);
        message->data[3] = (unsigned char)Dane->PozycjaNaMapce.yU16;
    }
    else
    {
        Dane->PozycjaNaMapce.xU16 = (unsigned int)message->data[0] << 8
                | (unsigned int)message->data[1];
        Dane->PozycjaNaMapce.yU16 = (unsigned int)message->data[2] << 8
                | (unsigned int)message->data[3];
    } */
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame of Software Version, program in Version 0.
*****************************************************************************************/
static void FRAME_SoftwareVersion(mID *message) //id = 0x0B
{
    
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 4;
        message->data[0] = (unsigned char)(Dane->wersjaOprogramowaniaU16 >> 8);
        message->data[1] = (unsigned char)Dane->wersjaOprogramowaniaU16;
        message->data[2] = (unsigned char)(Dane->wersjaSprzetuU16 >> 8);
        message->data[3] = (unsigned char)Dane->wersjaSprzetuU16;
    }
    else
    {

    } 
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame of Analog Value, not in use.
*****************************************************************************************/
static void FRAME_AnalogValue(mID *message, WORD set) //id = 0x0B
{
    WORD i;
    /*
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 6;
        for(i=0; i<3; i++)
        {
            message->data[2*i] = (unsigned char)(Sensor->OsXYZ[i].analogowySetResetU16[set] >> 8);
            message->data[(2*i)+1] = (unsigned char)Sensor->OsXYZ[i].analogowySetResetU16[set];
        }
    }
    else
    {

    }
    */
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame to .assignment to the card.
*****************************************************************************************/
static void FRAME_PrzypisanieDokarty(mID *message) // 0x0F
{
    if(message->message_type == CAN_MSG_RTR)
    {
        message->data_length = 3;
        message->data[0] = (unsigned char)Dane->NrKarty >> 8 ;
        message->data[1] = (unsigned char)Dane->NrKarty ;
        message->data[2] = (unsigned char)Dane->Nr_WeWy ;
        
    }
    else
    {
         Dane->NrKarty = (message->data[0] << 8) | (message->data[1]);
         Dane->Nr_WeWy = (message->data[2]);
    }
}
/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Frame ho menage adress of neighbors.
*****************************************************************************************/
static void FRAME_AdressOfNeighbors(mID *message, WORD nrRamki)  //0x10 
{
    WORD it;
    
    // Most important 
    WORD kier = (nrRamki-0x10);
    if(message->message_type == CAN_MSG_RTR)
    {
        
        message->data_length = 8;
        
        if(kier == 0)
        {
        message->data[0] = (NeightAdress1>> 8);
        message->data[1] = NeightAdress1 & 0b11111111;
        message->data[2] = (NeightAdress2>> 8);
        message->data[3] = NeightAdress2 & 0b11111111;
        message->data[4] = (NeightAdress3>> 8);
        message->data[5] = NeightAdress3 & 0b11111111;
        message->data[6] = (NeightAdress4>> 8);
        message->data[7] = NeightAdress4 & 0b11111111;
        }
        else
        {
        message->data[0] = (NeightAdress5>> 8);
        message->data[1] = NeightAdress5 & 0b11111111;
        message->data[2] = (NeightAdress6>> 8);
        message->data[3] = NeightAdress6 & 0b11111111;
        message->data[4] = (NeightAdress7>> 8);
        message->data[5] = NeightAdress7 & 0b11111111;
        message->data[6] = (NeightAdress8>> 8);
        message->data[7] = NeightAdress8 & 0b11111111;    
        }
        
       
        
    }
    else
    {
        
         
        if(kier == 0)
        {
         NeightAdress1 = (message->data[0] << 8)| message->data[1];
         NeightAdress2 = (message->data[2] << 8)| message->data[3];
         NeightAdress3 = (message->data[4] << 8)| message->data[5];
         NeightAdress4 = (message->data[6] << 8)| message->data[7];
         CAN_SetupFilter_Ne();
         
              
        }
        else
        {
         NeightAdress5 = (message->data[0] << 8)| message->data[1];
         NeightAdress6 = (message->data[2] << 8)| message->data[3];
         NeightAdress7 = (message->data[4] << 8)| message->data[5];
         NeightAdress8 = (message->data[6] << 8)| message->data[7];
         CAN_SetupFilter_Ne();
         
        }
         /*
        for(i=0; i<4; i++)
        {
            NeightAdress[i+(4*kier)] =  ((WORD)message->data[2*i]) |
                    (WORD)message->data[(2*i)+1];
            //zapisz adres struktury
            
        } 
          * */
         /*
         for(iterator_alfa=0; iterator_alfa<4;iterator_alfa++)
         {
             if(iterator_alfa>3)
             {
                 
             }
             else
             {
                 NeightAdress[(iterator_alfa+(4*kier))] =  (message->data[2*iterator_alfa] << 8)| message->data[((2*iterator_alfa)+1)];
         
             }
             
         }
          * */
         
         
                 
    } 
}

/***************************************************************************************/
/**
* @author Pawel Kasperek
* @date 14/03/2014
*
* @param *message  pointer on message 
*
*
* @section Description
* Function to handle CAN frame, menage all frames.
*****************************************************************************************/
void FRAME_HandleCanFrame(mID * message)
{
    BYTE identyfikator = (BYTE) message->id.v[2]/4;
   
    
    if(identyfikator != 0x01)
    {
        WORD ident = identyfikator;
        int alfa = ident;
    }
    
    switch(identyfikator)
    {
        case 0x01:
            FRAME_SensorExcitationStatus(message);
            break;
        case 0x02:
            FRAME_AccelerometerStatus(message);
            break;
        case 0x03:
            FRAME_ExcitationValue(message);
            break;
        case 0x04:
            FRAME_ExcitationMultiplier(message);
            break;
        case 0x05:
            FRAME_AveragingTimes(message);
            break;
        case 0x06:
            FRAME_AxisStatus(message);
            break;
        case 0x07:
            FRAME_DeviceReset(message);
            break;
        case 0x09:
            FRAME_Plot(message);
            break;
        case 0x0A:
            FRAME_MapPosition(message);
            break;
        case 0x0B:
            FRAME_SoftwareVersion(message);
            break;
        case 0x0C:
        case 0x0D:
            FRAME_AnalogValue(message, identyfikator - 0x0C);
            break;
        case 0x0F:
            FRAME_PrzypisanieDokarty(message);
            break;
        case 0x10:
            FRAME_AdressOfNeighbors(message,0x10);
            break;
        case 0x11:
            FRAME_AdressOfNeighbors(message,0x11);
            break;
         
            /*
        default:
          FRAME_AdressOfNeighbors(message, identyfikator - 0x10);

            break;
             * */
    }
    if(message->message_type == CAN_MSG_RTR)
    {
        
        message->frame_type = CAN_FRAME_EXT;
        message->message_type = CAN_MSG_DATA;
        message->id.Val = 0;
        
        message->id.v[2] = identyfikator*4;
        CAN_GenID(message,identyfikator);
        CAN_SendFrame(message);
        //poczekaj z wysylaniem ramki az sie skonczy probkowanie
        /*
        while(SENSOR == 0)
        {
            ;
        }
         * */
//        TXB0CONbits.TXREQ = 1;  
        
        
       while(RXB0CONbits.FILHIT3) // Poczekaj a? wysle wiadomo?c
       {
           if(TXB0CONbits.TXERR == 1){ // Je?li b??d wyswietl 
               LED_Error();
           }
           else
           {
               //LED_Clear();
           }    
           
       };
        
        //bits.TXREQ0 = 1;
         
    }
}
