#include <Arduino.h>
#include <PololuMaestro.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include "Arm.hpp"
#include "icons.hpp"

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

#define CURSOR(treshold) DSC(2, ((cursorPos - treshold) * 10) + 10); \
DPTL(">")

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

#define SPEED 0 // 0 is fastest
#define ACCELERATION 0

#define OPEN 8000
#define CLOSE 3968

bool settingServo = 0;
int currentMenu = 0;
int servoCon = 0;
int currentFinger = -1;
int maxMenuItems = 3; 
int minMenuItems = 1;
bool btnClick = 0;
long oldPosition  = -999;
int cursorPos = 1;
int servoPos = 0;
int value = 0;
uint8_t btnPrev;
bool resetCursor = 0;

MicroMaestro maestro(maestroSerial);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);

Encoder enc(CLK, DT);

void updateCursorPos(){
  cursorPos = enc.read() / 2;
  PT("cursor ");
  PT(maxMenuItems);
  PT(" ");
  PTL(minMenuItems);
  if(cursorPos >= maxMenuItems){
    cursorPos = maxMenuItems;
    enc.write(maxMenuItems*2);
  }
  else if(cursorPos <= minMenuItems){
    cursorPos = minMenuItems;
    enc.write(minMenuItems*2);
  }

  if(resetCursor){
    PTL("jsem tu");
    cursorPos = 0;
    enc.write(0);
    resetCursor = 0;
  }
  
}

void updateServoPos(){
  servoPos = enc.read() / 2;
  if (servoPos != oldPosition) { 
    if(oldPosition < servoPos ){
      servoCon >= OPEN ? servoCon = OPEN : servoCon += 200;
    } else if (oldPosition > servoPos){
      servoCon <= CLOSE ? servoCon = CLOSE : servoCon -= 200;
    }
    oldPosition = servoPos;
  }
}
  
void btnCheck(){
  uint8_t btnCurr = digitalRead(SW);
  btnClick = 0;

  if (btnCurr == LOW && btnPrev == HIGH){
    PTL("btn pressed");
    btnClick = 1;
  }

  btnPrev = digitalRead(SW);
}

void fingerMov(){
  if(settingServo == 1){
    switch(currentFinger){
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

void startScreen(){
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
    
    CURSOR(0);
    display.display();
  
}

void manualScreen(){
  display.clearDisplay();

  if(cursorPos < 5){
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

    CURSOR(0);
  }

  else if(cursorPos >= 5 ){
    DSC(10, 10);
    DPTL("WRIST");
    DSC(60, 10);
    DPT(ruka.getWristPos());

    DSC(10,20);
    DPTL("GO BACK"); 

    CURSOR(5);
  }

  display.display();
}

void gesturesScreen(){
  int pom = 0;
  display.clearDisplay();
  display.setTextSize(2);
  cursorPos == 0 ? pom = 0 : pom = 20;
  if( cursorPos < 2){
    display.drawBitmap(10, 0, hand, 30, 30, WHITE);  
    display.drawBitmap(10, 35, fist, 30, 30, WHITE);

    
    DSC(2, ((cursorPos) * 10) + pom + 10); 
    DPTL(">");

  }
  else if( cursorPos >= 2){
    display.drawBitmap(10, 35, fist, 30, 30, WHITE);
    display.drawBitmap(10, 0, hand, 30, 30, WHITE);  

    DSC(2, ((cursorPos) * 10) + pom + 10); 
    DPTL(">");
  }
  
  display.display();

}

void menuControl() {
  PTL("menu");

/*
  currentMenu = 0 -> Main menu
  currentMenu = 1 -> Manual handel menu 
  currentMenu = 2 -> EMG senzor menu
  currentMenu = 3 -> Gestures menu
*/


  //Main menu
  if(currentMenu == 0){
  //  PTL("jsem stale currentmenu 0");
    maxMenuItems = 3;
    minMenuItems = 1;
    if(btnClick == 1){
        switch (cursorPos){
        case 1:
          resetCursor = 1;
          currentMenu = 1;
          
          break;

        case 2:
          resetCursor = 1;
          currentMenu = 2;
          break;  

        case 3:
          resetCursor = 1;
          currentMenu = 3;
          break;  
        }
    }
  }

  // Manual menu
  else if (currentMenu == 1){
    maxMenuItems = 6;
    minMenuItems = 0;
    if(btnClick == 1){
      settingServo != 1 ? settingServo = 1 : settingServo = 0;
      servoCon = 0;
      switch (cursorPos){
        case 0:
            currentFinger = 0;
            servoCon += ruka.getThumbPos();            
          break;

        case 1:        
            currentFinger = 1;
            servoCon += ruka.getIndexPos();                     
          break;  

        case 2:                   
            currentFinger = 2;
            servoCon += ruka.getMiddlePos();                   
          break;  
        
        case 3:                 
            currentFinger = 3;
            servoCon += ruka.getRingPos();                    
          break;  
        
        case 4:                       
            currentFinger = 4;
            servoCon += ruka.getPinkyPos();                    
          break;  
        
        case 5:       
            currentFinger = 5;
            servoCon += ruka.getWristPos();  
          break;  
        
        case 6:
          enc.write(1);
          cursorPos = 1;
          currentMenu = 0;
          settingServo = 0;
          
          break;  
          
        }
      enc.write(currentFinger*2);
    }
  }

  // EMG screen
  else if (currentMenu == 2){
    maxMenuItems = 6;
    minMenuItems = 0;
    if(btnClick == 1){
      switch (cursorPos){
        case 0:
        break;  
          
      }
    }
    
  }

  //Gestures screen
  else if (currentMenu == 3){
    maxMenuItems = 6;
    minMenuItems = 0;
    if(btnClick == 1){
      switch (cursorPos){
        case 0:
          ruka.openFist();
          break;

        case 1:
          ruka.closeFist();
          break;   
          
      }
    }
  }
  
}

void setup(){
  pinMode(SW, INPUT_PULLUP);
  btnPrev = digitalRead(SW);

  // Set the serial baud rate.     
  Serial.begin(9600);
  maestroSerial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, fist, 25, 25, WHITE);
  display.display();
  delay(1000);
  ruka.openFist();
}

void loop(){
  
  settingServo  == 1 ? updateServoPos() : updateCursorPos();
  btnCheck();
  switch (currentMenu) {
    case 0:
      startScreen();
      break;
    case 1:
      manualScreen();
      fingerMov(); 
      break;
    case 2:
      break;
    case 3:
      gesturesScreen();
      break;     
  }  
    
  menuControl();




  delay(100);
  
}