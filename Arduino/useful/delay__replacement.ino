unsigned long time_now= 0;
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis()>=time_now + 1000){  // 1000 will be changed based on delay we want
    time_now= millis();
  }
  }


// the if statement will replace delay() will have to copy code each time 
