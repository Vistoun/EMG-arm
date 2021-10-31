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
  SoftwareSerial maestroSerial(9, 8);
#endif

//#define PT(s) Serial.print(s)  //makes life easier
//#define PTL(s) Serial.println(s)

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

MicroMaestro maestro(maestroSerial);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

void btnCheck(){
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			//Serial.println("Button pressed!");
      btnClick = 1;
		}else{
      btnClick = 0;
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

  int help = 7;
  display.setTextColor(WHITE);
  //---------------------------------
  display.setTextSize(1);

  if(pos >= 5 ){
    display.setCursor(10, 0);
    display.println("WRIST");
    display.setCursor(60, 0);
    display.print(ruka.getWristPos());

    display.setCursor(10,10);
    display.println("GO BACK"); 

    display.setCursor(2, ((pos - (help-1)) * 10) + 10);
    display.println(">");
  }

  else{
    display.setCursor(10, 10);
    display.println("THUMB");
    display.setCursor(60,10);
    display.print(ruka.getThumbPos());
    

    display.setCursor(10, 20);
    display.println("INDEX");
    display.setCursor(60,20);
    display.print(ruka.getIndexPos());

    display.setCursor(10, 30);
    display.println("MIDDLE");
    display.setCursor(60, 30);
    display.print(ruka.getMiddlePos());

    display.setCursor(10, 40);
    display.println("RING");
    display.setCursor(60, 40);
    display.print(ruka.getRingPos());

    display.setCursor(10, 50);
    display.println("PINKY");
    display.setCursor(60, 50);
    display.print(ruka.getPinkyPos());

    if(settingServo == 1){
      display.setCursor(2, (current_item * 10) + 10);
    }else{
      display.setCursor(2, (pos * 10) + 10);
    }
    
    display.println(">");
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
  
  attachInterrupt(0, updateEncoder, CHANGE);
	attachInterrupt(1, updateEncoder, CHANGE);
  ruka.closeFist();
}


void loop(){
  btnCheck();
  menuControl();
  display.clearDisplay();
 
  
  delay(100);
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
