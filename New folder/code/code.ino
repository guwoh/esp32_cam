#include "OneButton.h" 
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
                 
int nutnhan = 4; 
bool isOpen = false; // Track the state of the servo

OneButton button(nutnhan, true);

void setup() 
{ 
  Serial.begin(9600);
  pinMode(3, INPUT);
  myservo.attach(2);  // attaches the servo on pin 2 to the servo object
  myservo.write(0);   // Initial position
  button.attachClick(handleClick);
} 
  
void loop() 
{ 
  button.tick(); // Check button state
  delay(10);     // Small delay to debounce
} 

void handleClick() {
  if (isOpen) {
    myservo.write(0);   // Close the servo
    isOpen = false;     // Update the state
  } else {
    myservo.write(180); // Open the servo
    isOpen = true;      // Update the state
  }
}
