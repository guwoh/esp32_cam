#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards



void setup() {
  pinMode(3,INPUT);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(1000);
}

void loop() {
  int temp= digitalRead(3);
  if(temp==1)
  {
    myservo.write(180);
    delay(3000);
  }
  else
  {
    myservo.write(0);
  } 
  
  
  
}


