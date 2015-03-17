/**
*   @file   FONT_WRITER_KEY.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   20.12.2014
*   @brief  Main File of FONT_WRITER_KEY Example
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
	
	//attachInterrupt(0, taster, FALLING);
}


void loop()
{
	switch(matrix.key())
	{
		case 1:
		matrix.font_write(49);
		break;
		
		case 2:
		matrix.font_write(50);
		break;
		
		case 3:
		matrix.font_write(51);
		break;
	}
}

