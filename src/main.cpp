#include <Arduino.h>
#include <PololuMaestro.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arm.hpp"


 /* On boards with a hardware serial port available for use, use
that port to communicate with the Maestro. For other boards,
create a SoftwareSerial object using pin 10 to receive (RX) and
pin 11 to transmit (TX). */

#ifdef SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
  #include <SoftwareSerial.h>
  SoftwareSerial maestroSerial(0, 1);
#endif

#define PT(s) Serial.print(s)  //makes life easier
#define PTL(s) Serial.println(s)

#define WRIST 0
#define THUMB 1
#define INDEX 2
#define MIDDLE 3
#define RING 4
#define PINKY 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define encoder0PinA  2
#define encoder0PinB  3

#define OLED_ADDR   0x3C



volatile unsigned int encoder0Pos = 0;

const unsigned int SPEED = 0; // 0 is fastest
const unsigned int ACCELERATION = 0;

const unsigned int OPEN = 8000;
const unsigned int CLOSE = 3968;

byte clk;
byte menuCount = 1;
byte dir = 0;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MicroMaestro maestro(maestroSerial);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);


void Menu() {
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(10, 0);
  display.println("EMG ARM");
  //---------------------------------
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Manual");

  display.setCursor(10, 30);
  display.println("EMG");

  display.setCursor(10, 40);
  display.println("Gesta");

  display.setCursor(2, (menuCount * 10) + 10);
  display.println(">");

  display.display();
}


void menuCheck() {
  if (clk == LOW && menuCount < 5) {
    menuCount++;
    encoder0Pos = 0;
  }
  if (clk == LOW && menuCount >= 5) {
    menuCount = 1;
  }

  if (menuCount == 1) {
    valA = encoder0Pos;
  }
  if (menuCount == 2) {
   valB = encoder0Pos;
  }
  if (menuCount == 3) {
    valC = encoder0Pos;
  }
}


void doEncoder() {
  if (digitalRead(encoder0PinA) == HIGH) {
    if (digitalRead(encoder0PinB) == LOW && encoder0Pos > 0) {
      encoder0Pos = encoder0Pos - 1;
      dir = 0;
    }
    else {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
  }
  else
  {
    if (digitalRead(encoder0PinB) == LOW ) {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
    else {
      if (encoder0Pos > 0) {
        encoder0Pos = encoder0Pos - 1;
        dir = 0;
      }
    }
  }
}

void setup(){

  // Set the serial baud rate.     
  Serial.begin(9600);
  maestroSerial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();
  attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
}


void loop(){

}


/*
  if(stage == 0){
    while(!Serial.available());
    input = Serial.read();
    if((input == 'Y' || input == 'y') && stage == 0 ){
      delay(100);
      stage += 1;
      input = NULL;
    }
  }

  if(stage == 1){
    PTL("Zadejte kanal prstu, ktery chcete pouzivat: ");
      while(!Serial.available());
      userFinger = Serial.read();
      if(userFinger != NULL){
        stage++;
      }
    delay(100);
  }

  if(stage == 2){
    PTL("Zadejte pozici: ");
    while(!Serial.available());
    userPos = Serial.read();
    if(userPos != NULL){
      stage++;
      PTL("Pokud chcete pokracovat, tak zadejte Y: ");
    }    
    delay(100);
  }

  if(stage == 3){
    maestro.setTarget(userFinger, userPos);
    while(!Serial.available());
    input = Serial.read();
    8000
    if(input == 'Y' || input == 'y' ){
      stage = 1;
    }
    delay(100);
  }
 */ 
