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
Encoder leftEnc(3, 2);
Encoder rightEnc(19, 18);
long leftStart, rightStart, leftPos, rightPos;

//Bluetooth Setup
SoftwareSerial HC05(10, 11);
char input;
int jobs;
bool delivery;

int greenLED = 24;
int yellowLED = 25;
int redLED = 26;


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

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  
  pixy.init();
  Serial.println("Turning on PixyCam");
  pixy.setLamp(1, 1);
  //pixy.changeProg("line");
  pixy.changeProg("color");

  // look straight and down
  pixy.setServos(850, 700);
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

  while (delivery == true) {
    blocks = pixy.ccc.getBlocks();

    //Serial.println(blocks);
    if (blocks) {
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, LOW);
      //Serial.println("We have blocks");
      for (int i = 0; i < blocks; i++) {
        if (pixy.ccc.blocks[i].m_signature == 2) {
          //res = pixy.line.getMainFeatures();
          leftStart = 0;
          rightStart = 0;
          leftPos = 0;
          rightPos = 0;
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
      leftMotor -> run(RELEASE);
      rightMotor -> run(RELEASE);
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, HIGH);
    }
  }
}


/*
   User Defined Functions Below
   ==========================================================================================================
*/

int8_t StationFound(int jobs) { //StationFound(int8_t res, int jobs)
  leftMotor -> run(FORWARD);
  leftMotor -> setSpeed(50);
  rightMotor -> run(FORWARD);
  rightMotor -> setSpeed(50);
  delay(10);
  for (int s = 50; s > 0; s--) {
    leftMotor -> run(s);
    rightMotor -> run(s);
  }
  pixy.setLamp(0, 0);
  pixy.setServos(850, 600);
  leftMotor -> run(0);
  rightMotor -> run(0);
  pixy.changeProg("line");
  //Serial.println("Station Mode");
  delay(100);
  //Serial.println(res);
  if (jobs == 0) {
    jobs = 15;
  } else {
    jobs = jobs;
  }
  Serial.println(jobs);
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

    if (pixy.line.barcodes->m_code == 15) {
      HC05.print("Hub");
      leftMotor -> run(RELEASE);
      rightMotor -> run(RELEASE);
      delay(500);
      long leftTargetPos = leftPos + 9750;
      while (leftPos < leftTargetPos) {
        leftMotor -> run(FORWARD);
        leftMotor -> setSpeed(50);
        leftPos = leftEnc.read();
      }
      leftMotor -> run(RELEASE);
      // Serial.print("Left: ");
      // Serial.println(leftPos);
      // Serial.print("Right: ");
      // Serial.println(rightPos);
      delay(100);
      reset(leftPos, rightPos);
      leftTargetPos = leftPos + 8750;
      while (leftPos < leftTargetPos) {
        leftMotor -> run(FORWARD);
        leftMotor -> setSpeed(50);
        leftPos = leftEnc.read();
      }
      leftMotor -> run(RELEASE);
      // Serial.print("Left: ");
      // Serial.println(leftPos);
      // Serial.print("Right: ");
      // Serial.println(rightPos);
      delay(100);
      // stationCount = 0;
      jobs = 0;
      pixy.changeProg("line");
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, LOW);
      delivery = false;
    }
    else if (pixy.line.barcodes->m_code == jobs) { //change to jobs later
      Serial.print("Found Station ");
      Serial.println(jobs);
      // Turn 90 degrees to the right
      long leftTargetPos = leftPos + 9750;
      while (leftPos < leftTargetPos) {
        leftMotor -> run(FORWARD);
        leftMotor -> setSpeed(50);
        leftPos = leftEnc.read();
      }
      leftMotor -> run(RELEASE);
      // Serial.print("Left: ");
      // Serial.println(leftPos);
      // Serial.print("Right: ");
      // Serial.println(rightPos);
      delay(100);
    } //else { // Possible Implementation for passing station, tertieary demand
