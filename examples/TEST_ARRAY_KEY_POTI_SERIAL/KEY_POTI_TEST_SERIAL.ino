/**
*   @file   FONT_WRITER_KEY.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   03.03.2014
*   @brief  Main File of FONT_WRITER_KEY Example
*   @bug    No known bugs.
*
*/

#include "Matrix_RKAG.h"

#include "SPI.h"
#include "Wire.h"

//Instanz der Klasse bilden
//matrix matrix;

int potentiometer_vorher;

char bla[8] = { 0b10101010, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b01010101 };

void setup()
{
	//Initalisierung der Matrix HW Komponenten
	matrix.init();
	
	//attachInterrupt(0, taster, FALLING);

	Serial.begin(9600);	

	matrix.write_array(bla);
}


void loop()
{
	matrix.read_io();

	if(matrix.taste_1)
	{
		Serial.print("Taste1");
		Serial.print("\n");
	}

	if(matrix.taste_2)
	{
		Serial.print("Taste2");
		Serial.print("\n");
	}

	if(matrix.taste_3)
	{
		Serial.print("Taste3");
		Serial.print("\n");
	}

	if(matrix.potentiometer_0 != potentiometer_vorher)
	{
		Serial.print("Potentiometer aendert sich");
		Serial.print("\n");

		potentiometer_vorher = matrix.potentiometer_0;
	}

}

