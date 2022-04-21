/*********************************************************************************
 **
 **  Color Following Program
 **
 **  Written By:    Connor Geshan - Western New England University
 **  Written On:    March 2022
 **  Last Updated:  April 2022 - Connor Geshan - Western New England University
 **  
 **  
 **  Required Hardware:   
 **     Pixy Cam Version 2     
 **     HC-05 Bluetooth Module
 **     Adafruit Motor Shield, DC Motor w/ encoder (2)
 **     
 **
 *********************************************************************************/
 
 
#include <SoftwareSerial.h>
#include <Pixy2.h>
#include <PIDLoop.h>
#include <Wire.h>
#include <Encoder.h>
#include <Adafruit_MotorShield.h>

// Initializing Motor Shield, Motor speeds, maximum allowed is 250
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

// Initialization of PixyCam
#define X_CENTER_LINE         (pixy.frameWidth/2)
#define xCenterColor          180
Pixy2 pixy;
int8_t res, blocks;
int32_t error;
char buf[128] ;

// Color following stuff
long maxSize = 0;
int oldSignature, trackedBlock = 0;

// Creating PID Loop
PIDLoop headingLoop(5000, 0, 0, false);

// Encoder
Encoder leftEnc(19, 18);
Encoder rightEnc(2, 3);
long leftStart, rightStart;

//Bluetooth Setup
SoftwareSerial HC05(10, 11);
char input;
int jobs;
bool delivery;


void setup() {
  HC05.begin(9600);
  Serial.begin(9600);
  Serial.print("Starting...\n");
  delivery = false;

  AFMS.begin();
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  Serial.println("Motors On and Set to 0");
  delay(200);

  pixy.init();
  Serial.println("Turning on PixyCam");
  pixy.setLamp(1, 1);
  //pixy.changeProg("line");
  pixy.changeProg("color");

  // look straight and down
  pixy.setServos(550, 800);
}
void loop() {

  if (HC05.available() > 0) {
    input = HC05.read();
    Serial.print("Char input: ");
    Serial.println(input);
    jobs = input - '0';
    Serial.print("Converted Int: ");
    Serial.println(jobs);
    delay(2000);
    delivery = true;
  }

  while(delivery == true){
    blocks = pixy.ccc.getBlocks();

    //Serial.println(blocks);
    if (blocks) {
      //Serial.println("We have blocks");
      for (int i = 0; i < blocks; i++) {
        if (pixy.ccc.blocks[i].m_signature == 2) {
          //res = pixy.line.getMainFeatures();
          StationFound(jobs); //StationFound(res, jobs);
          //Try resetting these char values here...if reading error then move into Station Found function 
          buf[128] = '0';
        } else if (pixy.ccc.blocks[i].m_signature == 1) {
          long newSize = pixy.ccc.blocks[i].m_height * pixy.ccc.blocks[i].m_width;
          if (newSize > maxSize) {
            int trackedBlock = i;
            maxSize = newSize;
          }
          int xCoord = pixy.ccc.blocks[trackedBlock].m_x;
          int yCoord = pixy.ccc.blocks[trackedBlock].m_y;
          FollowLine(xCoord, yCoord);
        } else if (pixy.ccc.blocks[i].m_signature == 3) {

          pixy.changeProg("line");

          completeDelivery();
          returnToPath(leftStart, rightStart);
          delivery = false;
        }
      }
    } else { 
      Serial.println("No Blocks :(");
      leftMotor -> run(RELEASE);
      rightMotor -> run(RELEASE);
    }
  //} else {
//    leftMotor -> run(RELEASE);
//    rightMotor -> run(RELEASE);
  }
}


/*
 * User Defined Functions Below
 * ==========================================================================================================
 */