//      String statusUpdate = "Passing Station " + pixy.line.barcodes->m_code;
//      HC05.print(statusUpdate);
//      delay(100);
//    }
  } else {
    leftMotor -> run(RELEASE);
    rightMotor -> run(RELEASE);
  }
  pixy.setLamp(1, 1);
  pixy.setServos(850, 700);
  pixy.changeProg("color");
}

void crawl(long leftPos, long rightPos) {
  //Crawl Forward 2 inches due to camera tilt error
  delay(250);
  long rightAdjust = rightPos + 2500;
  long leftAdjust = leftPos + 2500;
  while (leftPos <= leftAdjust && rightPos <= rightAdjust) {
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(50);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(50);
    leftPos = abs(leftEnc.read());
    rightPos = abs(rightEnc.read());
    // Serial.print("Left: ");
    // Serial.println(leftPos);
    // Serial.print("Right: ");
    // Serial.println(rightPos);
  }
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  // Serial.print("Left: ");
  // Serial.println(leftPos);
  // Serial.print("Right: ");
  // Serial.println(rightPos);
  delay(100);
}

void completeDelivery() {
  Serial.println("Pick Up");
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  for(int i = 0; i < 10; i++){
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(500);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    delay(500);
  }
}

void returnToPath(long leftStart, long rightStart) {
  Serial.println("BACK UP");
  delay(100);

  // Serial.print("Right Start: ");
  // Serial.println(leftStart);
  // Serial.print("Right Start: ");
  // Serial.println(rightStart);
  // delay(100);

  long leftPos = abs(leftEnc.read());
  long rightPos = abs(rightEnc.read());
  // Serial.print("Left Current: ");
  // Serial.println(leftPos);
  // Serial.print("Right Current: ");
  // Serial.println(rightPos);
  delay(100);

  while (leftPos >= (leftStart - 750) && rightPos >= (rightStart - 750)) {
    leftMotor -> run(BACKWARD);
    leftMotor -> setSpeed(50);
    rightMotor -> run(BACKWARD);
    rightMotor -> setSpeed(50);
    leftPos = abs(leftEnc.read());
    rightPos = abs(rightEnc.read());
    // Serial.print("Left: ");
    // Serial.println(leftPos);
    // Serial.print("Right: ");
    // Serial.println(rightPos);
  }
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  delay(100);

  long rightAdjust = rightPos + (abs(leftEnc.read() - abs(leftStart)) - 500);
  // Serial.print("Right Current: ");
  // Serial.println(rightPos);
  // Serial.print("Right Adjust: ");
  // Serial.println(rightAdjust);
  delay(100);
  while (rightPos <= rightAdjust) {
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(50);
    rightPos = abs(rightEnc.read());
    // Serial.print("Left: ");
    // Serial.println(leftPos);
    // Serial.print("Right: ");
    // Serial.println(rightPos);
  }
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  HC05.print("Delivered");
  delay(100);
  Serial.println("Sent to Computer");
}

int FollowLine(int xCoord, int yCoord) {
  int left, right;
  //Serial.println("Follow Track");
  if (xCoord < (xCenterColor - 5)) {
    left = 25;
    right = 50;
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(left);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(right);
    //Serial.println("Right1");
  } else if (xCoord > (xCenterColor + 5)) {
    //Serial.println("Left1");
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

void reset(long leftPos, long rightPos) {
  //Crawl Forward 2 inches due to camera tilt error
  delay(250);
  long rightAdjust = rightPos + 10000;
  long leftAdjust = leftPos + 10000;
  while (leftPos <= leftAdjust && rightPos <= rightAdjust) {
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(50);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(50);
    leftPos = abs(leftEnc.read());
    rightPos = abs(rightEnc.read());
    // Serial.print("Left: ");
    // Serial.println(leftPos);
    // Serial.print("Right: ");
    // Serial.println(rightPos);
  }
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  // Serial.print("Left: ");
  // Serial.println(leftPos);
  // Serial.print("Right: ");
  // Serial.println(rightPos);
  delay(100);
}
