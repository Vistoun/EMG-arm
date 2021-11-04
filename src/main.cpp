#include <Arduino.h>
#include <PololuMaestro.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include "Arm.hpp"


/* On boards with a hardware serial port available for use, use
that port to communicate with the Maestro. For other boards,
create a SoftwareSerial object using pin D9 to receive (RX) and
pin D8 to transmit (TX). */

#ifdef SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
  #include <SoftwareSerial.h>
  SoftwareSerial maestroSerial(9, 8);
#endif

//#define PT(s) Serial.print(s)  //makes life easier
//#define PTL(s) Serial.println(s)

//#define DPT(s) display.print(s)
//#define DPTL(s) display.println(s)
//#define DSC(x,y) display.setCursor(x,y)

#define WRIST 0
#define THUMB 1
#define INDEX 2
#define MIDDLE 3
#define RING 4
#define PINKY 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_ADDR   0x3C // OLED I2C address

#define CLK 2
#define DT 3
#define SW 4

int pos = 0;
unsigned long lastButtonPress = 0;

const unsigned int SPEED = 0; // 0 is fastest
const unsigned int ACCELERATION = 0;

const int OPEN = 8000;
const int CLOSE = 3968;

bool btnClick = 0;
bool settingServo = 0;
int currentMenu = 1;
int servoCon = 0;
int current_item = -1;


bool buttonState = 0;        // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;

MicroMaestro maestro(maestroSerial);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);

//Encoder enc(CLK, DT);

long oldPosition  = -999;

/*
void updateEncoder(){
  long newPosition = enc.read() / 2;
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    pos = newPosition;
  }
}
*/
void btnCheck(){
  int btnState = digitalRead(SW);
  btnClick = 0;
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
    display.clearDisplay();
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
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  
  if(pos >= 5 ){
    display.setCursor(10, 0);
    display.println("WRIST");
    display.setCursor(60, 0);
    display.print(ruka.getWristPos());

    display.setCursor(10,10);
    display.println("GO BACK"); 

    display.setCursor(2, ((pos - 6) * 10) + 10);
    display.println(">");
  } else{
    display.setCursor(10, 10);
    display.println("THUMB");
    display.setCursor(60,10);
    //display.print(maestro.getPosition(WRIST));
    

    display.setCursor(10, 20);
    display.println("INDEX");
    display.setCursor(60,20);
   // display.print(ruka.getIndexPos());

    display.setCursor(10, 30);
    display.println("MIDDLE");
    display.setCursor(60, 30);
    //display.print(ruka.getMiddlePos());

    display.setCursor(10, 40);
    display.println("RING");
    display.setCursor(60, 40);
    //display.print(ruka.getRingPos());

    display.setCursor(10, 50);
    display.println("PINKY");
    display.setCursor(60, 50);
    //display.print(ruka.getPinkyPos());

    display.setCursor(2, (pos * 10) + 10);
    display.println(">");
    display.display();
  }

  
}


void fingerMov(){
  if(settingServo == 1){
    switch(current_item){
      case 0:
      ruka.moveFinger(THUMB, servoCon);
      break;

      case 1:
      ruka.moveFinger(INDEX,servoCon);
      break;

      case 2:
      ruka.moveFinger(MIDDLE,servoCon);
      break;

      case 3:
      ruka.moveFinger(RING,servoCon);
      break;

      case 4:
      ruka.moveFinger(PINKY,servoCon);
      break;

      case 5:
      ruka.moveFinger(WRIST,servoCon);
      break;
    }
    
  }
}

void menuControl() {
  if(currentMenu == 0){
    if(btnClick == 1){
        switch (pos){
        case 1:
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

  if (currentMenu == 1){
    if(btnClick == 1){
        switch (pos){
        case 0:
          if(settingServo != 1){
            settingServo = 1;
            servoCon = 0;
            current_item = 0;
            servoCon += ruka.getThumbPos();
            
          }else{
            settingServo = 0;
          }
          break;

        case 1:
          if(settingServo != 1){
            settingServo = 1;
            servoCon = 0;
            current_item = 1;
            servoCon += ruka.getIndexPos();
            
          }else{
            settingServo = 0;
          }
          break;  

        case 2:
          if(settingServo != 1){
            settingServo = 1;
            servoCon = 0;
            current_item = 2;
            servoCon += ruka.getMiddlePos();
            
          }else{
            settingServo = 0;
          }
          break;  
        
        case 3:
          if(settingServo != 1){
            settingServo = 1;
            servoCon = 0;
            current_item = 3;
            servoCon += ruka.getRingPos();
            
          }else{
            settingServo = 0;
          }
          break;  
        
        case 4:
            if(settingServo != 1){
            settingServo = 1;
            servoCon = 0;
            current_item = 4;
            servoCon += ruka.getPinkyPos();
            
          }else{
            settingServo = 0;
          }
          break;  
        
        case 5:
          if(settingServo != 1){
            settingServo = 1;
            servoCon = 0;
            current_item = 5;
            servoCon += ruka.getWristPos();
            
          }else{
            settingServo = 0;
          }
          break;  
        
        case 6:
          currentMenu = 0;
          break;  
          
        }
    }
    Serial.println("jsem tady");
    manMenu();
    fingerMov();
  }
 
}





void setup(){
  pinMode(SW, INPUT_PULLUP);

  // Set the serial baud rate.     
  Serial.begin(9600);
  maestroSerial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
}

void loop(){
  
  //btnCheck();
  //updateEncoder();
  //menuControl();
  Serial.println("jedu");
  Serial.println(maestro.getPosition(WRIST));
  Serial.println("jedu");
  delay(100);

}











































