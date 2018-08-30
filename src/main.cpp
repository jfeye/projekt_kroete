
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define P_RELAY_1 15
#define P_RELAY_2 14

Adafruit_BMP085 bmp;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

double target_temp = 40.0;
double tolerance = 0.5;

void setup() {
  Serial.begin(9600);

  pinMode(P_RELAY_1, OUTPUT);
  pinMode(P_RELAY_2, OUTPUT);
  digitalWrite(P_RELAY_1, HIGH);
  digitalWrite(P_RELAY_2, HIGH);

  if (!bmp.begin()) {
  	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  	while (1) {}
  }

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  display.clearDisplay();
  display.display();
}

void loop() {

  double current_temp = bmp.readTemperature();

  if(current_temp > target_temp + tolerance)
    digitalWrite(P_RELAY_2, HIGH);
  else if(current_temp < target_temp - tolerance)
    digitalWrite(P_RELAY_2, LOW);

  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("warm warm warm");
  display.setCursor(0,8);
  String msg = "" + String(current_temp) + " C";
  display.println("Ist " + msg);
  display.setCursor(0,16);
  String msg2 = "" + String(target_temp) + " +/- " + String(tolerance) + " C ";
  display.println("Soll " + msg2);
  display.display();

  delay(100);
}
