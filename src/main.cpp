#include <Arduino.h>
#include <PololuMaestro.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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

#define PT(s) Serial.print(s)  //makes life easier
#define PTL(s) Serial.println(s)

#define DPT(s) display.print(s)
#define DPTL(s) display.println(s)
#define DSC(x,y) display.setCursor(x,y)

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
int currentStateCLK;
int lastStateCLK;
unsigned long lastButtonPress = 0;

const unsigned int SPEED = 0; // 0 is fastest
const unsigned int ACCELERATION = 0;

const int OPEN = 8000;
const int CLOSE = 3968;

bool btnClick = 0;
bool settingServo = 0;
int currentMenu = 0;
int servoCon = 0;
int current_item = -1;

bool volatile encoderState = 0;
int volatile s1 = 0;
int volatile s2 = 0;

bool buttonState = 0;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;

MicroMaestro maestro(maestroSerial);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);

void encoderFlag(){
  if(encoderState){
    return;
  }else{
    encoderState = 1;
    s1 = digitalRead(CLK);
    s2 = digitalRead(DT);
  }
}

void updateEncoder(){
	// Read the current state of CLK
	currentStateCLK = s1;

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (s2 != currentStateCLK) {
      if(settingServo == 0){
        pos --;
      }
      servoCon -= 200;
      if( servoCon < CLOSE){
        servoCon = CLOSE;
      }
     
		} else {
			// Encoder is rotating CW so increment
      if(settingServo == 0){
        pos++;
      }
      servoCon+=200;
      if(servoCon > OPEN){
         servoCon = OPEN;
      } 
		}
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;
  delay(1);
}

/*
void btnCheck(){
  
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
      if(btnClick != 1){
        Serial.println("Button pressed!");
        btnClick = 1;
      }else{
        btnClick = 0;
      }
		}

		// Remember last button press event
		lastButtonPress = millis();
	}
}
*/ 

void btnCheck(){
   // read the state of the switch into a local variable:
  int reading = digitalRead(SW);
  btnClick = 0;

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        PTL("Button pressed");
        btnClick = 1;
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

}



void mainMenu(){
    display.setTextSize(2);
    display.setTextColor(WHITE);

    DSC(10, 0);
    DPTL("EMG ARM");

    //---------------------------------
    display.setTextSize(1);
    DSC(10, 20);
    DPTL("Manual");

    DSC(10, 30);
    DPTL("EMG");

    DSC(10, 40);
    DPTL("Gesta");
    
    DSC(2, (pos * 10) + 10);
    DPTL(">");
    display.display();
  
}


void manMenu(){

 // int help = 7;
  display.setTextColor(WHITE);
  display.setTextSize(1);

  if(pos >= 5 ){

    DSC(10, 10);
    DPTL("WRIST");
    DSC(60, 10);
    DPT(ruka.getWristPos());

    DSC(10,20);
    DPTL("GO BACK"); 

    DSC(2, ((pos - 5) * 10) + 10);
    //DSC(2, ((pos - (help-1)) * 10) + 10);
    DPTL(">");
  }

  else{
    DSC(10, 10);
    DPTL("THUMB");
    DSC(60,10);
    DPT(ruka.getThumbPos());
    

    DSC(10, 20);
    DPTL("INDEX");
    DSC(60,20);
    DPT(ruka.getIndexPos());

    DSC(10, 30);
    DPTL("MIDDLE");
    DSC(60, 30);
    DPT(ruka.getMiddlePos());

    DSC(10, 40);
    DPTL("RING");
    DSC(60, 40);
    DPT(ruka.getRingPos());

    DSC(10, 50);
    DPTL("PINKY");
    DSC(60, 50);
    DPT(ruka.getPinkyPos());

    if(settingServo == 1){
      DSC(2, (current_item * 10) + 10);
    }else{
      DSC(2, (pos * 10) + 10);
    }
    
    DPTL(">");
  }

  display.display();
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

void emgMenu(){
  display.setTextColor(WHITE);
  //---------------------------------
  display.setTextSize(1);

}

void gestaMenu(){
   display.setTextColor(WHITE);
  //---------------------------------
  display.setTextSize(1);
  DSC(10, 0);
  DPTL("HAND");

  DSC(10, 10);
  DPTL("FIST");

  DSC(10,20);
  DPTL("ROCK AND ROLL");

  DSC(10,30);
  DPTL("OK");

  DSC(10,40);
  DPTL("V");

  DSC(10,50);
  DPTL("COUNTDOWN");

  if(pos >= 5){

  }

}

void menuControl() {

/*
  currentMenu = 0 -> Main menu
  currentMenu = 1 -> Manual handel menu 
  currentMenu = 2 -> EMG senzor menu
  currentMenu = 3 -> Gestures menu
*/


  //Main menu
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

  // Manual menu
  else if (currentMenu == 1){
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
    manMenu();
    fingerMov();
  }

  else if(currentMenu == 2){
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
  }
 
}



void setup(){
  pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Set the serial baud rate.     
  Serial.begin(115200);
  maestroSerial.begin(115200);
  
  // Reads the initial state of the outputA
  lastStateCLK = digitalRead(CLK); 

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();

  // Call updateEncoder() when any high/low changed seen
	// on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, encoderFlag, CHANGE);
	attachInterrupt(1, encoderFlag, CHANGE);
  ruka.closeFist();
}


void loop(){
  if(encoderState == 1){
    updateEncoder();
    encoderState = 0;
  }
  
  btnCheck();
  menuControl();
  display.clearDisplay();
}

