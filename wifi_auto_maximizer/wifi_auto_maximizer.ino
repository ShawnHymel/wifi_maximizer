/* WiFi Auto Maximizer
 * Author: Shawn Hymel (SparkFun Electronics)
 * Date: April 14, 2017
 * 
 * Displays the connected WiFi RSSI on a Micro OLED while moving
 * through a series of pan/tilt angles. Once sweep is complete,
 * it will return to the position with the maximum connection
 * strength (RSSI).
 * 
 * OLED Connections:
 *   Thing Dev  | OLED
 *   -----------|------
 *      GND     |  GND
 *      3V3     |  3V3
 *    13 (MOSI) |  SDI
 *    14 (SCK)  |  SCK
 *    12 (MISO) |  SDO
 *       4      |  D/C
 *       0      |  RST
 *      15      |  CS
 * 
 * Servo Connections (pan/tilt):
 *   Thing Dev  | Pan Servo | Tilt Servo
 *   -----------|-----------|-----------
 *       GND    |    GND    |    GND
 *       VIN    |     +     |     +
 *        5     |    SIG    |
 *        2     |           |    SIG
 *      
 * Required Libraries:
 *   - ESP8266 Arduino Boards
 *     http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *   - SparkFun Micro OLED
 *     https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library/archive/master.zip
 *
 * License:
 *   Beerware - https://en.wikipedia.org/wiki/Beerware
 */
 
#include "ESP8266WiFi.h"
#include <Wire.h>  // Include Wire if you're using I2C
#include <SPI.h>   // Include SPI if you're using SPI
#include <Servo.h> // For great servo
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

// Network parameters
const char *SSID = "xxxxx";
const char *PASSWORD = "xxxxx";

// Min and max servo settings
const int PAN_MIN = 0;
const int PAN_MAX = 180;
const int PAN_INC = 5;
const int TILT_MIN = 80;
const int TILT_MAX = 150;
const int TILT_INC = 5;

// Pins
const int RESET_PIN = 0;
const int DC_PIN = 4;
const int CS_PIN = 15;
const int PAN_PIN = 5;
const int TILT_PIN = 2;

// Let's remember the maximal pan/tilt settings
int max_pan = 0;
int max_tilt = 0;
long max_rssi = -200;

// MicroOLED object
MicroOLED oled(RESET_PIN, DC_PIN, CS_PIN);

// Servo objects
Servo pan_servo;
Servo tilt_servo;

/*************************************************************
 * Setup
 ************************************************************/
void setup() {

  int pan_pos;
  int tilt_pos;
  long rssi;

  Serial.begin(9600);

  // Set up OLED
  oled.begin();
  oled.clear(ALL);
  oled.clear(PAGE);

  // Initialize servos
  pan_servo.attach(PAN_PIN);
  tilt_servo.attach(TILT_PIN);

  // Show the WiFi network we're connecting to
  oled.setFontType(0);
  oled.setCursor(0, 0);
  oled.print("Connecting to ");
  oled.print(SSID);
  oled.display();

  Serial.println("connecting...");

  // Connect to said network
  WiFi.begin(SSID, PASSWORD);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    oled.print(".");
    oled.display();
  }

  Serial.println("connected");

  // Show that we're connected
  oled.print("\nconnected!");
  delay(2000);
  oled.setFontType(1);

  // sweep through pan and tilt looking for max RSSI
  max_pan = pan_pos;
  max_tilt = tilt_pos;
  for ( int pan_pos = PAN_MIN; pan_pos <= PAN_MAX; pan_pos += PAN_INC ) {
    for ( int tilt_pos = TILT_MIN; tilt_pos <= TILT_MAX; tilt_pos += PAN_INC ) {
      pan_servo.write(pan_pos);
      tilt_servo.write(tilt_pos);
      rssi = getAndDisplayRSSI();
      if ( rssi > max_rssi ) {
        max_rssi = rssi;
        max_pan = pan_pos;
        max_tilt = tilt_pos;
      }
      delay(10);
    }
  }

  // Tell the user we found the max pan/tilt
  oled.clear(PAGE);
  oled.setFontType(0);
  oled.setCursor(0, 0);
  oled.print("Maximizing WiFi");
  oled.display();

  // Move to max WiFi position
  pan_servo.write(max_pan);
  tilt_servo.write(max_tilt);

  // Wait and clear OLED
  delay(1000);
  oled.clear(PAGE);
  oled.display();
  oled.setFontType(1);
}

/*************************************************************
 * Loop
 ************************************************************/
void loop() {
  getAndDisplayRSSI();
}

/*************************************************************
 * Functions
 ************************************************************/
long getAndDisplayRSSI() {

  long rssi = WiFi.RSSI();

  // Clear OLED and display RSSI
  oled.clear(PAGE);
  oled.setCursor(0, 20);
  oled.print(rssi);
  oled.print(" dBm");
  oled.display();
  delay(100);

  return rssi;
}
