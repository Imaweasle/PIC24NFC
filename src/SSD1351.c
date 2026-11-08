#include "SSD1351.h"



byte param;


void displaySetup() {

    /* The following code sequence shows SPI register configuration for Master mode */
    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0; // Disable the interrupt
    
    
    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0;    // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0;    // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 0;    // Communication is byte-wide (8 bits)
    SPI1CON1bits.MSTEN = 1;     // Master mode enabled
    SPI1CON1bits.SMP = 0;       // Input data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 0;       // Serial output data changes on transition from
                                // Idle clock state to active clock state
    SPI1CON1bits.CKP = 1;       // Idle state for clock is a low level;
                                // active state is a high level
    SPI1CON1bits.PPRE = 2;     //4 to 1 decrease
    SPI1CON1bits.SPRE = 7;      //1:1
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 1;     // Enable SPI module
    
    
    

    RPOR4bits.RP9R = 7;
    RPOR4bits.RP8R = 8;

    
    
    
    
    // Interrupt Controller Settings
    //IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    //IEC0bits.SPI1IE = 1; // Enable the interrupt
    
    TRISB = 0x1FFF;
    LATB |= RST;
    chipEnable(0);
    setDC(0);
    
    
}


//Set DC (command is low)
void setDC(bool x) {
    if(x) {
        LATB |= DC;
    } else {
        LATB &= ~DC;
    }
}

//Set chip select accordingly
void chipEnable(bool x){
    if(!x){
        LATB |= CS;
    }else {
        LATB &= ~CS;
    }
    
}

void delay_ms(int time) {
    while(time > 0) {
        delay1000();
        time--;
    }
}

void displayReset() {
    
    LATB &= ~RST; //set RST Low
    delay100();
    LATB |= RST;
    delay_ms(5); //minimum 2 ms but eh we'll not build to tolerance
}
byte spiWrite(byte* data, uint16_t len){
    unsigned int accumulator = 0;
    volatile int rec = 0; //To clear received bits
    while(accumulator < len) {
        SPI1BUF = data[accumulator];
        while(!SPI1STATbits.SPIRBF){
            
        }
        rec = SPI1BUF;
        accumulator++;
    }
    
    return 0;
}

void drawLine( point_t start, point_t end, color_t color) {
    int xDelta = end.x - start.x;
    int yDelta = end.y - start.y;
    
    
    point_t pixel;
    
    if(xDelta != 0) {
        float slope = (float)yDelta / (float)xDelta;
        if(fabs(slope) > 1) {
            //Paramaterize as x= (y-b)/m
            slope = (float)xDelta / (float) yDelta;
            if(yDelta < 0) {
                
                for(pixel.y = start.y; pixel.y > end.y; pixel.y--) {
                    pixel.x = (abs(pixel.y - start.y) * slope) + start.x;
                    
                    setPixel(pixel, color);
                }
            } else {
                for(pixel.y = start.y; pixel.y < end.y; pixel.y++) {
                    pixel.x = (abs(pixel.y - start.y) * slope) + start.x;
                    
                    setPixel(pixel, color);
                }
                
                
            }
            
        } else {
            
            if(xDelta < 0) {
                for(pixel.x = start.x; pixel.x > end.x; pixel.x--) {
                    pixel.y = (abs(pixel.x-start.x) * slope) + start.y;
                    
                    setPixel(pixel, color);
                    
                    
                }
            } else {
                for(pixel.x = start.x; pixel.x < end.x; pixel.x++) {
                    pixel.y = (abs(pixel.x-start.x) * slope) + start.y;
                    
                    setPixel(pixel, color);
                }
            }
        }
       
    }
    
}

void setPixel(point_t spot, color_t color) {
    setColumn(spot.x, spot.x+1);
    setRow(spot.y, spot.y+1);
    
    chipEnable(1);
    
    setDC(0);
    
    param = WRITE_RAM;
    spiWrite(&param, 1);

    setDC(1);
    byte data[2] = {color.MSB, color.LSB};
    spiWrite(&data, 2);
    setDC(0);
    chipEnable(0);
         
   
    
}







void setRow(byte start, byte stop) {
    chipEnable(1);
            
    
    setDC(0);
    param = SET_ROW_ADDRESS;
    spiWrite(&param, 1);
    
    setDC(1);
    byte data[2] = {start, stop};
    spiWrite(&data, 2);
    
    setDC(0);
    
    chipEnable(0);
    
}

void setColumn(byte start, byte stop) {
    chipEnable(1);
    setDC(0);
    param = SET_COLUMN_ADDRESS;
    spiWrite(&param, 1);
    
    setDC(1);
    byte data[2] = {start, stop};
    spiWrite(&data, 2);
    setDC(0);
    chipEnable(0);
        
}


