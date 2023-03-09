// BMP180_I2C.ino
//
// shows how to use the BMP180MI library with the sensor connected using I2C.
//
// Copyright (c) 2018 Gregor Christandl
//
// connect the BMP180 to the Arduino like this:
// Arduino - BMC180
// 5V ------ VCC
// GND ----- GND
// SDA ----- SDA
// SCL ----- SCL

#include <Arduino.h>
#include <Wire.h>

#include <BMP180I2C.h>

#define I2C_ADDRESS 0x77

//create an BMP180 object using the I2C interface
BMP180I2C bmp180(I2C_ADDRESS);
unsigned long time_now= 0; 
float pressure =0;

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);

	//wait for serial connection to open (only necessary on some boards)
	while (!Serial);

	Wire.begin();

	//begin() initializes the interface, checks the sensor ID and reads the calibration parameters.  
	if (!bmp180.begin())
	{
		Serial.println("begin() failed. check your BMP180 Interface and I2C Address.");
		while (1);
	}

	//reset sensor to default parameters.
	bmp180.resetToDefaults();

	//enable ultra high resolution mode for pressure measurements
	bmp180.setSamplingMode(BMP180MI::MODE_UHR);
}

void loop() {
 

	//start a pressure measurement. pressure measurements depend on temperature measurement, you should only start a pressure 
	//measurement immediately after a temperature measurement. 
	if (!bmp180.measurePressure())
	{
		Serial.println("could not start perssure measurement, is a measurement already running?");
		return;
	}

	//wait for the measurement to finish. proceed as soon as hasValue() returned true. 
	do
	{
		if(millis()>=time_now + 1000){  // 1000 will be changed based on dealy we want
    time_now+= millis();
    }
	} while (!bmp180.hasValue());

	Serial.print("Pressure: "); 
	Serial.print(bmp180.getPressure());
	Serial.println(" Pa");
  pressure = bmp180.getPressure();
}
