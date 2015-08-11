/**
*   @file   FONT_WRITER_POTI.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   11.8.2015
*   @brief  Main File of FONT_WRITER_POTI Example
*   @bug    No known bugs.
*
*/

#include "Matrix_RKAG.h"

#include "SPI.h"
#include "Wire.h"

void setup()
{
	//Initialize HW
	matrix.init();

}


void loop()
{
	matrix.read_io();

	matrix.write_char(matrix.potentiometer_0/40+65);

	delay(20);
}
