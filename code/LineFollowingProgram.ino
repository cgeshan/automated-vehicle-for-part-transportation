
#include <Pixy2.h>
#include <PIDLoop.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create an array of delivery stations
String allStations[] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15"};
//String myDeliveries[];

// LED Setup for various line following testing, removing implementation in final deliverable
int leftLED = 10;
int rightLED = 9;
//int stopLED = 26;


// Initializing Motor Shield, Motor speeds, maximum allowed is 250
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
// Play around with these values, remember 250 is max speed
]int FAST = 200;
int SLOW = 100;

// Initialization of PixyCam
#define X_CENTER         (pixy.frameWidth/2)
Pixy2 pixy;

// Creating PID Loop
PIDLoop headingLoop(5000, 0, 0, false);

void setup() 
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
  AFMS.begin();
  leftMotor -> run(RELEASE);
  leftMotor -> setSpeed(0);
  rightMotor -> run(RELEASE);
  rightMotor -> setSpeed(0);
//  motors.setLeftSpeed(0);
//  motors.setRightSpeed(0);

  pixy.init();
  // Turn on both lamps, upper and lower for maximum exposure
  pixy.setLamp(1, 1);
  // change to the line_tracking program.  Note, changeProg can use partial strings, so for example,
  // you can change to the line_tracking program by calling changeProg("line") instead of the whole
  // string changeProg("line_tracking")
  pixy.changeProg("line");

  // look straight and down
  pixy.setServos(500, 1000);

// Setup for testing LEDs
pinMode(leftLED, OUTPUT);
pinMode(rightLED, OUTPUT);
//pinMode(stopLED, OUTPUT);
}


void loop()
{
  int8_t res;
  int32_t error; 
  int left, right;
  char buf[96];

  // Get latest data from Pixy, including main vector, new intersections and new barcodes.
  res = pixy.line.getMainFeatures();

  // If error or nothing detected, stop motors
  if (res<=0) 
  {
  leftMotor -> run(RELEASE);
  leftMotor -> setSpeed(0);
  rightMotor -> run(RELEASE);
  rightMotor -> setSpeed(0);

// Let's make a red LED turn on signaling an error, maybe the indicator I bought
//  digitalWrite(stopLED, HIGH);
//  digitalWrite(intersectionLED, LOW);
//  digitalWrite(normalLED, LOW);
      
//    motors.setLeftSpeed(0);
//    motors.setRightSpeed(0);
   // buzzer.playFrequency(500, 50, 15);
    Serial.print("stop ");
    Serial.println(res);
    return;
  }

  // We found the vector...
  if (res&LINE_VECTOR)
  {
    // Calculate heading error with respect to m_x1, which is the far-end of the vector,
    // the part of the vector we're heading toward.
    error = (int32_t)pixy.line.vectors->m_x1 - (int32_t)X_CENTER;

    pixy.line.vectors->print();

    // Perform PID calcs on heading error.
    headingLoop.update(error);

    // separate heading into left and right wheel velocities.
    left = headingLoop.m_command;
    right = -headingLoop.m_command;

    // If vector is heading away from us (arrow pointing up), things are normal.
    if (pixy.line.vectors->m_y0 > pixy.line.vectors->m_y1)
    {
      // ... but slow down a little if intersection is present, so we don't miss it.
      if (pixy.line.vectors->m_flags&LINE_FLAG_INTERSECTION_PRESENT)
      {
// Let's mess around with these values
        left += SLOW;
        right += SLOW;

// Turn on yellow LED signalling an intersection is approaching
//  digitalWrite(stopLED, LOW);
//  digitalWrite(intersectionLED, HIGH);
//  digitalWrite(normalLED, LOW);
    }
      else // otherwise, pedal to the metal!
      {
// Let's mess around with these values
        left += FAST;
        right += FAST;
        
// Turn on green LED signalling an clear path forward
//  digitalWrite(stopLED, LOW);
//  digitalWrite(intersectionLED, LOW);
//  digitalWrite(normalLED, HIGH);
      }    
    }
    else  // If the vector is pointing down, or down-ish, we need to go backwards to follow.
    {
// Let's mess around with these values
      left -= SLOW;
      right -= SLOW;  
    } 
    leftMotor -> run(FORWARD);
    leftMotor -> setSpeed(left);
    rightMotor -> run(FORWARD);
    rightMotor -> setSpeed(right);
    if(left > right){
      digitalWrite(leftLED, HIGH);
      digitalWrite(rightLED, LOW);
      
    }else if (right>left){
      digitalWrite(leftLED, LOW);
      digitalWrite(rightLED, HIGH);
      
    }else {
      digitalWrite(leftLED, LOW);
      digitalWrite(rightLED, LOW);
      
    }
    //motors.setLeftSpeed(left);
    //motors.setRightSpeed(right);
  }

  // If intersection, do nothing (we've already set the turn), but acknowledge with a beep.
  if (res&LINE_INTERSECTION)
  {
 //   buzzer.playFrequency(1000, 100, 15);
    pixy.line.intersections->print();
  }

  // If barcode, acknowledge with beep, and set left or right turn accordingly. 
  // When calling setNextTurn(), Pixy will "execute" the turn upon the next intersection, 
  // making the left or right branch in the intersection the new main vector, depending on 
  // the angle passed to setNextTurn(). The robot will then follow the branch.  
  // If the turn is not set, Pixy will choose the straight(est) path by default, but 
  // the default turn can be changed too by calling setDefaultTurn(). The default turn
  // is normally 0 (straight).   
  if (res&LINE_BARCODE)
  {
 //   buzzer.playFrequency(2000, 100, 15);
    pixy.line.barcodes->print();
    // code==0 is our left-turn sign
    if (pixy.line.barcodes->m_code==0)
      pixy.line.setNextTurn(90); // 90 degrees is a left turn 
    // code==5 is our right-turn sign
    else if (pixy.line.barcodes->m_code==5)
      pixy.line.setNextTurn(-90); // -90 is a right turn 
  }
}

void stationDelivery(String station){
  int8_t res2;
  res2 = pixy.line.getMainFeatures();
  
  if(res2&LINE_BARCODE){
    // If scanned barcode matched the station
    if(pixy.line.barcodes -> m_code == station.toInt()){
      // Turn to the right
      pixy.line.setNextTurn(-90);
      /* At this point in the code we will need to complete the delivery of the materials. 
         in order to complete this we may need to end functionality to PixyCam, complete delivery, 
         return to track/line, then turn line following program back on and continue with deliveries. 

         Cannot forget to remove station from myDeliveries Array

         
      */
    }
  }
}

String readSerial(){
/*
 * Current implementation is for single station delivery, will need to edit 
 * in order to add multi station delivery functionality. Will need to append station to 
 * myDeliveries in numerical order. 
 * */
  String line = Serial.readStringUntil("/n");
  String station;
// If Serial monitor line contains the string "New Delivery" from python GUI then append myDeliveries array    
  if(line.indexOf("New Delivery") > 0){
    // read the last characters of the string until a space for delivery station
    if(line.length() > 23){
      station = line.substring(22, 24);
      Serial.println(station);
    }else if(line.length()>22){
      station = line.substring(22, 23);
      Serial.println(station);
    }else{
      Serial.println("Error, not delivery station was received!");
    }
  }
  return station;
}
