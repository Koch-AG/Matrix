/**
*   @file   ACC_DEMO.ino
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   11.04.2016
*   @brief  
*   @bug    No known bugs.
*
*/

#include "Matrix_RKAG.h"

#include "SPI.h"
#include "Wire.h"

//Variable for the left and right move
int move = 0;

void setup()
{
		//Initalisierung der Matrix HW Komponenten
		matrix.init();
}

void loop()
{
	matrix.read_io();

	move = ((matrix.accelerometer_x + 1) * 4);
	
	if((matrix.accelerometer_y >= 0.8) && (matrix.accelerometer_y <= 0.95))
	{
		matrix.write((0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	}
	if((matrix.accelerometer_y >= 0.5) && (matrix.accelerometer_y <= 0.7))
	{
		matrix.write(0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	}
	if((matrix.accelerometer_y >= 0.2) && (matrix.accelerometer_y <= 0.4))
	{
		matrix.write(0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00);
	}
	if((matrix.accelerometer_y >= 0) && (matrix.accelerometer_y <= 0.2))
	{
		matrix.write(0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00);
	}
       
	if((matrix.accelerometer_y >= -0.2) && (matrix.accelerometer_y <= -0))
	{
		matrix.write(0x00, 0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00);
	}
	if((matrix.accelerometer_y >=  -0.4) && (matrix.accelerometer_y <= -0.2))
	{
		matrix.write(0x00, 0x0, 0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00);
	}
	if((matrix.accelerometer_y >= -0.7) && (matrix.accelerometer_y <= -0.5))
	{
		matrix.write(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (0x80 >> move), 0x00);
	}
	if((matrix.accelerometer_y >= -0.95) && (matrix.accelerometer_y <= -0.8))
	{
		matrix.write(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (0x80 >> move));
	}         
	    
	//20ms warten
	delay(10);
}
