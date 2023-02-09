// Load Wi-Fi library
#include <WiFiEspAT.h>
#include <ArduinoJson.h>

#define AT_BAUD_RATE 115200

// Replace with your network credentials
const char *ssid     = "O’s iPhone";
const char *password = "110covered";


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output13State = "off";

// Assign output variables to GPIO pins
const int output13 = 13;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial3.begin(AT_BAUD_RATE);
  WiFi.init(Serial3);

  // Initialize the output variables as outputs
  pinMode(output13, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output13, LOW);

  // Connect to Wi-Fi network with SSID and password
  //Serial.println("Connecting to ");
  //Serial.println(ssid);
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

  
}

void loop(){
  DynamicJsonDocument sensorData(1024);
  sensorData["temperature"] = 25;
  sensorData["pressure"] = 1000;
  
  String jsonData;
  serializeJson(sensorData, jsonData);
  int contentLength = jsonData.length();
  //Serial.println(contentLength);
  String contentLengthString = "Content Length: " + String(contentLength, 10);
  //Serial.println(contentLengthString);

  DynamicJsonDocument response(1024);
  response["success"] = "true";
  response["payload"] = jsonData;
  String responseData;
  serializeJson(response, responseData);
  //int responseLength = responseData.length();
  //Serial.println(responseLength);
  //String contentLengthString = "Content Length: " + String(contentLength, 10);
  //Serial.println(contentLengthString);



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
            client.println("HTTP/1.1 200 OK");
            client.println("\r\n\r\n");
            client.println("Content-type: application/json");
            client.println(responseData);
            //client.println(contentLengthString);
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              output13State = "on";
              digitalWrite(output13, HIGH);
            } else if (header.indexOf("GET /13/off") >= 0) {
              Serial.println("GPIO 13 off");
              output13State = "off";
              digitalWrite(output13, LOW);
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

void connectToWiFi() {
//Connect to WiFi Network
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
