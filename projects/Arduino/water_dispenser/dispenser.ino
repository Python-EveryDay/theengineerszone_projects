#include <Servo.h>

Servo gateServo;

const int irPin = 2;

void setup() {
  pinMode(irPin, INPUT);

  gateServo.attach(9);
  gateServo.write(0);   // Initial position

  Serial.begin(9600);
}

void loop() {

  int objectDetected = digitalRead(irPin);

  // Most IR obstacle sensors output LOW when object is detected
  if (objectDetected == LOW) {

    Serial.println("Object Detected!");
    for(int i = 0; i<= 90; i+=5){
    gateServo.write(i);
    }
    
  }
  else{
    gateServo.write(0);
  }
}