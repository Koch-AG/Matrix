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
}


void loop()
{
	//Eingänge (Potentiometer + 3Taster abfragen und aktualisieren
	matrix.read_io();

	//Prüfen ob Taste 1 gedrückt
	if(matrix.taste_1)
	{
		//Ascii Zeichen ausgeben
		matrix.font_write(49);
	}
	
	//Prüfen ob Taste 2 gedrückt
	if(matrix.taste_2)
	{
		//Ascii Zeichen ausgeben
		matrix.font_write(50);
	}

	//Prüfen ob Taste 3 gedrückt
	if(matrix.taste_3)
	{
		//Ascii Zeichen ausgeben
		matrix.font_write(51);
	}
}

