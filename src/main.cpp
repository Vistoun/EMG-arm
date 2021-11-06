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

int pos = 1;


const unsigned int SPEED = 0; // 0 is fastest
const unsigned int ACCELERATION = 0;

const int OPEN = 8000;
const int CLOSE = 3968;

bool settingServo = 0;
int currentMenu = 0;
int servoCon = 0;
int current_item = -1;
int maxMenuItems = 5; 
int minMenuItems = 1;

bool btnClick = 0;
uint8_t btnPrev;

MicroMaestro maestro(maestroSerial);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);

Encoder enc(CLK, DT);

long oldPosition  = -999;


void updateEncoder(){
  long newPosition = enc.read() / 2;

  if (newPosition != oldPosition) {
    if(oldPosition < newPosition){
      servoCon >= OPEN ? servoCon = OPEN : servoCon += 200;
    }else{
      servoCon <= CLOSE ? servoCon = CLOSE : servoCon -= 200;
    }

    if(!settingServo){ 
      oldPosition = newPosition;
      newPosition >= maxMenuItems ? newPosition = maxMenuItems : pos = newPosition;
      newPosition <= minMenuItems ? newPosition = minMenuItems : pos = newPosition;
    } 
  } 

}


void btnCheck(){
  uint8_t btnCurr = digitalRead(SW);

  if (btnCurr == LOW && btnPrev == HIGH){
    btnClick = 1;
  }

  btnPrev = digitalRead(SW);
}

void mainMenu(){
    display.clearDisplay();
    display.setTextSize(2);

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
  display.clearDisplay();

  if(pos >= 5 ){

    DSC(10, 10);
    DPTL("WRIST");
    DSC(60, 10);
    DPT(ruka.getWristPos());

    DSC(10,20);
    DPTL("GO BACK"); 

    DSC(2, ((pos - 5) * 10) + 10);
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

    DSC(2, (pos * 10) + 10);
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


void gestaMenu(){
  display.clearDisplay();
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
  display.display();

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
}

void setup(){
  pinMode(SW, INPUT_PULLUP);

  // Set the serial baud rate.     
  Serial.begin(9600);
  maestroSerial.begin(9600);
  btnPrev = digitalRead(SW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) { 
  Serial.println("SSD1306 allocation failed");
  for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  ruka.closeFist();
}

void loop(){

  updateEncoder();
  btnCheck();
  menuControl();

  PTL(pos);
  
  delay(10);
}
