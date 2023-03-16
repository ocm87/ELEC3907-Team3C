#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

 // takes in filename, str
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}
void writeSD(String FileName, String Cont){
    myFile=SD.open(FileName, FILE_WRITE);
    myFile.println(Cont);
    myFile.close();
  }

void readSD(String FileName){
  myFile=SD.open(FileName);
  while (myFile.available()) {
      Serial.write(myFile.read());
    }
  myFile.close();
}

void loop() {
  SD.remove("A");
  Write("A","nice");
  Read("A");
  delay(1000);
}
