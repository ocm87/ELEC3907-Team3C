#include <Wire.h>
#include <AHT20.h>
AHT20 aht20;
  int sensorValue = 0;
  float temperature = aht20.getTemperature();

unsigned long time_now= 0;
unsigned long time_now2= 0;
unsigned long time_now3= 0;
const int PIN_RED   = A1;
const int PIN_GREEN = A2;
const int PIN_BLUE  = A3;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
//aht20
Serial.println("Humidity AHT20 examples");

  Wire.begin(); //Join I2C bus
  //Check if the AHT20 will acknowledge
  if (aht20.begin() == false)
  {
    Serial.println("AHT20 not detected. Please check wiring. Freezing.");
    while (1);
  }
  Serial.println("AHT20 acknowledged.");
  
//LED
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
}

void loop() {
 
if(millis()>=time_now + 2000){  // 1000 will be changed based on delay we want
    time_now= millis();
    // C02 Sensor
      // read the input on analog pin 0:
     
      int sensorValuePrecal = analogRead(A0);
    //  int sensorValue = 0;
      sensorValue = sensorValuePrecal* 7;
  Serial.println("The amount of CO2 (in PPM): ");
      Serial.println(sensorValue);
  //AHT20
  //If a new measurement is available
  if (aht20.available() == true)
  {
    //Get the new temperature and humidity value
  //  float temperature = aht20.getTemperature();
   float humidity = aht20.getHumidity();

 temperature = aht20.getTemperature();

    //Print the results
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" C\t");
    Serial.print("Humidity: ");
    Serial.print(humidity, 2);
    Serial.print("% RH");

    Serial.println();
  }
  }



//LED
//if(millis()>=time_now2 + 1000){  // 1000 will be changed based on delay we want
 //   time_now3 = millis();
    
  if(sensorValue >1000 ){//1000 if co2 gas is too high
    analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 255);
    analogWrite(PIN_BLUE,  255);
    
delay(5000);
    }

 
  if(temperature>40){ //40 if temp to high
     analogWrite(PIN_RED,   255);
    analogWrite(PIN_GREEN, 255);
    analogWrite(PIN_BLUE,  0);
    
delay(5000);
 
  }
    }
//}
