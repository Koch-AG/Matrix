/**
*   @file   	Matrix_RKAG.cpp
*   @author 	Benjamin Marty (bmarty@kochag.ch)
*   @date   	09.03.2014
*   @brief  	cpp File of Matrix_RKAG Library
*   @bug    	No known bugs.
*	@version	1.1.0
*
*/


// include this library's description file
#include "Matrix_RKAG.h"

#include <SPI.h>
#include <Wire.h>

#include <TimerOne.h>

#include "font.h"
#include "acc_register.h"

#define RCK A3			//RCK Pin von SR
#define SRCLR A2		//''

#define PCF_ADRESS 0x38	//Portexpander Adresse


//Zwischenspeicher (Array) für die nächste Übertragung auf die Matrix
int data[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//Pin RCK, SRCLR, SS konfiguieren sobald Instanz gebildet wird
matrix_class::matrix_class()
{
	pinMode(RCK, OUTPUT);	//Register Clock high Pegel sensitive
	pinMode(SRCLR, OUTPUT);	//SRClear low Pegel sensitive
	pinMode(SS, OUTPUT);

	digitalWrite(RCK, 0);
	digitalWrite(SRCLR, 1);
	digitalWrite(SS, 1);
}

//Schlaufe die auf den Hardware Timer angebunden ist
void matrix_timer()
{
	static int zaehler;
	
	//SR Speicher Reset
	digitalWrite(SRCLR, 1);
	digitalWrite(SRCLR, 0);
	digitalWrite(SRCLR, 1);

	//Zeile 
	SPI.transfer(0x80 >> zaehler);

	//Daten aus Matrix Array rüberschiben
	SPI.transfer(data[zaehler]);

	//Puls generieren -> Schieberegister auf Output Buffer schalten
	digitalWrite(RCK, 0);
	digitalWrite(RCK, 1);
	digitalWrite(RCK, 0);

	if (zaehler == 7)
	{
		//Zaehler auf 0 stellen um erneut wieder 8 Bit zu uebetragen
		zaehler = 0;
	}
	else
	{
		//Zaehler hochzaehlen da noch nicht 8 Bit erreicht
		zaehler++;
	}
}

//Initalisierung (Timer konfiguieren, SPI initalisieren)
void matrix_class::init()
{
	Wire.begin();							//I2C Interface starten und Bus als Master betreten
	//pcf_write(0x00);						//PCF reseten

	Timer1.initialize(800);					//1000000 = 1 Sekunde - 80000 = 8ms = ca. 15.6Hz Updaterate des kompletten Bild
	Timer1.attachInterrupt(matrix_timer);	//Funktion matrix_timer bei jedem Timerueberlauf ausführen

	SPI.begin();							//SPI Starten
	SPI.setClockDivider(SPI_CLOCK_DIV128);	//16Mhz/128 = 125kHz
	SPI.setBitOrder(LSBFIRST);				//MSB zuerst senden

}

//Updateroutine für I/O Hardware an der Matrix
int matrix_class::read_io()
{
	char counter_value = 2;
	char pcf_state;

	microseconds_now = micros();			//Aktuellen us Wert speichern
	pcf_state = pcf_read();					//Aktuellen Wert aus PCF Portexpander speichern
	potentiometer_0 = 1023-(analogRead(A0));		//Potentiometer lesen und in potentiometer_0 schreiben

	//Werte der drei Tasten reseten
	taste_1 = 0;
	taste_2 = 0;
	taste_3 = 0;

	//Prüfen ob seit dem letzten mal bereits 5ms vergangen sind.
	if((microseconds_now - microseconds_saved) > 5000)
	{
		//Prüfen ob Taste gedrückt, wenn ja Zähler hochzählen, wenn nicht Zähler auf 0
		if(pcf_state & 0x01)
		{
			counter_taste_1++;
		}

		if(pcf_state & 0x02)
		{
			counter_taste_2++;
		}

		if(pcf_state & 0x04)
		{
			counter_taste_3++;
		}	

		//Aktuelle micros speichern für nächsten Durchgang
		microseconds_saved = micros();
	}

	//Wenn der Counter höher als counter_value ist & das Bit des entsprechenden Taster Low ist dann taste gedrückt worden
	if(counter_taste_1 > counter_value && (!(pcf_state & 0x01)))
	{
		taste_1 = 1;
		counter_taste_1 = 0;
	}


	if(counter_taste_2 > counter_value && (!(pcf_state & 0x02)))
	{
		taste_2 = 1;
		counter_taste_2 = 0;
	}


	if(counter_taste_3 > counter_value && (!(pcf_state & 0x04)))
	{
		taste_3 = 1;
		counter_taste_3 = 0;
	}
}

//Matrix Schreibfunktion um Daten auf die Matrix zu schreiben
void matrix_class::write(char byte1, char byte2, char byte3, char byte4, char byte5, char byte6, char byte7, char byte8)
{
	//Vom Nutzer stammende Daten Byte für Byte ins Data Array kopieren
	data[0] = byte1;
	data[1] = byte2;
	data[2] = byte3;
	data[3] = byte4;
	data[4] = byte5;
	data[5] = byte6;
	data[6] = byte7;
	data[7] = byte8;
}

//Matrix Schreibfunktion um Daten von einem Array auf die Matrix zu schreiben
void matrix_class::write_array(char matrix[])
{
	//Vom Nutzer stammende Daten Byte für Byte ins Data Array kopieren
	data[0] = matrix[0];
	data[1] = matrix[1];
	data[2] = matrix[2];
	data[3] = matrix[3];
	data[4] = matrix[4];
	data[5] = matrix[5];
	data[6] = matrix[6];
	data[7] = matrix[7];
}

//Matrix Löschfunktion
void matrix_class::clear(void)
{
	//Alle 8 Byte löschen
	for(int x = 0; x < 8; x++)
	{
		data[x] = 0;
	}
}

//Matrix Sample/Muster Funktion
void matrix_class::sample(int number)
{
	//Prüfen welches Muster, danach entsprechende Muster in Data Array kopieren
	switch(number)
	{
		case 0:
			data[0] = 0b10101010;
			data[1] = 0b01010101;
			data[2] = 0b10101010;
			data[3] = 0b01010101;
			data[4] = 0b10101010;
			data[5] = 0b01010101;
			data[6] = 0b10101010;
			data[7] = 0b01010101;
			break;
		case 1:
			data[0] = ~0b10101010;
			data[1] = ~0b01010101;
			data[2] = ~0b10101010;
			data[3] = ~0b01010101;
			data[4] = ~0b10101010;
			data[5] = ~0b01010101;
			data[6] = ~0b10101010;
			data[7] = ~0b01010101;
			break;
  }
}

//Matrix Font Funktion
void matrix_class::font_write(int numb)
{
	//Buffer um Font aus Flash in SRAM zwischenzuspeichern
	char buffer[8];

	matrix_class::write(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

	//Font Daten aus Flash Speicher holen und in Matrix Array schreiben
	for(int x = 0; x <= 7; x++)
	{
		buffer[x] = pgm_read_byte(&font[numb][x]);
	}

	for(int x = 0; x <= 7; x++)
	{
		for(int z = 0; z <= 7; z++)
		{	
			if(buffer[x] & (0x01 << z))
			{
				data[x] |= (0x80 >> z) << 1;
			}
	
		}
	}
}

//PCF Ausgabe Funktion
void matrix_class::pcf_write(int data)
{
	Wire.beginTransmission(PCF_ADRESS);

	Wire.write((~(0x07) & data ));  //Daten schreiben, hinterste 3 Bit ignorieren da daran die Taster sind. Diese Zeile kommentieren falls untere Zeile auskommentiert wird.

	//Wire.write(data);				//Diese Zeile auskommentieren falls auch die 3 "gesperrten" Bits am Ende genutzt werden wollen.

	Wire.endTransmission();

}

//PCF Eingabe Funktion
int matrix_class::pcf_read()
{
	int pcf_buffer;

	Wire.beginTransmission(PCF_ADRESS);

	Wire.requestFrom(PCF_ADRESS, 1);

	while(Wire.available() == 0);

	pcf_buffer = Wire.read();

	return pcf_buffer;
}

matrix_class matrix;

