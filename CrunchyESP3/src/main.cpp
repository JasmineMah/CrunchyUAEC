#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "SM-Ultra";           // Replace with your Wi-Fi network name

ESP8266WebServer server(80);             // Initialize server on port 80

// Variables to hold the five values
String leftW = "0";
String rightW = "0";
String stepArm = "0";
String vexArm = "0";
String flipArm = "0";

void handleRoot() {
  String message;
  
  // Check if each value parameter is in the URL and update variables
  if (server.hasArg("leftW")) leftW = server.arg("leftW");
  if (server.hasArg("rightW")) rightW = server.arg("rightW");
  if (server.hasArg("stepArm")) stepArm = server.arg("stepArm");
  if (server.hasArg("vexArm")) vexArm = server.arg("vexArm");
  if (server.hasArg("flipArm")) flipArm = server.arg("flipArm");

  // Display received values in Serial Monitor
  Serial.println("Received values:");
  Serial.println("Left wheel: " + leftW);
  Serial.println("Right wheel: " + rightW);
  Serial.println("Stepper Arm: " + stepArm);
  Serial.println("Vex Arm: " + vexArm);
  Serial.println("Flipper Servo: " + flipArm);

  // Send the values serially to Arduino
  Serial.print(leftW + "," + rightW + "," + stepArm + "," + vexArm + "," + flipArm + "\n");

  // Create HTML form for submitting values
  message = "<form method='GET' action='/'>";
  message += "<label>Left wheel: </label><input type='text' name='leftW' placeholder='Enter Left wheel'><br>";
  message += "<label>Right wheel: </label><input type='text' name='rightW' placeholder='Enter Right wheel'><br>";
  message += "<label>Stepper Arm: </label><input type='text' name='stepArm' placeholder='Enter Stepper Arm'><br>";
  message += "<label>Vex Arm: </label><input type='text' name='vexArm' placeholder='Enter Vex Arm'><br>";
  message += "<label>Flipper Servo: </label><input type='text' name='flipArm' placeholder='Enter Flipper Servo'><br>";
  message += "<input type='submit' value='Send'>";
  message += "</form>";

  // Send response with form and message
  server.send(200, "text/html", message);
}

void setup() {
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up server routes
  server.on("/", handleRoot);    // Define the root endpoint
  server.begin();                // Start the server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Listen for incoming clients
}
