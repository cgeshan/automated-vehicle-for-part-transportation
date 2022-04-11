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

// Initialization of PixyCam
#define X_CENTER_LINE         (pixy.frameWidth/2)
#define xCenterColor          180
Pixy2 pixy;

// Color following stuff
long maxSize = 0;
int oldSignature, trackedBlock = 0;

// Creating PID Loop
PIDLoop headingLoop(5000, 0, 0, false);

// Encoder 
Encoder leftEnc(19,18);
Encoder rightEnc(2,3);

// HC-05 Module
SoftwareSerial HC05(10,11);
char jobs;

/*
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 */
 

void setup(){
  HC05.begin(115200);
  
  Serial.begin(115200);
  Serial.print("Starting...\n");

  AFMS.begin();
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  Serial.println("Motors On and Set to 0");
  delay(2000);
  
  pixy.init();
//  pixy.setLamp(1, 1);  
//  pixy.changeProg("line");
//  pixy.changeProg("color");

//  look straight and down
//  pixy.setServos(550, 600);
}

/*
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 */
 
void loop(){
  
  int8_t res, blocks;
  int32_t error;
  char buf[96];

  if(HC05.available() > 0){
    Serial.println("Connection Successful!");
    jobs = HC05.read();
    delay(20);
    if (jobs > 0){
      Serial.println("Processing Jobs...");
      Serial.print("Job: Delivery required to station ");
      Serial.println(jobs);
      delay(5000);

        pixy.setLamp(1, 1);  
      //  pixy.changeProg("line");
        pixy.changeProg("color");
      
      //  look straight and down
        pixy.setServos(550, 600);

              
        blocks = pixy.ccc.getBlocks();
        //Serial.println(blocks);
        if(blocks){
          //Serial.println("We have blocks");
          for (int i = 0; i < blocks; i++){
            if (pixy.ccc.blocks[i].m_signature == 2){
              res = pixy.line.getMainFeatures();
              StationFound(res, jobs);  
            }else if(pixy.ccc.blocks[i].m_signature == 1){
              long newSize = pixy.ccc.blocks[i].m_height * pixy.ccc.blocks[i].m_width;
              if (newSize > maxSize){
                int trackedBlock = i;
                maxSize = newSize;
              }
              int xCoord = pixy.ccc.blocks[trackedBlock].m_x;
              int yCoord = pixy.ccc.blocks[trackedBlock].m_y;
              FollowLine(xCoord, yCoord);
            } 
          }
        }else{
          Serial.println("No Blocks :(");
          leftMotor -> run(RELEASE);
          rightMotor -> run(RELEASE);
        }        
    } else{
        leftMotor -> run(RELEASE);
        rightMotor -> run(RELEASE);
    }
  }else{
    leftMotor -> run(RELEASE);
    rightMotor -> run(RELEASE);
  }
//
//  blocks = pixy.ccc.getBlocks();
//  //Serial.println(blocks);
//  if(blocks){
//    //Serial.println("We have blocks");
//    for (int i = 0; i < blocks; i++){
//      if (pixy.ccc.blocks[i].m_signature == 2){
//        res = pixy.line.getMainFeatures();
//        StationFound(res);  
//      }else if(pixy.ccc.blocks[i].m_signature == 1){
//        long newSize = pixy.ccc.blocks[i].m_height * pixy.ccc.blocks[i].m_width;
//        if (newSize > maxSize){
//          int trackedBlock = i;
//          maxSize = newSize;
//        }
//        int xCoord = pixy.ccc.blocks[trackedBlock].m_x;
//        int yCoord = pixy.ccc.blocks[trackedBlock].m_y;
//        FollowLine(xCoord, yCoord);
//      } 
//    }
//  }else{
//    Serial.println("No Blocks :(");
//    leftMotor -> run(RELEASE);
//    rightMotor -> run(RELEASE);
//  }    
}



/*
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------
 */
int FollowLine(int xCoord, int yCoord){
  int left, right;
  Serial.println("Follow Track");
  if(xCoord < (xCenterColor - 10)){
      left = 75;
      right = 100;
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(left);
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(right);
      Serial.println("Right1");
    }else if(xCoord > (xCenterColor + 10)){
      left = 100;
      right =75;
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(left);
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(right);
      Serial.println("Left1");      
    }else if(xCoord < (xCenterColor - 100)){
      left = 25;
      right = 100;
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(left);
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(right);
      Serial.println("Right2");
    }else if(xCoord > (xCenterColor + 100)){
      left = 100;
      right = 25;
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(left);
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(right);
      Serial.println("Left2");
    }else {
      left = 100;
      right = 100;
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(left);
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(right);
      Serial.println("Straight");
    }
}

int8_t StationFound(int8_t res, char jobs){
  
  leftMotor -> run(RELEASE);
  rightMotor -> run(RELEASE);
  pixy.changeProg("line");
  Serial.println("Station Mode");
  delay(2000);  
  Serial.println(res);
  pixy.line.barcodes -> print();
  Serial.println(pixy.line.barcodes -> m_code);
  if (LINE_BARCODE){
    long leftPos = abs(leftEnc.read());
    long rightPos = abs(rightEnc.read());
    Crawl(leftPos, rightPos);
    
      if (pixy.line.barcodes->m_code==jobs){
        Serial.print("Found Station ");
        Serial.println(jobs);
        
        //Turn 90 degrees to the right
        Serial.print("Left: ");
        Serial.println(leftPos);
        Serial.print("Right: ");
        Serial.println(rightPos);
        delay(5000);
//
      //Crawl Forward 2 inches due to camera tilt error
//        long rightAdjust = rightPos + 1500;
//        long leftAdjust = leftPos + 1500;
//        while(leftPos <= leftAdjust && rightPos <= rightAdjust){
//          leftMotor -> run(FORWARD);
//          leftMotor -> setSpeed(50);
//          rightMotor -> run(FORWARD);
//          rightMotor -> setSpeed(50);
//          leftPos = abs(leftEnc.read());
//          rightPos = abs(rightEnc.read());
//          Serial.print("Left: ");
//          Serial.println(leftPos);
//          Serial.print("Right: ");
//          Serial.println(rightPos);
//        }
//        leftMotor -> run(RELEASE);
//        rightMotor -> run(RELEASE);
//        Serial.print("Left: ");
//        Serial.println(leftPos);
//        Serial.print("Right: ");
//        Serial.println(rightPos);
//        delay(10000);

        long leftTargetPos = leftPos + 6500;        
        while(leftPos < leftTargetPos){
          leftMotor -> run(FORWARD);
          leftMotor -> setSpeed(50);
          leftPos = leftEnc.read();
      
        }
          leftMotor -> run(RELEASE);
          Serial.print("Left: ");
          Serial.println(leftPos);
          Serial.print("Right: ");
          Serial.println(rightPos);
        
      }delay(1000);
  } 
  pixy.changeProg("color");
}

void Crawl(long leftPos, long rightPos){
      //Crawl Forward 2 inches due to camera tilt error
    long rightAdjust = rightPos + 1500;
    long leftAdjust = leftPos + 1500;
    while(leftPos <= leftAdjust && rightPos <= rightAdjust){
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(50);
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(50);
      leftPos = abs(leftEnc.read());
      rightPos = abs(rightEnc.read());
      Serial.print("Left: ");
      Serial.println(leftPos);
      Serial.print("Right: ");
      Serial.println(rightPos);
    }
      leftMotor -> run(RELEASE);
      rightMotor -> run(RELEASE);
      Serial.print("Left: ");
      Serial.println(leftPos);
      Serial.print("Right: ");
      Serial.println(rightPos);
      delay(10000);
}
