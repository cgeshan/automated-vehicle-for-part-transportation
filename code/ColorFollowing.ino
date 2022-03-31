#include <Pixy2.h>
#include <PIDLoop.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

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

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  AFMS.begin();
  leftMotor -> run(RELEASE);
  leftMotor -> setSpeed(0);
  rightMotor -> run(RELEASE);

  pixy.init();
  pixy.setLamp(1, 0);  
  //pixy.changeProg("line");
  pixy.changeProg("color");

  // look straight and down
  pixy.setServos(550, 550);
}
void loop()
{
  int8_t res, blocks;
  int32_t error;
  int left, right;
  char buf[96];

  blocks = pixy.ccc.getBlocks();
  //Serial.println(blocks);


  if(blocks){
    //Serial.println("We have blocks");
    for (int i = 0; i < blocks; i++){
      if ((oldSignature == 0) || (pixy.ccc.blocks[i].m_signature == oldSignature)){
      long newSize = pixy.ccc.blocks[i].m_height * pixy.ccc.blocks[i].m_width;
        if (newSize > maxSize){
          int trackedBlock = i;
          maxSize = newSize;
        }
      }
     // pixy.ccc.blocks[i].print();
      int xCoord = pixy.ccc.blocks[trackedBlock].m_x;
      int yCoord = pixy.ccc.blocks[trackedBlock].m_y;

      if(xCoord < (xCenterColor - 10)){
        left = 75;
        right = 100;
        Serial.println("Right");
      }else if(xCoord > (xCenterColor + 10)){
        Serial.println("Left");
        left = 100;
        right =75;
      }else if(xCoord < (xCenterColor - 100)){
        left = 25;
        right = 100;
        Serial.println("Right");
      }else if(xCoord > (xCenterColor + 100)){
        Serial.println("Left");
        left = 100;
        right = 25;
      }else {
        left = 100;
        right = 100;
        Serial.println("Straight");
      }  
      leftMotor -> run(FORWARD);
      leftMotor -> setSpeed(left);
      rightMotor -> run(FORWARD);
      rightMotor -> setSpeed(right);
      
/*
      Serial.print("(");
      Serial.print(xCoord);
      Serial.print(", ");
      Serial.print(yCoord);
      Serial.print(")     Center of Screen x coordinate: ");
      Serial.println(xCenterColor);
*/
    }
  }else{
    Serial.println("No Blocks :(");
      leftMotor -> run(RELEASE);
      rightMotor -> run(RELEASE);
  }
  
/*
  // Get latest data from Pixy, including main vector, new intersections and new barcodes.
  res = pixy.line.getMainFeatures();
  Serial.println(res);
  
  if (res == 1) {
      if (pixy.line.vectors->m_x1 > X_CENTER) {
          left = 75;
          right = 50;
          Serial.println("RIGHT");
      } else if (pixy.line.vectors->m_x1 < X_CENTER) {
          left = 50;
          right = 75;
          Serial.println("LEFT");
      } else {
          left = 75;
          right = 75;
          Serial.println("Straight");
      }
    } else if(res <= 0) {
      left = 0;
      right = 0;
      Serial.println("stop ");
    }
  leftMotor -> run(FORWARD);
  leftMotor -> setSpeed(left);
  rightMotor -> run(FORWARD);
  rightMotor -> setSpeed(right);
  Serial.println(left);
  Serial.println(right);
  //delay(2000);

  if (res&LINE_BARCODE){
      if (pixy.line.barcodes->m_code==0){
        leftMotor -> run(RELEASE);
        rightMotor -> run(RELEASE);
        Serial.println("Delivered");
        delay(10000);
      }
    }

    */
    
}