int8_t StationFound(int jobs) { //StationFound(int8_t res, int jobs)

  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  pixy.changeProg("line");
  //Serial.println("Station Mode");
  delay(100);
  //Serial.println(res);

  leftStart = abs(leftEnc.read());
  rightStart = abs(rightEnc.read());
  Serial.println("Station Mode:");
//  Serial.println(leftStart);
//  Serial.println(rightStart);

  long leftPos = abs(leftEnc.read());
  long rightPos = abs(rightEnc.read());
  //  Serial.print("Left: ");
  //  Serial.println(leftPos);
  //  Serial.print("Right: ");
  //  Serial.println(rightPos);
  delay(100);

  // Crawl Forward 2 inches
  crawl(leftPos, rightPos);
  delay(500);
  res = pixy.line.getAllFeatures();
  pixy.line.barcodes -> print();
  // Serial.println(pixy.line.barcodes -> m_code);
  if (LINE_BARCODE) {
    if (pixy.line.barcodes->m_code == jobs) { //change to jobs later
      Serial.print("Found Station ");
      Serial.println(jobs);
      //Turn 90 degrees to the right
      long leftTargetPos = leftPos + 10000;
      while (leftPos < leftTargetPos) {
        leftMotor -> run(FORWARD);
        leftMotor -> setSpeed(50);
        leftPos = leftEnc.read();
      }
      leftMotor -> run(RELEASE);
      //        Serial.print("Left: ");
      //        Serial.println(leftPos);
      //        Serial.print("Right: ");
      //        Serial.println(rightPos);
      delay(100);
    }
//    buf[96] = '0';
//    buf[128] = '0';
  } else {
    leftMotor -> run(RELEASE);
    rightMotor -> run(RELEASE);
  }
  
  pixy.changeProg("color");

}


void crawl(long leftPos, long rightPos) {
  //Crawl Forward 2 inches due to camera tilt error
  delay(250);
  long rightAdjust = rightPos + 3000;
  long leftAdjust = leftPos + 3000;
  while (leftPos <= leftAdjust && rightPos <= rightAdjust) {
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(50);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(50);
    leftPos = abs(leftEnc.read());
    rightPos = abs(rightEnc.read());
    //    Serial.print("Left: ");
    //    Serial.println(leftPos);
    //    Serial.print("Right: ");
    //    Serial.println(rightPos);
  }
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  //  Serial.print("Left: ");
  //  Serial.println(leftPos);
  //  Serial.print("Right: ");
  //  Serial.println(rightPos);
  delay(100);
}

void completeDelivery() {
  Serial.println("Pick Up");
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  //delay(10000);
  delay(2500);

}

void returnToPath(long leftStart, long rightStart) {
  Serial.println("BACK UP");
  delay(100);

//  Serial.print("Right Start: ");
//  Serial.println(leftStart);
//  Serial.print("Right Start: ");
//  Serial.println(rightStart);
  delay(100);

  long leftPos = abs(leftEnc.read());
  long rightPos = abs(rightEnc.read());
//  Serial.print("Left Current: ");
//  Serial.println(leftPos);
//  Serial.print("Right Current: ");
//  Serial.println(rightPos);
  delay(100);

  while (leftPos >= leftStart && rightPos >= rightStart) {
    leftMotor -> run(BACKWARD);
    leftMotor -> setSpeed(50);
    rightMotor -> run(BACKWARD);
    rightMotor -> setSpeed(50);
    leftPos = abs(leftEnc.read());
    rightPos = abs(rightEnc.read());
    //    Serial.print("Left: ");
    //    Serial.println(leftPos);
    //    Serial.print("Right: ");
    //    Serial.println(rightPos);
  }
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  delay(100);

  long rightAdjust = rightPos + (abs(leftEnc.read() - abs(leftStart))) -500 ;
//  Serial.print("Right Current: ");
//  Serial.println(rightPos);
  Serial.print("Right Adjust: ");
  Serial.println(rightAdjust);
  delay(100);
  while (rightPos <= rightAdjust) {
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(50);
    rightPos = abs(rightEnc.read());
    //    Serial.print("Left: ");
    //    Serial.println(leftPos);
    Serial.print("Right: ");
    Serial.println(rightPos);
  }
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE); 
  HC05.print("Delivered");
  delay(100);
  Serial.println("Sent to Computer");
  delay(100);   
}

int FollowLine(int xCoord, int yCoord) {
  int left, right;
  //Serial.println("Follow Track");
  if (xCoord < (xCenterColor - 10)) {
    left = 25;
    right = 50;
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(left);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(right);
    //Serial.println("Right1");
  } else if (xCoord > (xCenterColor + 10)) {
    //Serial.println("Left1");
    left = 50;
    right = 25;
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(left);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(right);
  } else if (xCoord < (xCenterColor - 100)) {
    left = 25;
    right = 50;
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(left);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(right);
   // Serial.println("Right2");
  } else if (xCoord > (xCenterColor + 100)) {
   // Serial.println("Left2");
    left = 50;
    right = 25;
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(left);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(right);
  } else {
    left = 50;
    right = 50;
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(left);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(right);
   // Serial.println("Straight");
  }
}
