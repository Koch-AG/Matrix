/**
* \file     Matrix_RKAG.cpp
* \brief    RKAG Matrix library source file
* \author   Benjamin Marty <bmarty@kochag.ch>
* \author   Sven Gehring <sgehring@kochag.ch>
* \version  1.3.0
*/


#include <SPI.h>
#include <Wire.h>
#include <TimerOne.h>

#include "Matrix_RKAG.h"
#include "acc_register.h"
#include "font.h"

#define RCK A3                                                    /* define RCK as Arduino Pin A3 */
#define SRCLR A2                                                /* define SRCLR as Arduino Pin A2 */

#define PCF_ADDRESS 0x38                                  /* define port expander address as 0x38 */
#define ACCELEROMETER_ADRESS 0x1D                         /* define accelerometer address as 0x1D */

#define GSCALE 2           /* Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values */
#define PCF_MASKBTN 1       /* indicator if button values (b0 - b2 should be masked in pcf write) */
#define BTN_THRESHOLD 2                    /* button counter threshold for detecting button press */
#define ACCELEROMETER_ACTIVE 0x01                /* indicator if the accelerometer should be used */


static byte data[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};      /* matrix spi buffer */


/** \brief Matrix class constructor
* Construct a new instance of the matrix class, initialize pins RCK, SRCLR & SS
*
*/
rkag_matrix::rkag_matrix() {
    pinMode(RCK, OUTPUT);                                          /* register clock, high active */
    pinMode(SRCLR, OUTPUT);                                                /* SRClear, low active */
    pinMode(SS, OUTPUT);

    digitalWrite(RCK, 0);
    digitalWrite(SRCLR, 1);
    digitalWrite(SS, 1);
}


/** \brief Matrix loop that is bound to the hardware timer
* This method will be bound to the hardware timer and called in every iteration
*/
void matrix_timer() {
    static int counter;
    
    digitalWrite(SRCLR, 1);                                                   /* SR storage reset */
    digitalWrite(SRCLR, 0);
    digitalWrite(SRCLR, 1);
 
    SPI.transfer(0x80 >> counter);                                         /* transfer single row */
    SPI.transfer(data[counter]);                                         /* transfer counter data */

    digitalWrite(RCK, 0);                         /* generate puls, set register to output buffer */
    digitalWrite(RCK, 1);
    digitalWrite(RCK, 0);

    if (counter == 7) counter = 0;                        /* reset counter to write another 8 bit */
    else counter++;                                        /* increment counter to calcuate 8 bit */
}


/** \brief Initialize matrix class instance
* This will configure the instance bound timer as well as the
* internal SPI connection used to transfer the data.
*/
void rkag_matrix::init() {
    Wire.begin();                                                /* initialize i2c bus as master  */

    Timer1.initialize(800);                    /* initialize timer with 8ms (~15.6Hz update rate) */
    Timer1.attachInterrupt(matrix_timer);         /* bind method 'matrix_timer' to hardware timer */

    SPI.begin();                                                            /* initialize spi bus */
    SPI.setClockDivider(SPI_CLOCK_DIV128);    /* set spi clock divider to 128, 16Mhz/128 = 125kHz */
    SPI.setBitOrder(LSBFIRST);                                 /* set bit order to send LSB first */

    if(ACCELEROMETER_ACTIVE == 0x01)
    {
        matrix.accelerometer_init();                     /* initialize accelerometer if activated */
    }
}


/** \brief Internal update routine to update matrix IO
*/
int rkag_matrix::read_io() {
    char pcf_state;                                                 /* port expander state buffer */
    float accelerometer_state[3];                                   /* accelerometer value buffer */ 


    microseconds_now = micros();                        /* get current runtime microseconds value */
    pcf_state = pcf_read();                           /* get current value from pcf port expander */
    potentiometer_0 = 1023-(analogRead(A0));              /* read potentiometer from analog input */

    button_1 = button_2 = button_3 = 0x00;                           /* reset input button values */

    if ((microseconds_now - microseconds_saved) > 5000) {                    /* over 5ms delayed? */
        if(pcf_state & 0x01) counter_button_1++;                             /* get button inputs */
        if(pcf_state & 0x02) counter_button_2++;
        if(pcf_state & 0x04) counter_button_3++;

        microseconds_saved = micros();                         /* save new loop microsecond value */
    }


                                         /* if button counter is higher than the button threshold */
                                /* and the input is now 0 (released), the button has been pressed */
    if (counter_button_1 > BTN_THRESHOLD && (!(pcf_state & 0x01))) {
        counter_button_1 = 0x00;
        button_1 = 0x01;
    }


    if (counter_button_2 > BTN_THRESHOLD && (!(pcf_state & 0x02))) {
        counter_button_2 = 0x00;
        button_2 = 0x01;
    }


    if (counter_button_3 > BTN_THRESHOLD && (!(pcf_state & 0x04))) {
        counter_button_3 = 0x00;
        button_3 = 0x01;
    }

    if(ACCELEROMETER_ACTIVE == 0x01)
    {
        matrix.accelerometer_meassure(accelerometer_state);     /* read values from accelerometer */
        accelerometer_x = accelerometer_state[0];               /* store values to public storage */
        accelerometer_y = accelerometer_state[1];
        accelerometer_z = accelerometer_state[2];
    }
}


