
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
int16_t accelData[6];
#define LED_PIN 13
int16_t avgData;
bool blinkState = false;

void setup() {
    Wire.begin();
    Serial.begin(38400);
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    accelgyro.setFullScaleAccelRange(3);
    accelgyro.setFullScaleGyroRange(3);
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    pinMode(LED_PIN, OUTPUT);
}

int16_t getData(){
        int16_t AccelAx=ax;
        int16_t AccelAy=ay;
        int16_t AccelAz=az;
        int16_t AccelGx=gx;
        int16_t AccelGy=gy;
        int16_t AccelGz=gz;
    
        accelData[0]=AccelAx;
        accelData[1]=AccelAy;
        accelData[2]=AccelAz;
        accelData[3]=AccelGx;
        accelData[4]=AccelGy;
        accelData[5]=AccelGz;
        
        return accelData;
}

boolean falldetect(int16_t avgData){
    if(avgData>20000){
      return true;
    }
}

void loop() {
 
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Serial.print("a/g/m:\t");
    avgData=sqrt(pow(ax,2)+pow(ay,2)+pow(az,2)+pow(gx,2)+pow(gy,2)+pow(gz,2)+pow(mx,2)+pow(my,2)+pow(mz,2));
    Serial.println(avgData); Serial.print("\t");
    /*Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz); Serial.print("\t");
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.println(mz);*/
    getData();
    falldetect(avgData);
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
}
