// Load Wi-Fi library
#include <WiFiEspAT.h>
//Load json library
#include <ArduinoJson.h>
//Wire library for I2C
#include <Wire.h>
//AHT20 library
#include <AHT20.h>
//SPI library
#include <SPI.h>
//SD library
#include <SD.h>
//BMP library
#include <BMP180I2C.h>
//GPS Libs
#include <TinyGPS++.h>


#define AT_BAUD_RATE 115200
#define CS_PIN 53

// Replace with your network credentials
const char *ssid     = "SSID";
const char *password = "passwd";

// ESP8266 Hostname
const char *hostname = "BLACK_BOX";
// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
const char *response = "{data: value, temp: value}"; // test plain text response

// Global SOS var
bool SOS = false;
const int SOS_LED = 7;

//Button setup
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 500;    // the debounce time; increase if the output flickers
const int buttonPin = 2;    // the number of the pushbutton pin
//int buttonState = LOW;            // the current reading from the input pin
int lastButtonState = HIGH;

volatile bool btnInterrupt = false;

// json doc setup
DynamicJsonDocument doc(1024);

//AHT20 Setup
AHT20 aht20;

//BMP180 setup
#define BMP_I2C_ADDRESS 0x77
BMP180I2C bmp180(BMP_I2C_ADDRESS);
unsigned long lastPressure = 0;
unsigned long pressureReadDelay = 100;


// SD Card setup
File myFile;
unsigned long writeDelay = 1000; // Wait 1000ms between writes to the SD Card
unsigned long lastWriteTime = 0;
String sensorFile = "SENSORS.TXT";

// Gas Sensor setup
#define GAS_PIN A0
unsigned long lastGasRead = 0;
unsigned long gasReadDelay = 500;

// GPS Setup
unsigned long lastGPSRead = 0;
unsigned long GPSReadDelay = 1000;
static const int RXPin = 19, TXPin = 18;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps; // TinyGPS object
//SoftwareSerial ss(RXPin, TXPin); //software serial objects


void setup() {
  // button setup
  pinMode(buttonPin, INPUT);
  attachInterrupt(0, buttonInterrupt, RISING);

  Serial.begin(115200);
  while(!Serial);

  Serial3.begin(AT_BAUD_RATE);
  WiFi.init(Serial3);

  // Connect to Wi-Fi network with SSID and password
  //Serial.println("Connecting to ");
  //Serial.println(ssid);
  // Set Hostname
  WiFi.setHostname(hostname);
  connectToWiFi();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("The Setup is complete, starting loop");

  Serial.println(WiFi.hostname());

  //json init
  doc["SOS"] = SOS;
  doc["temp"] = 0;
  doc["humid"] = 0;
  doc["pressure"] = 0;
  doc["CO2"] = 0;
  doc["latitude"] = 45.384915;
  doc["longitude"] = -75.697438;

  // AHT20 Setup
  Serial.println("Setting up AHT20:");
  Wire.begin(); //Join I2C bus
  //Check if the AHT20 will acknowledge
  if (aht20.begin() == false)
  {
    Serial.println("AHT20 not detected. Please check wiring. Freezing.");
    while (1);
  }
  Serial.println("AHT20 acknowledged.");

  // SD Card Setup
  Serial.print("Initializing SD card...");
  
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  if (!SD.begin(53)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialization done.");
  SD.remove(sensorFile);
  
  
  // BMP180 Setup
  if (!bmp180.begin())
	{
		Serial.println("Initialization failed check your BMP180 Interface and I2C Address.");
		while (1);
	}
	bmp180.resetToDefaults(); //reset sensor to default parameters
	bmp180.setSamplingMode(BMP180MI::MODE_UHR); //enable ultra high resolution mode for pressure measurements

  // Gas Sensor Setup
  pinMode(GAS_PIN, INPUT);

  // GPS Setup
  //ss.begin(GPSBaud);
  Serial2.begin(GPSBaud);
  while (!Serial2);

  // SOS LED
  pinMode(SOS_LED, OUTPUT);
}

void loop(){
  // Button Interrrupt
  if (btnInterrupt) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      lastDebounceTime = millis();
      toggleSOS();
      delayMicroseconds(debounceDelay);

      btnInterrupt = false;
    }
  }

  
  // Update json doc with sensor readings
  doc["SOS"] = SOS;
  
  updateSOSLED();

  // read temp and humidity values
  readTempHumidity(); // updates JSON doc as well

  readPressure(); // updates JSON doc with pressure

  readCO2(); // updates JSON doc with CO2 in ppm

  readGPS();

  //Write to the SD Card
  char buffer[200];
  serializeJson(doc, buffer);
  //Serial.println(buffer);
  if (WiFi.status() != WL_CONNECTED) {
    delayedWriteSD(sensorFile, buffer);
  }

  // Write JSON DOC to Micro SD Card
  // Should have filename, string as inputs
  // Should just open the files and write the string to the card

  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:

            if (header.indexOf("PUT") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("\r\n\r\n");
              client.println("Content-type: text/plain"); 
              client.println("request ok");           
              client.println("Connection: close");
              client.println();
            } else if (header.indexOf("GET /sensor/all") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("\r\n\r\n");
              client.println("Content-type: text/plain"); 
              serializeJson(doc, client);
              client.println("\n");        
              client.println("Connection: close");
              client.println();
            } else if (header.indexOf("POST /sos/on") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("\r\n\r\n");
              client.println("SOS true");
              client.println("Connection: close");
              client.println();
              Serial.println("SOS Var true");
              SOS = true;
            } else if (header.indexOf("POST /sos/off") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("\r\n\r\n");
              client.println("SOS false");
              client.println("Connection: close");
              client.println();
              Serial.println("SOS Var false");
              SOS = false;              
            } else {
              client.println("HTTP/1.1 400 OK");
              client.println("Connection: close");
              client.println();
            }
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void buttonInterrupt () {
  btnInterrupt = true;
}

