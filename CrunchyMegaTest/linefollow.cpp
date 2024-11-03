#include <Arduino.h>
#include <Stepper.h>

// Line detected if ~1000, else araound ~50-100
int threshold = 800;

// Set up the line follower sensor pin
int lineFollowerPinL = A7;
int lineFollowerPinM = A8;
int lineFollowerPinR = A9;

// Set up the digital pin for the button
int buttonPin = 53;

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 15;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 6, 7, 8, 9);

// Set up motor ctonrol pins
int motorPin1 = 11;
int motorPin2 = 10;


void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Set the line follower sensor pin as an input
  pinMode(lineFollowerPin, INPUT);
  
  // Set the button pin as an input
  pinMode(buttonPin, INPUT);

  // Stepper contro
  myStepper.setSpeed(rolePerMinute);
  // initialize the serial port:

  // Set up the motor control pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);

  // Set the motor control pins to turn the motor on
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);

  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);

  // Set the motor control pins to turn the motor on
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);

  // 
  analogRead(A7)
  analogRead(A8)
  analogRead(A9)
  delay(500);

  // RIGHT sensor sees dark:
  if(SensorValue(lineFollowerRIGHT) > threshold){
      // counter-steer right:
      motor[leftMotor]  = 63;
      motor[rightMotor] = 0;
    }
    // CENTER sensor sees dark:
    if(SensorValue(lineFollowerCENTER) > threshold){
      // go straight
      motor[leftMotor]  = 63;
      motor[rightMotor] = 63;
    }
    // LEFT sensor sees dark:
    if(SensorValue(lineFollowerLEFT) > threshold){
      // counter-steer left:
      motor[leftMotor]  = 0;
      motor[rightMotor] = 63;
    }
}