void startup() {
    chipEnable(1);
    delay100(); //Just in case
    displayReset();
    
    byte color[2] = {0x00, 0x00};
    
    
    
    byte data[2];
    
    
    
    
    
    
    param = DISPLAY_ON;
    spiWrite(&param, 1);
    
    
    param = SET_COMMAND_LOCK;
    spiWrite(&param, 1);
    
    setDC(1);
    data[0] = 0xB1; //Unlock extra commands
    spiWrite(&data, 1);
    
    setDC(0);
    
    
    
    param = SET_DISPLAY_OFFSET;
    spiWrite(&param, 1);
    
    
    setDC(1);
    data[0] = 0x00;
    spiWrite(&data, 1);
    
    setDC(0);
    
    
    
    
    // Column address
    setColumn(0, 0x7F);


    // Row address
    setRow(0x00, 0x7F);
    chipEnable(1);
    
    setDC(0);
    param = WRITE_RAM;
    spiWrite(&param, 1);
    
    setDC(1);// Set data high to send GDDRAM info
    uint16_t i = 0;
    for(i = 0; i < 32768; i++) {
        spiWrite(color, 2);
    }
    setDC(0);
    
    
    
    
    
    
    
    
    
    
}



//void sendCommand(byte command)
//{
//    setDC(0);
//    spiWrite(&command, 1);
//}
//
//void sendData(byte data)
//{
//    setDC(1);
//    spiWrite(&data, 1);
//}
//
//void startup(void)
//{
//    byte data[3];
//
//    chipEnable(1);
//
//    // Hardware reset
//    displayReset();
//
//    // Unlock command interface
//    sendCommand(SET_COMMAND_LOCK);
//    data[0] = 0x12;
//    setDC(1);
//    spiWrite(data, 1);
//
//    // Allow access to protected commands
//    sendCommand(SET_COMMAND_LOCK);
//    data[0] = 0xB1;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Display OFF while configuring
//    sendCommand(DISPLAY_OFF);
//
//
//    // Clock divider / oscillator
//    sendCommand(FRONT_CLOCK_DIVIDER_OSC_FREQ);
//    data[0] = 0xF1;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Multiplex ratio = 128
//    sendCommand(SET_MUX_RATIO);
//    data[0] = 0x7F;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Remap / color depth
//    sendCommand(SET_CONFIG);
//    data[0] = 0x74;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Column address
//    sendCommand(SET_COLUMN_ADDRESS);
//    data[0] = 0x00;
//    data[1] = 0x7F;
//    setDC(1);
//    spiWrite(data, 2);
//
//
//    // Row address
//    sendCommand(SET_ROW_ADDRESS);
//    data[0] = 0x00;
//    data[1] = 0x7F;
//    setDC(1);
//    spiWrite(data, 2);
//
//
//    // Display start line
//    sendCommand(SET_DISPLAY_START_LINE);
//    data[0] = 0x00;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Display offset
//    sendCommand(SET_DISPLAY_OFFSET);
//    data[0] = 0x00;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // GPIO
//    sendCommand(SET_GPIO);
//    data[0] = 0x00;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Function selection
//    sendCommand(FUNCTION_SELECTION);
//    data[0] = 0x01;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Phase length
//    sendCommand(SET_RESET_PRECHARGE_PERIOD);
//    data[0] = 0x32;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Segment low voltage
//    sendCommand(SET_SEGMENT_LOW_VOLTAGE);
//    data[0] = 0xA0;
//    data[1] = 0xB5;
//    data[2] = 0x55;
//    setDC(1);
//    spiWrite(data, 3);
//
//
//    // Precharge voltage
//    sendCommand(SET_PRE_CHARGE_VOLTAGE);
//    data[0] = 0x17;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // COM deselect voltage
//    sendCommand(SET_COM_DESELECT_VOLTAGE);
//    data[0] = 0x05;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Contrast
//    sendCommand(SET_CONTRAST_CURRENT);
//    data[0] = 0xC8;
//    data[1] = 0x80;
//    data[2] = 0xC8;
//    setDC(1);
//    spiWrite(data, 3);
//
//
//    // Master contrast
//    sendCommand(SET_MASTER_CONTRAST);
//    data[0] = 0x0F;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Second precharge
//    sendCommand(SET_SECOND_PRECHARGE_PERIOD);
//    data[0] = 0x01;
//    setDC(1);
//    spiWrite(data, 1);
//
//
//    // Normal display mode
//    sendCommand(SET_DISPLAY_NORMAL);
//
//
//    // Display ON
//    sendCommand(DISPLAY_ON);
//
//
//    // Set column again
//    sendCommand(SET_COLUMN_ADDRESS);
//    data[0] = 0x00;
//    data[1] = 0x7F;
//    setDC(1);
//    spiWrite(data, 2);
//
//
//    // Set row again
//    sendCommand(SET_ROW_ADDRESS);
//    data[0] = 0x00;
//    data[1] = 0x7F;
//    setDC(1);
//    spiWrite(data, 2);
//
//
//    // Start writing RAM
//    sendCommand(WRITE_RAM);
//
//
//    // 128x128 = 16384 pixels
//    // RGB565 red = F800
//    byte red[2] = {0xF8, 0x00};
//
//    setDC(1);
//
//    for (uint32_t i = 0; i < 16384UL; i++)
//    {
//        spiWrite(red, 2);
//    }
//
//    setDC(0);
//}