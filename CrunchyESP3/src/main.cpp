#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "SM-Ultra";         // Replace with your Wi-Fi network name

ESP8266WebServer server(80);           // Initialize server on port 80

void handleRoot() {
  String message;
  
  // Check if a "value" parameter is in the URL
  if (server.hasArg("value")) {
    String value = server.arg("value");
    Serial.println("Received value: " + value);
    message = "Value received: " + value;
  } else {
    message = "Send a value using ?value=<your_value>";
  }

  // HTML form for submitting values
  message += "<form method='GET' action='/'>";
  message += "<input type='text' name='value' placeholder='Enter a value'>";
  message += "<input type='submit' value='Send'>";
  message += "</form>";

  server.send(200, "text/html", message);  // Send the response
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