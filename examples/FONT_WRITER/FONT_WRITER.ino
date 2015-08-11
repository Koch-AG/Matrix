/**
*   @file   FONT_WRITER.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   11.8.2015
*   @brief  Main File of FONT_WRITER Example
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

        //Speicher von Matrix leeren
		matrix.clear();
	
        for(int x = 65; x <= 90; x++)
        {
            matrix.write_char(x);
            
            delay(500);
        }
        
        matrix.write_char(142);
        
        delay(500);
        
        matrix.write_char(153);
        
        delay(500);
        
        matrix.write_char(154);
        
        delay(500);

}
