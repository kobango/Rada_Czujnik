/*
 * File:   LED.c
 * Author: hp
 *
 * Created on July 8, 2019, 3:40 PM
 */
#define INIT_WEKTORA_POZYCJI 0b0000000001  /**< Position Start wector */


#include <xc.h>
#include "LED.h"
#include "GenericTypeDefs.h"

typedef enum LED_RGA {RED = 0,GREEN = 1}LED_RGA_type;  /**< ENUM type for LED color contain */
 
UINT8 LED_Control(LED_RGA_type color, UINT diode);
void INI_LED_Start(void);
UINT LED_Vect_Create(void);
UINT LED_Right(UINT a);
UINT LED_Left(UINT a);
UINT LED_Light_Pos(LED_RGA_type color,UINT pos, UINT fulfillment);
UINT8 LED_Update(void);
UINT LED_Error(void);
UINT LED_Clear(void);

void Fulfillment_Lvl_Set(UINT a);
UINT Fulfillment_Lvl_Get(void);

void LOCK_Set(BYTE k);
BYTE LOCK_Get(void);



static UINT Fulfillment_Lvl = 20;  /**< Percentage of glowing LEDs with one color.*/
static BYTE LOCK = 0;    // IS Target lock? 0 or 1

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
* @return 1 when function done
*
*
* @section Description
* Make 5 red diodes light on. Usses for inform user. Actually uses to inform while CAN can conection not work. 
*****************************************************************************************/