/** \brief Write a full set of characters to the matrix
* Each character stands for a line on the matrix display, starting from the top
*
* \param byte1 Binary value of line 1 led states
* \param byte2 Binary value of line 2 led states
* \param byte3 Binary value of line 3 led states
* \param byte4 Binary value of line 4 led states
* \param byte5 Binary value of line 5 led states
* \param byte6 Binary value of line 6 led states
* \param byte7 Binary value of line 7 led states
* \param byte8 Binary value of line 8 led states
*/
void rkag_matrix::write (byte byte1, byte byte2, byte byte3, byte byte4,
                          byte byte5, byte byte6, byte byte7, byte byte8) {
    data[0] = byte1;
    data[1] = byte2;
    data[2] = byte3;
    data[3] = byte4;
    data[4] = byte5;
    data[5] = byte6;
    data[6] = byte7;
    data[7] = byte8;
}


/** \brief Write a character array to the matrix display
* Equals '::write' but takes a full character array as input
*
* \param matrix Array with all line data chars
*/
void rkag_matrix::write_array(byte matrix[]) {
    for (int i = 0; i < 8; i++) data[i] = matrix[i];
}


/** \brief Write a character to the matrix display
*
* \param character The character to write
*/
void rkag_matrix::write_char(char character) {
    char buffer[8];                                            /* buffer to store fron from flash */

    rkag_matrix::clear();                                                  /* clear matrix screen */

    for(int i = 0; i < 8; i++)                                            /* read font from flash */
        buffer[i] = pgm_read_byte(&font[character][i]);
    

    for (int i = 0; i < 8; i++) {                     /* write font from buffer to matrix display */
        for (int j = 0; j < 8; j++) {    
            if (buffer[i] & (0x01 << j))
                data[i] |= (0x80 >> j) << 1;
        }
    }
}


/** \brief Clear the matrix by resetting all lines to zero
*
*/
void rkag_matrix::clear(void) {
    for (int i = 0; i < 8; i++) data[i] = 0x00;
}


/** \brief Write a matrix sample to the screen
* This will output a simple bit pattern to the screen for testing
*
* \param parttern The id of the pattern that should be drawn
*/
void rkag_matrix::sample(int pattern) {
    switch(pattern) {
        case 0:
            for (int i = 0; i < 8; i++)
                data[i] = ((i % 2) == 0) ? 0b10101010 : 0b01010101;

            break;
        case 1:
            for (int i = 0; i < 8; i++)
                data[i] = ((i % 2) == 0) ? 0b01010101 : 0b10101010;

            break;
        default: {
            break;
        }
    }
}

/** \brief Write a matrix sample to the screen
* This will output a simple bit pattern to the screen for testing
*
* \param parttern The id of the pattern that should be drawn
*/
void rkag_matrix::progressbar(byte value, byte line) {
    data[line] = ((0xFF00 >> value) & 0xFF);
}

/** \brief Write data to the i2c bus
* This will write the given data to the i2c bus
*
* \param data The data value
*/
void rkag_matrix::pcf_write(int data) {
    Wire.beginTransmission(PCF_ADDRESS);             /* start bus transimission with pcf instance */

    #if ( PCF_MASKBTN == 1 )
        Wire.write((~(0x07) & data ));        /* write pcf data without the last 3 bits (buttons) */
    #else
        Wire.write(data);                                                   /* write all pcf data */
    #endif

    Wire.endTransmission();                                               /* end bus transmission */
}


