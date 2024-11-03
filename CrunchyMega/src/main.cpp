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
int lineThreshold = 700;

// Left Wheel motor pins
int driveMotor1 = 13;    // Pin used to control the drive motor 1
int driveMotor2 = 12;    // Pin used to control the drive motor 2

// Right Wheel Motor pins
int driveMotor3 = 11;    // Pin used to control the drive motor 3
int driveMotor4 = 10;    // Pin used to control the drive motor 4

// Arm Motor pins
int vexMotor = 9;       // Pin used to control the vex motor
int stepMotor = 8;
int rotationServoPin = 7; // Pin used to control the rotation servo

int ledPin = 7;        // Pin used to control the LED


int motorControl(int value) {
    // Set the motor controller to the desired value
    servo.write(map(value, -100, 100, 1000, 2000));
    return value;
}

void setup() {
    // Start serial communication at 9600 bps
    Serial.begin(9600);
    Serial1.begin(9600);      // Serial1 for ESP8266

    // Set the line follower pins as inputs
    pinMode(leftFollower, INPUT);
    pinMode(middleFollower, INPUT);
    pinMode(rightFollower, INPUT);

    // Set the motor pins as outputs
    pinMode(driveMotor1, OUTPUT);
    pinMode(driveMotor2, OUTPUT);
    pinMode(driveMotor3, OUTPUT);
    pinMode(driveMotor4, OUTPUT);
    pinMode(stepMotor, OUTPUT);
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
    // TEST CODE:

    // motorControl(100);
    // Serial.println("Motor full speed forward");
    // digitalWrite(ledPin, HIGH);

    // // Get the encoder value 5 times
    // for (int i = 0; i < 10; i++) {
    //     Serial.print("Encoder SPEED: ");
    //     Serial.println(encoder.getSpeed());
    //     Serial.print("Encoder Position: ");
    //     Serial.println(encoder.getPosition());
    //     delay(500);
    // }

    // // Full Reverse
    // motorControl(25);
    // Serial.println("Motor full speed reverse");
    // digitalWrite(ledPin, LOW);

    // // Get the encoder value 5 times
    // for (int i = 0; i < 10; i++) {
    //     Serial.print("Encoder SPEED: ");
    //     Serial.println(encoder.getSpeed());
    //     Serial.print("Encoder Position: ");
    //     Serial.println(encoder.getPosition());
    //     delay(500);
    // }
    // END OF TEST CODE

    // Check if data is available on Serial1 (from ESP8266)
    if (Serial1.available()) {
    String receivedData = "";      // To store the incoming data

    // Read each character and build the received string
    while (Serial1.available()) {
      char incomingChar = Serial1.read();
      receivedData += incomingChar;
      delay(5); // Small delay to ensure all characters are received
    }

    // Print the received data to the Serial Monitor
    Serial.print("Received from ESP8266: ");
    Serial.println(receivedData);

    // (Optional) Parse the data if needed, e.g., using commas as delimiters
    receivedData = Serial1.readStringUntil('\n');  // Read until newline character
    Serial.println("Received data: " + receivedData);

    // Parse data into individual values
    int index1 = receivedData.indexOf(',');              // Find first comma
    int index2 = receivedData.indexOf(',', index1 + 1);  // Find second comma
    int index3 = receivedData.indexOf(',', index2 + 1);  // Find third comma
    int index4 = receivedData.indexOf(',', index3 + 1);  // Find fourth comma

    // Extract each value as a substring
    int leftW = receivedData.substring(0, index1).toInt();
    int rightW = receivedData.substring(index1 + 1, index2).toInt();
    int step = receivedData.substring(index2 + 1, index3).toInt();
    int vex = receivedData.substring(index3 + 1, index4).toInt();
    int servo = receivedData.substring(index4 + 1).toInt();

    // Move each motor according to recieved values
    digitalWrite(driveMotor1, leftW);
    digitalWrite(driveMotor2, leftW);
    digitalWrite(driveMotor3, rightW);
    digitalWrite(driveMotor4, rightW);
    digitalWrite(stepMotor, step);
    digitalWrite(vexMotor, vex);
    digitalWrite(rotationServoPin, servo);
  }

    // Read Line sensors as values
    int lDetected = analogRead(A2);
    int mDetected = analogRead(A1);
    int rDetected = analogRead(A0);
    delay(500);

    // RIGHT sensor sees dark:
    if(rDetected > lineThreshold){
        // counter-steer right:
        digitalWrite(driveMotor1, 63);
        digitalWrite(driveMotor2, 63);
        digitalWrite(driveMotor3, 0);
        digitalWrite(driveMotor4, 0);
    }
    // CENTER sensor sees dark:
    if(mDetected > lineThreshold){
        // go straight
        digitalWrite(driveMotor1, 63);
        digitalWrite(driveMotor2, 63);
        digitalWrite(driveMotor3, 63);
        digitalWrite(driveMotor4, 63);
    }
    // LEFT sensor sees dark:
    if(lDetected > lineThreshold){
        // counter-steer left:
        digitalWrite(driveMotor1, 0);
        digitalWrite(driveMotor2, 0);
        digitalWrite(driveMotor3, 63);
        digitalWrite(driveMotor4, 63);
    }
    // Stop
    motorControl(0);
    Serial.println("Motor stopped");
    delay(5000);
}
