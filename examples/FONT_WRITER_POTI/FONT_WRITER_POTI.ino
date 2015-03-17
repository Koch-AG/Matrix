/**
*   @file   FONT_WRITER_POTI.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   20.12.2014
*   @brief  Main File of FONT_WRITER_POTI Example
*   @bug    No known bugs.
*
*/

#include "Matrix_RKAG.h"

#include "SPI.h"
#include "Wire.h"

void setup()
{
	//Initalisierung der Matrix HW Komponenten
	matrix.init();

}


void loop()
{
	matrix.read_io();

	matrix.font_write(matrix.potentiometer_0/40+65);

	delay(20);
}
