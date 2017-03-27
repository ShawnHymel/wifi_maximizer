/* WiFi Maximizer
 * Author: Shawn Hymel (SparkFun Electronics)
 * Date: March 23, 2017
 * 
 * Displays the connected WiFi RSSI on a Micro OLED.
 * 
 * Connections:
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
 * Required Libraries:
 *   - ESP8266 Arduino Boards
 *     http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *   - SparkFun Micro OLED
 *     https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library/archive/master.zip
 *
 * License:
 *   Beerware - https://en.wikipedia.org/wiki/Beerware
 */
 
 /

#include "ESP8266WiFi.h"
#include <Wire.h>  // Include Wire if you're using I2C
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

// Network parameters
const char *SSID = "xxxxx";
const char *PASSWORD = "xxxxx";

// Pins
const int RESET_PIN = 0;
const int DC_PIN = 4;
const int CS_PIN = 15;

// MicroOLED Object
MicroOLED oled(RESET_PIN, DC_PIN, CS_PIN);

void setup() {

  Serial.begin(9600);

  // Set up OLED
  oled.begin();
  oled.clear(ALL);
  oled.clear(PAGE);

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
}

void loop() {

  Serial.println(WiFi.RSSI());

  // Clear OLED and display RSSI
  oled.clear(PAGE);
  oled.setCursor(0, 20);
  oled.print(WiFi.RSSI());
  oled.print(" dBm");
  oled.display();
  delay(100);
}
