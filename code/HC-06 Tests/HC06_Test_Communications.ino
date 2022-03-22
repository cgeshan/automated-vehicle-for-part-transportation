#include <SoftwareSerial.h>
SoftwareSerial HC05(10,11);

char jobs;

void setup() {
  // put your setup code here, to run once:

  HC05.begin(9600);
  Serial.begin(9600);
//  HC05.println("Ready");

}

void loop() {
  // put your main code here, to run repeatedly:

  if(HC05.available() > 0){
    Serial.println("YES ");
    jobs = HC05.read();
    if(jobs > 0){
      Serial.print("SUCCESS!");
    }
    Serial.println(jobs);
    HC05.println("Delivered");
  }

}