/** \brief Read data from the i2c bus
* This will read the input data from the i2c bus
*
* \return int The received data
*/
int rkag_matrix::pcf_read() {
    int pcf_buffer;                                                 /* buffer for pcf input value */

    Wire.beginTransmission(PCF_ADDRESS);             /* start bus transimission with pcf instance */
    Wire.requestFrom(PCF_ADDRESS, 1);                    /* request pcf data from correct address */

    while(Wire.available() == 0);                                  /* wait until data is received */

    pcf_buffer = Wire.read();                                        /* read data into the buffer */

    return pcf_buffer;                                                     /* return buffer value */
}


/** \brief Write data to the i2c accelerometer
* This will write the given data to the i2c bus
*
* \param data The data value
*/
void rkag_matrix::accelerometer_write(byte addressToWrite, byte dataToWrite) {
    Wire.beginTransmission(ACCELEROMETER_ADRESS);
    Wire.write(addressToWrite);
    Wire.write(dataToWrite);
    Wire.endTransmission();
}


/** \brief Read data from the i2c accelerometer
* This will read the input data from the i2c bus
*
* \return int The received data
*/
int rkag_matrix::accelerometer_read_byte(byte addressToRead) {
    Wire.beginTransmission(ACCELEROMETER_ADRESS);
    Wire.write(addressToRead);
    Wire.endTransmission(false);                 /* endTransmission but keep the connection active */

    Wire.requestFrom(ACCELEROMETER_ADRESS, 1);       /* Ask for 1 byte, once done, bus is released */

    while(!Wire.available()) ;                                   /* Wait for the data to come back */
    return Wire.read();                                                    /* Return this one byte */
}


/** \brief Read multiple bytes from the i2c accelerometer
* This will read the input data from the i2c bus
*
* \return int The received data
*/
void rkag_matrix::accelerometer_read_multiple(byte addressToRead, int bytesToRead, byte * dest)
{
    Wire.beginTransmission(ACCELEROMETER_ADRESS);
    Wire.write(addressToRead);
    Wire.endTransmission(false);                 /* endTransmission but keep the connection active */

    Wire.requestFrom(ACCELEROMETER_ADRESS, bytesToRead);            /* Ask for n bytes, once done, */
                                                                               /*  bus is released */

    while(Wire.available() < bytesToRead);       /* Hang out until we get the # of bytes we expect */

    for(int x = 0 ; x < bytesToRead ; x++)
    dest[x] = Wire.read();
}


/** \brief Set the accelerometer to the standby mode
*
*/
void rkag_matrix::accelerometer_standby(void) {
    byte c = accelerometer_read_byte(CTRL_REG1);
    accelerometer_write(CTRL_REG1, c & ~(0x01));         /* Clear the active bit to go into standby */
}


/** \brief Set the accelerometer to the active mode
*
*/
void rkag_matrix::accelerometer_active(void) {
    byte c = accelerometer_read_byte(CTRL_REG1);
    accelerometer_write(CTRL_REG1, c | 0x01);              /* Set the active bit to begin detection */
}


/** \brief Initialize accelerometer
* Test and intialize the MMA8452
*
*/
void rkag_matrix::accelerometer_init(void) {
    byte c = accelerometer_read_byte(WHO_AM_I);                           /* Read WHO_AM_I register */

    accelerometer_standby();

    // Set up the full scale range to 2, 4, or 8g.
    byte fsr = GSCALE;
    if(fsr > 8) fsr = 8; //Easy error check
    fsr >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
    accelerometer_write(XYZ_DATA_CFG, fsr);   

    accelerometer_active();
}


/** \brief Start meassurement from accelerometer
*
*/
void rkag_matrix::accelerometer_meassure(float *destination) {
    byte rawData[6];                                         /* Buffer to operate with X/Y/Z values */

    accelerometer_read_multiple(OUT_X_MSB, 6, rawData);              /* Read the six data registers */

    for(int i = 0; i < 3 ; i++)           /* Loop to calculate 12-bit ADC and g value for each axis */
    {
        int gCount = (rawData[i*2] << 8) | rawData[(i*2)+1];          /* Combine to 3x single value */
        gCount >>= 4;                                           /* righ-align the left-aligned data */

        if (rawData[i*2] > 0x7F)
        {
            gCount = ~gCount + 1;
            gCount *= -1;                                 /* Transform into negative 2's complement */
        }

        destination[i] = gCount;                       /* Record this gCount into the 3 float array */
    }

    for (int i = 0 ; i < 3 ; i++)                                      /* Calculate raw Data to G's */
    {
        destination[i] = (float) destination[i] / ((1<<12)/(2*GSCALE));
    } 
}


/**
 * Global instance of the matrix class
 */
rkag_matrix matrix;
