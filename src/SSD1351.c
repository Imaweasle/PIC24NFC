#include "../include/SSD1351.h"



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
    spiWrite(data, 2);
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
    spiWrite(data, 2);
    
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
    spiWrite(data, 2);
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
    spiWrite(data, 1);
    
    setDC(0);
    
    
    
    param = SET_DISPLAY_OFFSET;
    spiWrite(&param, 1);
    
    
    setDC(1);
    data[0] = 0x00;
    spiWrite(data, 1);
    
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