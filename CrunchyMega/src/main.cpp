#include <Arduino.h>
#include <Servo.h>
#include <I2CEncoder.h>
#include <Wire.h>
#include <Stepper.h>

// Create objects
Servo servo;           // Create a Servo object to control the servo 
Servo rotationServo;   // Create a Servo object to control the rotation servo
I2CEncoder encoder;    // Create an I2CEncoder object to read the encoder from the vex motor
Stepper myStepper(2048, 10, 11, 12, 13);

// Set pins
int leftFollower = A2;  // Pin used to read the left follower
int middleFollower = A1; // Pin used to read the middle follower
int rightFollower = A0; // Pin used to read the right follower
int lineThreshold = 700;

// Left Wheel motor pins
int driveMotor1 = 53;    // Pin used to control the drive motor 1
int driveMotor2 = 52;    // Pin used to control the drive motor 2

// Right Wheel Motor pins
int driveMotor3 = 51;    // Pin used to control the drive motor 3
int driveMotor4 = 50;    // Pin used to control the drive motor 4

// Arm Motor pins
int vexMotor = 8;       // Pin used to control the vex motor
int rotationServoPin = 9; // Pin used to control the rotation servo

int ledPin = 13;        // Pin used to control the LED


int motorControl(int value) {
    // Set the motor controller to the desired value
    servo.write(map(value, -100, 100, 1000, 2000));
    return value;
}

void setup() {
    // Start serial communication at 9600 bps
    Serial.begin(9600);

    // Set the line follower pins as inputs
    pinMode(leftFollower, INPUT);
    pinMode(middleFollower, INPUT);
    pinMode(rightFollower, INPUT);

    // Set the motor pins as outputs
    pinMode(driveMotor1, OUTPUT);
    pinMode(driveMotor2, OUTPUT);
    pinMode(driveMotor3, OUTPUT);
    pinMode(driveMotor4, OUTPUT);
    pinMode(vexMotor, OUTPUT);

    // Set the LED pin as an output
    pinMode(ledPin, OUTPUT);

    // Attach the vex motor controller to the pin
    servo.attach(vexMotor);

    // Attach the rotation servo to the pin
    rotationServo.attach(rotationServoPin);

    // Initialize the encoder for a 393 motor controller
    Wire.begin();
    encoder.init(MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
}

void loop() {
    // Full Forward
    motorControl(100);
    Serial.println("Motor full speed forward");
    digitalWrite(ledPin, HIGH);

    // Get the encoder value 5 times
    for (int i = 0; i < 10; i++) {
        Serial.print("Encoder SPEED: ");
        Serial.println(encoder.getSpeed());
        Serial.print("Encoder Position: ");
        Serial.println(encoder.getPosition());
        delay(500);
    }

    // Full Reverse
    motorControl(25);
    Serial.println("Motor full speed reverse");
    digitalWrite(ledPin, LOW);

    // Get the encoder value 5 times
    for (int i = 0; i < 10; i++) {
        Serial.print("Encoder SPEED: ");
        Serial.println(encoder.getSpeed());
        Serial.print("Encoder Position: ");
        Serial.println(encoder.getPosition());
        delay(500);
    }

    // Read Line sensors as values
    lDetected = analogRead(A2)
    mDetected = analogRead(A1)
    rDetected = analogRead(A0)
    delay(500);

    // RIGHT sensor sees dark:
    if(rDetected > LineThreshold){
        // counter-steer right:
        digitalWrite(driveMotor1, 63)
        digitalWrite(driveMotor2, 63)
        digitalWrite(driveMotor3, 0)
        digitalWrite(driveMotor4, 0)
    }
    // CENTER sensor sees dark:
    if(mDetected > LineThreshold){
        // go straight
        digitalWrite(driveMotor1, 63)
        digitalWrite(driveMotor2, 63)
        digitalWrite(driveMotor3, 63)
        digitalWrite(driveMotor4, 63)
    }
    // LEFT sensor sees dark:
    if(lDetected > LineThreshold){
        // counter-steer left:
        digitalWrite(driveMotor1, 0)
        digitalWrite(driveMotor2, 0)
        digitalWrite(driveMotor3, 63)
        digitalWrite(driveMotor4, 63)
    }
    // Stop
    motorControl(0);
    Serial.println("Motor stopped");
    delay(5000);
}
