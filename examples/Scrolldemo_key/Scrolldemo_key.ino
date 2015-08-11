/**
*   @file   Scrolldemo.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   11.8.2015
*   @brief  Main File for the Matrix
*   @bug    Generates Low memory Alert. No Idea why.
*   @note	Works with Library 1.2.0
*
*/

#include <Matrix_RKAG.h>

#include <TimerOne.h>
#include "SPI.h"
#include <Wire.h>

#include "font.h"


//General Variables for the Scroll Mechanism
boolean space[8][180];
char buffer2[8];
char screen[8];

int counter = 0;

void writer(int numb, int stell)
{
	int pos;

	pos = (stell * 6) - 8;
	
	//Get the Font Data from the Flash storage and write it to the Matrix Array
	for(int x = 0; x <= 7; x++)
	{
		buffer2[x] = pgm_read_byte(&font[numb][x]);
	}
	
	//Write the Buffer to the Space
	for(int x = 0; x <= 7; x++)	//Choose Row
	{
		for(int c = 1; c <= 8; c++)	//Choose column
		{
			if(buffer2[x] & (0x01 << (c - 1)))	//Check if Bit is set in Flash
			{
				space[x][(c-1)+pos] = 1;	//Set Bit in Space
			}
			else
			{
				if(c > 3)	//Ignore last 3 Columns to generate no overlapping
				{
					space[x][(c-1)+pos] = 0;	//Clear Bit
				}
			}
		}
	}
}

void cutscreen(int offset)
{
	//Cut the Space to a 8x8 Field
	for(int y = 0; y <= 7; y++)	//Choose Row
	{
		for(int d = 0; d <= 7; d++)	//Choose Column
		{
			if(space[y][d+offset] == 1)	//Prüfen ob Bit im Space gesetzt ist und entsprechend auf Screen verfahren. Offset wird übergeben beim Aufruf -> Position
			{
				screen[y] |= (0x80 >> d);
			}
			else
			{
				screen[y] &= ~((0x80 >> d));
			}
		}
	}

	//Write the 8 Bit's to the Matrix
	matrix.write(screen[0], screen[1], screen[2], screen[3], screen[4], screen[5], screen[6], screen[7]);

}

void setup()
{
	//Initalize HW
	matrix.init();
	
	//attachInterrupt(0, swcheck, FALLING);
	
	Serial.begin(9600);
	
	//Loop to write the Data to the Matrix
	for(int x = 0; x <= 25; x++)
	{
		writer(x+65,x+1);		
	}
}


void loop()
{
	matrix.read_io();

	//Offset of 0-63
	cutscreen(counter);
	

	if(matrix.button_1)
	{
		counter++;
	}

	if(matrix.button_3)
	{
		counter--;
	}


}