UINT LED_Error(void)
{
    LED_Control(RED,0b1010101010);
    LED_Control(GREEN,0b0000000000);
    return 1;
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
* @return 1 when function done
*
*
* @section Description
* Make 5 green diodes light on. Usses for inform user. Actually not in use but helpfull to debug. 
*****************************************************************************************/
UINT LED_Clear(void)
{
    LED_Control(RED,0b0000000000);
    LED_Control(GREEN,0b1010101010);
    return 1;
}
/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
* @param k  Status of detection. (1 or 0). 
*
*
* @section Description
* Flag setter function for get data of status, function control color of led. Red if 1 and green if set to 0.
*****************************************************************************************/
void LOCK_Set(BYTE k)
{
    LOCK = k;
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 30/07/2019
*
* @return  Status of detection. (1 or 0)
*
*
* @section Description
* Flag getter function for get data of status, function control color of led. Red if 1 and green if set to 0.
*****************************************************************************************/
BYTE LOCK_Get(void)
{
    return LOCK;
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @param a  Percentage of glowing LEDs with one color. 
*
*
* @section Description
* Flag setting function for setting the amount of luminescent LEDs
*****************************************************************************************/
void Fulfillment_Lvl_Set(UINT a)
{
    Fulfillment_Lvl = a;
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @return   Percentage of glowing LEDs with one color. 
*
*
* @section Description
* Flag geting function for get the amount of luminescent LEDs
*****************************************************************************************/
UINT Fulfillment_Lvl_Get(void)
{
    return Fulfillment_Lvl;
}


/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @return  Returns 1 when successful
*
* @section Description
* A function that performs switching LEDs to the next position
*****************************************************************************************/
UINT8 LED_Update(void)
{
    static LED_RGA_type Red = RED; /**< enum for RED LED */
    static LED_RGA_type Green = GREEN; /**< enum for GREEN LED */
    static UINT pos1 = INIT_WEKTORA_POZYCJI; /**< Start Wector for Green LED */
    static UINT pos2 = INIT_WEKTORA_POZYCJI; /**< Start Wector for Red LED */
    
    if(LOCK == 0)
    {
    LED_Control(RED,0b0000000000);
    LED_Light_Pos(Green,pos2,Fulfillment_Lvl);
    
    }
    else
    {
    LED_Control(Green,0b0000000000);
    LED_Light_Pos(RED,pos1,Fulfillment_Lvl);
    }
    pos1 = LED_Right(pos1);
    pos2 = LED_Left(pos2);
    
    return 1;
}
/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @param a Wector of LED status (1 when glowing, 0 when shut down)
* 
* @return  Wector of LED status after move 1 position right
*
* @section Description
* A function that performs a cyclic shift of the leds vector to the right. The first bit becomes the last.
*****************************************************************************************/
UINT LED_Right(UINT a)
{
    if ((a & 0b0000000001) == 0b0000000001)
    {
        UINT b = a & 0b1111111110;
        b = b >> 1;
        b = b | 0b1000000000;
        return b;
    }
    else
    {
        a =a >> 1;
        return a;
    }
}
/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @param a Wector of LED status (1 when glowing, 0 when shut down)
* 
* @return  Wector of LED status after move 1 position left
*
* @section Description
* A function that performs a cyclic shift of the leds vector to the left. The last bit becomes the first.
*****************************************************************************************/
UINT LED_Left(UINT a)
{
    if ((a & 0b1000000000) == 0b1000000000)
    {
        UINT b = a & 0b0111111111;
        b = b << 1;
        b = b | 0b0000000001;
        return b;
    }
    else
    {
        a = a << 1;
        return a;
    }
}
/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @param color Color of LED-s to glow up
* @param pos Wector of LED status (1 when glowing, 0 when shut down)
* @param fulfillment Percentage of glowing LEDs with one color.
* 
* @return  Wector of LED status
*
* @section Description
* The function glowing up the appropriate number of LEDs depending on the position, filling and color. Red diode are lit left from the starting position and green to the right.
*****************************************************************************************/
UINT LED_Light_Pos(LED_RGA_type color,UINT pos, UINT fulfillment)
{
    UINT pos2 =pos;
    UINT LED_array = 0b0000000000;
    UINT flage = fulfillment/10;
    if(color == RED)
    {
        while(flage>0)
        {
            LED_array = pos2|LED_array;
            pos2 = LED_Left(pos2);
            flage = flage - 1;
        }
    }
    else
    {
        while(flage>0)
        {
            LED_array = pos2|LED_array;
            pos2 = LED_Right(pos2);
            flage = flage - 1;
        }
    }
    LED_Control(color, LED_array);
    return LED_array;
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @section Description
* The function of setting the outputs for LEDs.
*****************************************************************************************/
void INI_LED_Start(void)
{
    TRISA = TRISA & 0b00000001;
    TRISB = TRISB & 0b11111100;
    TRISE = TRISE & 0b11111000;
    TRISD = TRISD & 0b00000000;
    LED_Control(RED,0b0000000000);
    LED_Control(GREEN,0b0000000000);
    
    
}

/***************************************************************************************/
/**
* @author Kamil Szkaradnik
* @date 08/07/2019
*
* @param color Color of LED-s to glow up
* @param diode Vector of Led Diode to light up
* 
* @return  Returns 1 if successful
*
* @section Description
* The function glowing up LEDs on specified position.
*****************************************************************************************/
UINT8 LED_Control(LED_RGA_type color,UINT diode)
{
    UINT cos  =1;
    if(color == RED)
    {
        // LED 1
        if((diode & 0b0000000001)== 0b0000000001) 
        {
            LATD = LATD | ~0b11101111;
        }
        else
        {
            LATD = LATD &0b11101111;
        }
        // LED 2
        if((diode & 0b0000000010) == 0b0000000010)
        {
            LATD = LATD | ~0b10111111;
        }
        else
        {
            LATD = LATD  & 0b10111111;
        }
        // LED 3
        if((diode & 0b0000000100) == 0b0000000100)
        {
            LATB = LATB | ~0b11111110;
        }
        else
        {
            LATB = LATB & 0b11111110;
        }
         
        // LED 4
        if((diode & 0b0000001000) == 0b0000001000)
        {
            LATA = LATA | ~0b11111101;
        }
        else
        {
            LATA = LATA & 0b11111101;
        }
            
        // LED 5
        if((diode & 0b0000010000) == 0b0000010000)
        {
            LATA = LATA | ~0b11110111;
        }
        else
        {
            LATA = LATA & 0b11110111;
        }    
            
        // LED 6
        if((diode & 0b0000100000) == 0b0000100000)
        {
            LATA = LATA | ~0b11011111;
        }
        else
        {
            LATA = LATA & 0b11011111;
        }    
        // LED 7
        if((diode & 0b0001000000) == 0b0001000000)
        {
            LATE = LATE | ~0b11111101;
        }
        else
        {
            LATE = LATE & 0b11111101;
        }    
        // LED 8
        if((diode & 0b0010000000) == 0b0010000000)
        {
            LATA = LATA | ~0b01111111;
        }
        else
        {
            LATA = LATA & 0b01111111;
        }    
            
        // LED 9
        if((diode & 0b0100000000) == 0b0100000000)
        {
            LATD = LATD | ~0b11111110;
        }
        else
        {
            LATD = LATD & 0b11111110;
        }    
        // LED 10
        if((diode & 0b1000000000) == 0b1000000000)
        {
            LATD = LATD | ~0b11111011;
        }
        else
        {
            LATD = LATD & 0b11111011;
        }    
                
    }
    
    if(color == GREEN)
    {
        // LED 1
        if((diode & 0b0000000001) == 0b0000000001) 
        {
            LATD = LATD | ~0b11011111;
        }
        else
        {
            LATD = LATD & 0b11011111;
        }
            
        // LED 2
        if((diode & 0b0000000010) == 0b0000000010)
        {
            LATD = LATD | ~0b01111111;
        }
        else
        {
            LATD = LATD & 0b01111111;
        }    
            
        // LED 3
        if((diode & 0b0000000100) == 0b0000000100)
        {
            LATB = LATB | ~0b11111101;
        }
        else
        {
            LATB = LATB & 0b11111101;
        }    
        // LED 4
        if((diode & 0b0000001000) == 0b0000001000)
        {
            LATA = LATA | ~0b11111011;
        }
        else
        {
            LATA = LATA & 0b11111011;
        }    
            
        // LED 5
        if((diode & 0b0000010000) == 0b0000010000)
        {
            LATA = LATA | ~0b11101111;
        }
        else
        {
            LATA = LATA & 0b11101111;
        }    
            
        // LED 6
        if((diode & 0b0000100000) == 0b0000100000)
        {
            LATE = LATE | ~0b11111110;
        }
        else
        {
            LATE = LATE & 0b11111110;
        }    
            
        // LED 7
        if((diode & 0b0001000000) == 0b0001000000)
        {
            LATE = LATE | ~0b11111011;
        }
        else
        {
            LATE = LATE & 0b11111011;
        }
        // LED 8
        if((diode & 0b0010000000) == 0b0010000000)
        {
            LATA = LATA | ~0b10111111;
        }
        else
        {
            LATA = LATA & 0b10111111;
        }    
        // LED 9
        if((diode & 0b0100000000) == 0b0100000000)
        {
            LATD = LATD | ~0b11111101;
        }
        else
        {
            LATD = LATD & 0b11111101;
        }
        // LED 10
        if((diode & 0b1000000000) == 0b1000000000)
        {
            LATD = LATD | ~0b11110111;
        }
        else
        {
            LATD = LATD &0b11110111;
        }
                
    }
    return 1;
}
