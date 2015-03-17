/**
*   @file   EIN_AUS_DEMO.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   17.12.2014
*   @brief  Main File of EIN_AUS_DEMO Example
*   @bug    No known bugs.
*
*/

#include "Matrix_RKAG.h"

#include <SPI.h>
#include <Wire.h>


void setup()
{
	//Initalisierung der Matrix HW Komponenten
	matrix.init();
}


void loop()
{
	//Speicher von Matrix leeren
	matrix.clear();
	
	//1s warten  
	delay(1000);
	
	//Die 8 angegebenen Bytes in Matrix Speicher übernehmen  
	matrix.write(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
  
	//1s warten	
	delay(1000);
}
