/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  NOTE: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"



#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "****";
char pass[] = "******";

// Define the depth of the water tank and sensor pin
const int depth = 15;
const int WATER_LEVEL_PIN = 12;


const int trigPin = 13; // GPIO 13 for the Trig pin of the ultrasonic sensor
const int echoPin = 18; // GPIO 18 for the Echo pin of the ultrasonic sensor

// Initialize the BlynkTimer for timed tasks
BlynkTimer timer;

// Function to check water level
void checkWaterLevel() {
  int waterLevel = digitalRead(WATER_LEVEL_PIN);

  // Check if the tank is full based on the water level sensor
  if (waterLevel == HIGH) {
    Blynk.virtualWrite(V0, "Tank is full");
    Serial.println("tank is full");
  } else {
    Blynk.virtualWrite(V0, "Tank is not full");
    Serial.println("tank is not full");
  }
}



// Function to measure water level using an ultrasonic sensor
void waterlevel() {
// Trigger the ultrasonic sensor to measure distance
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Measure the time it takes for the ultrasonic waves to bounce back (pulseIn function)
 long t = pulseIn(echoPin, HIGH);
// Calculate the distance in centimeters
 long cm = t *0.034 / 2;
// Print the distance to the Serial Monitor
 Serial.println(cm);
// Calculate the water level based on tank depth and measured distance
 long level = depth - cm;
// Ensure the water level doesn't go negative
 if (level < 0)
 level = 0;
// Map the water level to a percentage and update the Blynk app
 level = map(level, 0, depth - 3, 0, 100);
Blynk.virtualWrite(V1, level);
}


// Setup function that runs once at the beginning
void setup() {
  // Configure pin mode for the water level sensor
  pinMode(WATER_LEVEL_PIN, INPUT_PULLUP); // Water level sensor with pull-up resistor

  pinMode(trigPin, OUTPUT); // Ultrasonic sensor trigger pin
  pinMode(echoPin, INPUT); // Ultrasonic sensor echo pin

  // Start serial communication for debugging
  Serial.begin(115200);
  // Connect to Wi-Fi and Blynk using the provided credentials and token
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // Set up timed tasks using the BlynkTimer
  timer.setInterval(1000L, checkWaterLevel); // Check water level every 1 second
  timer.setInterval(10L, waterlevel); // Measure water level every 10 milliseconds
}


// Loop function that runs continuously
void loop() {
  // Execute Blynk and timer tasks
  Blynk.run();
  timer.run();
}