void toggleSOS () {
  if (SOS == true) {
    SOS = false;
  }
  else {
    SOS = true;
  }
  Serial.println("SOS Status:");
  Serial.println(SOS);
}

void connectToWiFi() {
// Connect to WiFi Network
   Serial.println();
   Serial.println();
   Serial.print("Connecting to WiFi");
   Serial.println("...");
   WiFi.begin(ssid, password);
   int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 15)) {
   retries++;
   delay(500);
   Serial.print(".");
  }
  if (retries > 14) {
    Serial.println(F("WiFi connection FAILED"));
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected!"));
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
    Serial.println(F("Setup ready"));
}

void readTempHumidity() { 
  if (aht20.available() == true)
  {
    //Get the new temperature and humidity value
    float temperature = aht20.getTemperature();
    float humidity = aht20.getHumidity();
    doc["temp"] = temperature;
    doc["humid"] = humidity;
  }
}

void readPressure () { 
  currentTime = millis();
  if ((currentTime - lastPressure) > pressureReadDelay) {
    if (!bmp180.measureTemperature()) {
		  Serial.println("could not start temperature measurement, is a measurement already running?");
		  return;
	  }
    //wait for the measurement to finish. proceed as soon as hasValue() returned true. 
    do {
      delay(10);
    } while (!bmp180.hasValue());

    bmp180.getTemperature(); 
    //start a pressure measurement. pressure measurements depend on temperature measurement
    //only start pressure right after temp.
    //measurement immediately after a temperature measurement. 
    if (!bmp180.measurePressure())
    {
      Serial.println("could not start perssure measurement, is a measurement already running?");
      return;
    }
    //wait for the measurement to finish. proceed as soon as hasValue() returned true. 
    do {
      delay(10);
    } while (!bmp180.hasValue());
    doc["pressure"] = bmp180.getPressure();
    lastPressure = millis();
  }
}

void writeSD (String FileName, String Cont) {
    myFile=SD.open(FileName, FILE_WRITE);
    if (myFile) {
      myFile.println(Cont);
      myFile.close();
    } else {
      Serial.println("file not open");      
    }
}

void delayedWriteSD (String filename, String cont) {
  currentTime = millis();
  if (currentTime - lastWriteTime > writeDelay) {
    lastWriteTime = currentTime;
    writeSD(filename, cont);
    Serial.println("Write SD:");
    Serial.println(cont); //comment out if needed
  }
}

void readSD (String FileName) {
  myFile=SD.open(FileName);
  while (myFile.available()) {
      Serial.write(myFile.read());
    }
  myFile.close();
}

void readCO2 () {
  currentTime = millis();
  if ((currentTime - lastGasRead) > gasReadDelay) {
    doc["CO2"] = analogRead(GAS_PIN);
    lastGasRead = millis();
  }
}

void readGPS () {
  currentTime = millis();
  if ((currentTime - lastGPSRead) > GPSReadDelay) {
    gps.encode(Serial2.read());
    if (gps.location.isValid()) {
      doc["latitude"] = gps.location.lat();
      doc["longitude"] = gps.location.lng();
      lastGPSRead = millis();
    }
  }
}

void updateSOSLED() {
  if (SOS == true) {
    digitalWrite(SOS_LED, HIGH);
  }
  else
    digitalWrite(SOS_LED, LOW);
}