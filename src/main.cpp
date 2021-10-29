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
#define OLED_ADDR   0x3C

#define CLK 2
#define DT 3
#define SW 4

int pos = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

const unsigned int SPEED = 0; // 0 is fastest
const unsigned int ACCELERATION = 0;

const unsigned int OPEN = 8000;
const unsigned int CLOSE = 3968;
bool btnClick = 0;
int currentMenu = 0;



// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MicroMaestro maestro(maestroSerial);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);


void updateEncoder(){
	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			pos --;
      if(pos < 1){
         pos = 1;
      }
     
			currentDir ="CCW";
		} else {
			// Encoder is rotating CW so increment
			pos ++;
      if(pos > 5){
         pos = 5;
      }
     
			currentDir ="CW";
		}

	
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;
  delay(1);
}

void btnCheck(){
  btnClick = 0;
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			Serial.println("Button pressed!");
      btnClick = 1;
		}

		// Remember last button press event
		lastButtonPress = millis();
	}
  
}

void mainMenu(){
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
    
    display.setCursor(2, (pos * 10) + 10);
    display.println(">");
    display.display();
  
}


void manMenu(){
  display.setTextColor(WHITE);
  //---------------------------------
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.println("Thumb");

  display.setCursor(10, 20);
  display.println("Index");

  display.setCursor(10, 30);
  display.println("Middle");

  display.setCursor(10, 40);
  display.println("Ring");

  display.setCursor(10, 50);
  display.println("Pinky");
  
 // Serial.println(pos);
  display.setCursor(2, (pos * 10) + 10);
  display.println(">");

  display.display();
}

void menuControl() {



  if(currentMenu == 0){
    if(btnClick == 1){
      Serial.println(pos);
        switch (pos){
        case 1:
          Serial.println("currentmenu pos 1");
          currentMenu = 1;
          break;

        case 2:
          currentMenu = 2;
          break;  

        case 3:
          currentMenu = 3;
          break;  
        }
    }
    mainMenu();
  }


  else if (currentMenu == 1){
    manMenu();
  }
 

}



void setup(){
  pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  // Set the serial baud rate.     
  Serial.begin(115200);
  maestroSerial.begin(9600);
   
  // Reads the initial state of the outputA
  
  lastStateCLK = digitalRead(CLK); 

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();
  attachInterrupt(0, updateEncoder, CHANGE);
	attachInterrupt(1, updateEncoder, CHANGE);
}


void loop(){
  btnCheck();

  menuControl();
  display.clearDisplay();
  
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
