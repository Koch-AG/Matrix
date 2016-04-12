/**
*   @file   FONT_WRITER_KEY.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   12.04.2016
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
	//Eing�nge (Potentiometer + 3Taster abfragen und aktualisieren
	matrix.read_io();

	//Pr�fen ob Taste 1 gedr�ckt
	if(matrix.button_1)
	{
		//Ascii Zeichen ausgeben
		matrix.write_char(49);
	}
	
	//Pr�fen ob Taste 2 gedr�ckt
	if(matrix.button_2)
	{
		//Ascii Zeichen ausgeben
		matrix.write_char(50);
	}

	//Pr�fen ob Taste 3 gedr�ckt
	if(matrix.button_3)
	{
		//Ascii Zeichen ausgeben
		matrix.write_char(51);
	}
}

