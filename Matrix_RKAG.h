/**
* \file     Matrix_RKAG.cpp
* \brief    RKAG Matrix library source file
* \author   Benjamin Marty <bmarty@kochag.ch>
* \author   Sven Gehring <sgehring@kochag.ch>
* \version  1.3.0
*/


#ifndef MATRIX_RKAG_H
#define MATRIX_RKAG_H

#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>


/** \brief RKAG Matrix library class
* This class grants access to all major matrix functionality to
* control the RKAG edu project
*/
class rkag_matrix {
  public:
	rkag_matrix ();

	void init (void);
	int read_io (void);

    void clear (void);
    void sample (int pattern);
    void write_char (char character);
    void write_array (byte matrix[]);
    void write (byte byte1, byte byte2, byte byte3, byte byte4,
                byte byte5, byte byte6, byte byte7, byte byte8);
    void progressbar(byte value, byte line);

	void pcf_write (int data);
	int pcf_read (void);

    void accelerometer_write(byte addressToWrite, byte dataToWrite);
	int accelerometer_read_byte(byte addressToRead);
    void accelerometer_read_multiple(byte addressToRead, int bytesToRead, byte * dest);
    void accelerometer_standby(void);
    void accelerometer_active(void);
    void accelerometer_init(void);
    void accelerometer_meassure(float *destination);

	int button_1;
	int button_2;
	int button_3;

    float accelerometer_x, accelerometer_y, accelerometer_z;

	int potentiometer_0;

  private:
   	unsigned long microseconds_now; 	
  	unsigned long microseconds_saved;

	int counter_button_1;
	int counter_button_2;
	int counter_button_3; 
};

extern rkag_matrix matrix;

#endif                                                                           /* MATRIX_RKAG_H */

