// Load Wi-Fi library
#include <WiFiEspAT.h>

#define AT_BAUD_RATE 115200

// Replace with your network credentials
const char *ssid     = "O’s iPhone";
const char *password = "110covered";

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

//Button setup
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
const int buttonPin = 2;    // the number of the pushbutton pin
//int buttonState = LOW;            // the current reading from the input pin
int lastButtonState = HIGH;

volatile bool btnInterrupt = false;



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
}

void loop(){

  if (btnInterrupt) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      lastDebounceTime = millis();
      toggleSOS();
      delayMicroseconds(debounceDelay);
      btnInterrupt = false;
    }
  }
  // Button needs constant polling so it must be in the regular loop
  /*
  int reading = digitalRead(buttonPin);
  
  //Serial.println(reading); // read button state
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != buttonState) {
      // only toggle the LED if the new button state is HIGH
      if (reading == HIGH) {
        toggleSOS();
      }
    }
  }
  */
  Serial.println("SOS state is:");
  Serial.println(SOS);
  // uncomment these for viewing the sos status
  //Serial.println("SOS Status");
  //Serial.println(SOS);
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

            if (header.indexOf("GET") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("\r\n\r\n");
              client.println("Content-type: text/plain"); 
              client.println(response);           
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
