/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "delay.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"


// TODO Insert C++ class definitions if appropriate
#define CS 0x2000 //pin 13 active low
/* 
 * Reset triggers the following values:
 * Display is Off
 * 128 MUX display mode
 * Normal segment and display data column address and row address mapping (SEG0 mapped to address 0x00 and COM0 mapped to address 0x00)
 * Display start line is set at DRAM address 0
 * Column address counter is set at 0
 * Normal scan direction of the COM outputs
 * Command 0xA2,0xB1,0xB3,0xBB,0xBE are locked by command 0xFD
 */
#define RST 0x4000 //pin 14 active low

#define DC 0x8000 //pin 15

typedef uint8_t byte;


//List of commands
#define SET_COLUMN_ADDRESS 0x15
#define SET_ROW_ADDRESS 0x75
#define WRITE_RAM 0x5C
#define READ_RAM 0x5D
#define SET_CONFIG 0xA0 // In the data-sheet this is SET Re-map/Color Depth (Display RAM to Panel)
#define SET_DISPLAY_START_LINE 0xA1 //Set vertical scroll by RAM from 0~127
#define SET_DISPLAY_OFFSET 0xA2 //Set Vertical scroll  by Row from 0-127
#define SET_DISPLAY_ALL_OFF 0xA4 //Sets all pixels off
#define SET_DISPLAY_ALL_ON 0xA5 //Sets all picels on (All picels have GS63)
#define SET_DISPLAY_NORMAL 0xA6 //Reset to normal display [reset]
#define SET_DISPLAY_INVERSE 0xA7 //Inverts all pixels on display
#define FUNCTION_SELECTION 0xAB //Allows for enabling or disabling internal V_dd reg and setting interface (defaults to SPI)
#define DISPLAY_NOP 0xAD //NOP
#define DISPLAY_OFF 0xAE //Display Off
#define DISPLAY_ON 0xAF //Display On
#define SET_RESET_PRECHARGE_PERIOD 0xB1 //Set Reset (Phase 1)/Pre-charge (Phase 2) period
#define DISPLAY_ENHANCEMENT 0xB2 //Followed by three data bytes, engances performance (idk why I put this here)
#define FRONT_CLOCK_DIVIDER_OSC_FREQ 0xB3 //Followed by a data byte to select divider and internal oscillator frequency
                                            //This is locked by command 0xFD
#define SET_SEGMENT_LOW_VOLTAGE 0xB4 //Followed by 3 data bytes idk that I'm gonna use this
#define SET_GPIO 0xB5 //GPIO command is probably not useful here
#define SET_SECOND_PRECHARGE_PERIOD 0xB6 //Set Second Pre-Charge Period
#define SET_GRAY_SCALE_TABLE 0xB8 //Followed by 63 data bytes of gamma settings for GS0-GS63
#define SET_GRAY_SCALE_LINEAR 0xB9 //Just use this for grayscale, this is the reset value as well
#define SET_PRE_CHARGE_VOLTAGE 0xBB //Set pre-charge voltage level I hope I don't have to edit reset value, locked by 0xFD
#define SET_COM_DESELECT_VOLTAGE 0xBE //Set COM deselect voltage level, locked by 0xFD
#define SET_CONTRAST_CURRENT 0xC1 //Followed by 3 data bytes of contrast values for colors A. B, and C
#define SET_MASTER_CONTRAST 0xC7 //Master Contrast Current Control (defaults to max)
#define SET_MUX_RATIO 0xCA //Set MUX RATIO (default at highest)
#define SET_COMMAND_LOCK 0xFD //Configure OLED driver IC MCU interface lock.
#define HORIZONTAL_SCROLL 0x96 //Horizontal Scroll setting
#define STOP_MOVING 0x9E //Stop Horizontal Scroll (ram data needs to be rewritten after this command)
#define START_MOVING 0x9F //Start Horizontal scroll


//Color definitons:




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    
    struct color { //RGB -565 format (Display uses Big Endian)
        byte MSB; 
        byte LSB;
    };
    
    struct point {
        uint8_t x;
        uint8_t y;
    };
    
    typedef struct color color_t;
    typedef struct point point_t;
    
    
    void drawLine( point_t start, point_t end, color_t color);
    
    
    void delay_ms(int time);
    
    void displaySetup();
    
    void startup();
    
    byte spiWrite(byte* data, uint16_t len);
    
    void chipEnable(bool);
    
    void setDC(bool);
    
    void setColumn (byte start, byte stop);
    
    void setRow(byte start, byte stop);
    
    void setPixel(point_t spot, color_t color);
    
    
    //Reset the display with the RST wire
    void displayReset();
    
    


    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

