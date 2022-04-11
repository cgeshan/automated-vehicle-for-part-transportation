#include <Pixy2.h>
#include <PIDLoop.h>
#include <Wire.h>
#include <Encoder.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

// Initializing Motor Shield, Motor speeds, maximum allowed is 250
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);


// HC-05 Module
SoftwareSerial HC05(10,11);
char jobs; 

void setup(){
  HC05.begin(115200);
  
  Serial.begin(115200);
  Serial.print("Starting...\n");

  AFMS.begin();
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  Serial.println("Motors On and Set to 0");
  delay(2000);
}


 
void loop(){

  if(HC05.available() > 0){
    Serial.println("Connection Successful!");
    jobs = HC05.read();
    delay(20);
    if (jobs > 0){
      Serial.println("Processing Jobs...");
      Serial.print("Job: Delivery required to station ");
      Serial.println(jobs);
      delay(5000);
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(50);       
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(50);
      delay(2000);
      leftMotor -> run(RELEASE);
      rightMotor -> run(RELEASE);

      HC05.println("Delivered");
      jobs = 0;
    }else{
        leftMotor -> run(RELEASE);
        rightMotor -> run(RELEASE);
     }        
  }else{
    leftMotor -> run(RELEASE);
    rightMotor -> run(RELEASE);
  }
}
