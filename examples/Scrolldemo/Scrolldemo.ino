/**
*   @file   Scrolldemo.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   2.2.2015
*   @brief  Main File for the Matrix
*   @bug    Why so much Ram Usage?!
*
*/

#include <Matrix_RKAG.h>

#include <TimerOne.h>
#include "SPI.h"
#include <Wire.h>

#include "font.h"


//Allgemeine Variabeln für den Scroller
boolean space[8][180];
char buffer2[8];
char screen[8];

void writer(int numb, int stell)
{
	int pos;

	pos = (stell * 6) - 8;
	
	//Font Daten aus Flash Speicher holen und in Matrix Array schreiben
	for(int x = 0; x <= 7; x++)
	{
		buffer2[x] = pgm_read_byte(&font[numb][x]);
	}
	
	//Buffer (Zeichenbuffer/Eigentliche Zeichen) in Space schreiben
	for(int x = 0; x <= 7; x++)	//Zeile wählen
	{
		for(int c = 1; c <= 8; c++)	//Hilfszahl/Spalte wählen
		{
			if(buffer2[x] & (0x01 << (c - 1)))	//Prüfen ob entsprechendes Bit gesetzt ist (Anhand kopierten Bits aus dem Flash PGM)
			{
				space[x][(c-1)+pos] = 1;	//Bit im Space setzen
			}
			else
			{
				if(c > 2)	//Hintersten 3 Spalten ignorieren da im normalfall leer und überlappend
				{
					space[x][(c-1)+pos] = 0;	//Bit löschen
				}
			}
		}
	}
}

void cutscreen(int offset)
{
	//Space in einen 8x8 Auschnitt schneiden um diesen danach ausgeben zu können
	for(int y = 0; y <= 7; y++)	//Zeile wählen
	{
		for(int d = 0; d <= 7; d++)	//Spalte wählen
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

	//Die 8 angegebenen geschnittenen Bytes ausgeben
	matrix.write(screen[0], screen[1], screen[2], screen[3], screen[4], screen[5], screen[6], screen[7]);

}

void setup()
{
	//Initalisierung der Matrix HW Komponenten
	matrix.init();
	
	//Writer um Buchstaben in den Space zu hauen
	for(int x = 0; x <= 25; x++)
	{
		writer(x+65,x+1);		
	}
}


void loop()
{
	matrix.read_io();

	//Offset von 0-63
	cutscreen((matrix.potentiometer_0/8));
}
