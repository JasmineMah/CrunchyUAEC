#include <Arduino.h>
#include <Servo.h>
#include <I2CEncoder.h>
#include <Wire.h>

Servo motorController;  // Create a Servo object to control the motor controller
int motorPin = 4;       // Pin used to control the motor controller
int ledPin = 13;        // Pin used to control the LED

// Create an I2CEncoder object to read the encoder
I2CEncoder encoder;

int motorControl(int value) {
    // Set the motor controller to the desired value
    motorController.write(map(value, -100, 100, 1000, 2000));
    return value;
}

void setup() {
    // Start serial communication at 9600 bps
    Serial.begin(9600);

    // Set the pin as an output
    pinMode(motorPin, OUTPUT);
    pinMode(ledPin, OUTPUT);

    // Attach the motor controller to the pin
    motorController.attach(motorPin);

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

    // Stop
    motorControl(0);
    Serial.println("Motor stopped");
    delay(5000);
}
