/**
*   @file   BINAERZAEHLER.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   14.12.2014
*   @date   14.01.2015
*   @brief  Main File of BINAERZAEHLER Example
*   @bug    No known bugs.
*
*/

#include "Matrix_RKAG.h"

#include "SPI.h"
#include "Wire.h"


//Zahlvariabel fuer innerhalb vom Main
int x = 0;

void setup()
{
	//Initalisierung der Matrix HW Komponenten
	matrix.init();


}

void loop()
{

	
	//Wenn x 255 entspricht
	if(x == 255)
	{
		//x auf null setzen da 8Bit = maximal Dezimal 255
		x = 0;
	}
	else
	{
		//x inkrementieren
		x++;
	}
	
	//oberste Zeile entsprechend x in Binaer ausgeben, restliche Zeilen 0
	matrix.write(x, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);   
    
	//20ms warten
	delay(20);
}
