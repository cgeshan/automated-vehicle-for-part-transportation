#include <SoftwareSerial.h>
SoftwareSerial HC06(10,11);



void setup() {
  // put your setup code here, to run once:

  HC06.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  if(HC06.available() > )){
    array jobs = HC06.read();
    HC06.print("SUCCESS!");
    HC06.println(jobs);
  }

}
