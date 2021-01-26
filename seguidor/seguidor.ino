#include <Servo.h>

//Constants
const unsigned int T = 1;
const float kp = 0.03;
const float ki = 0.03;
const float kd = 0.03;
int servoPin = 9;
Servo Servo1;

//Global variables
float mean, error, sensorL, sensorR, meanR, meanL;
float errorAcc = 0.0;
float errorPrev = 0.0;
float angle = 0.0;
int iteration = 0;


void setup() {
  Serial.begin(9600);
  Servo1.attach(servoPin);
}

void loop() {
  float tmpL = analogRead(A0);
  sensorL += tmpL;
  delay(1);
  float tmpR = analogRead(A1);
  sensorR += tmpR;
  delay(1);
  
  if(iteration++ % 40 == 0){
    meanL = sensorL / 40;
    meanR = sensorR / 40;
    sensorL = 0.0;
    sensorR = 0.0;
    error = meanL - meanR;
    errorAcc += error * ki;
    errorAcc = constrain(errorAcc, 0, 179);
    //errorAcc = constrain(errorAcc, -179, 179);
    if(abs(error) >= 10){
      angle = (kp * error) + errorAcc + (kd * (error - errorPrev));
      angle = constrain(angle, 0, 179);
    }
    errorPrev = error;
    Serial.println(angle);
    moveServo(angle);
    
  }
  
}

void moveServo(float angle){
  Servo1.write(angle);
  delay(40);
}
