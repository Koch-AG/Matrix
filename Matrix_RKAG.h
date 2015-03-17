/**
*   @file   	Matrix_RKAG.h
*   @author 	Benjamin Marty (bmarty@kochag.ch)
*   @date   	09.03.2014
*   @brief  	Header File of Matrix_RKAG Library
*   @bug    	No known bugs.
*	@version	1.1.0
*
*/


// ensure this library description is only included once
#ifndef Matrix_RKAG_h
#define Matrix_RKAG_h

#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>

// library interface description
class matrix_class
{
  // user-accessible "public" interface
  public:
	matrix_class();

	void init();
	int read_io();

    void write(char byte1, char byte2, char byte3, char byte4, char byte5, char byte6, char byte7, char byte8);
    void write_array(char matrix[]);
    void clear();
	void sample(int numb);
	void font_write(int numb);

	void pcf_write(int data);
	int pcf_read();
	
	int taste_1;
	int taste_2;
	int taste_3;

	int potentiometer_0;

  private:
   	unsigned long microseconds_now; 	
  	unsigned long microseconds_saved;

	int counter_taste_1;
	int counter_taste_2;
	int counter_taste_3; 
};

extern matrix_class matrix;

#endif

