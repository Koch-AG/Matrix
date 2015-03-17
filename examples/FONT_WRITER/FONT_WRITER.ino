/**
*   @file   FONT_WRITER.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   20.12.2014
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
            matrix.font_write(x);
            
            delay(500);
        }
        
        matrix.font_write(142);
        
        delay(500);
        
        matrix.font_write(153);
        
        delay(500);
        
        matrix.font_write(154);
        
        delay(500);

}
