#include <Pixy2.h>
#include <PIDLoop.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Initializing Motor Shield, Motor speeds, maximum allowed is 250
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

// Initialization of PixyCam
#define X_CENTER         (pixy.frameWidth/2)
Pixy2 pixy;

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
  pixy.setLamp(1, 1);
  pixy.changeProg("line");

  // look straight and down
  pixy.setServos(550, 600);
}
void loop()
{
  int16_t res;
  int32_t error;
  int left, right;
  char buf[96];
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
}
