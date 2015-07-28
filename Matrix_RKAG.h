/**
* \file     Matrix_RKAG.cpp
* \brief    RKAG Matrix library source file
* \author   Benjamin Marty <bmarty@kochag.ch>
* \author   Sven Gehring <sgehring@kochag.ch>
* \version  1.2.0
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
    void write_array (char matrix[]);
    void write (char byte1, char byte2, char byte3, char byte4,
                char byte5, char byte6, char byte7, char byte8);

	void pcf_write (int data);
	int pcf_read (void);
	
	int button_1;
	int button_2;
	int button_3;

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

