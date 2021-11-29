#include <Arduino.h>
#include <PololuMaestro.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include <string.h>
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

#define WRIST 0
#define THUMB 1
#define INDEX 2
#define MIDDLE 3
#define RING 4
#define PINKY 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_ADDR   0x3C // OLED I2C address

#define CLK 3
#define DT 2
#define SW 4

#define SPEED 0 // 0 is fastest
#define ACCELERATION 0

#define OPEN 8000
#define CLOSE 3968

#define PT(s) Serial.print(s)  //makes life easier
#define PTL(s) Serial.println(s)

#define DPT(s) display.print(s)
#define DPTL(s) display.println(s)
#define DSC(x,y) display.setCursor(x,y)

#define CURSOR(treshold) DSC(2, ((cursorPos - treshold) * 10) + 10); \
DPTL(">")

#define CENTER() 

#define FINGER(finger, value) DSC(((SCREEN_WIDTH - 64 ) / 2) , ((SCREEN_HEIGHT - 35) / 2) ); \ 
      DPTL(finger);                                                                          \
      DSC( ((SCREEN_WIDTH - 55 ) / 2) , ((SCREEN_HEIGHT + 30 ) / 2) );                       \
      DPTL(value)


#define CURSORS(min, max)   if(cursorPos == min){   \
        DSC(116,((SCREEN_HEIGHT - 10) / 2) );       \
        DPT(">");                                   \
        }                                           \ 
        else if(cursorPos == max ){                 \ 
          DSC(0,((SCREEN_HEIGHT - 10) / 2) );       \ 
          DPT("<");                                 \ 
        }                                           \
      else{                                         \ 
        DSC(116,((SCREEN_HEIGHT - 10) / 2) );       \ 
        DPT(">");                                   \ 
        DSC(0,((SCREEN_HEIGHT - 10) / 2) );         \ 
        DPT("<");                                   \
      }
  
#define ICON(gesture) display.drawBitmap(((SCREEN_WIDTH - 45 ) / 2), ((SCREEN_HEIGHT - 45) / 2), gesture, 45, 45, WHITE)  

bool settingServo = 0;
int currentMenu = 0;
int servoCon = 0;
int currentFinger = -1;
int maxMenuItems = 0; 
int minMenuItems = 0;
bool btnClick = 0;
long oldPosition  = -999;
int cursorPos = 0;
int servoPos = 0;
uint8_t btnPrev;
unsigned long myTime = 0;
int sensorValue = 0;
int sensorTreshold = 0;
bool sensorFist = 0;
bool sensorSwitch = 0;
float millivolt = 0;

MicroMaestro maestro(maestroSerial);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);

Encoder enc(DT, CLK);


void oledDisplayCenter(String text) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  // display on horizontal and vertical center
  display.clearDisplay(); // clear display
  display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
  display.println(text); // text to display
  display.display();
}

void updateCursorPos(){
  cursorPos = enc.read() / 2;
  if(cursorPos >= maxMenuItems){
    cursorPos = maxMenuItems;
    enc.write(maxMenuItems*2);
  }
  else if(cursorPos <= minMenuItems){
    cursorPos = minMenuItems;
    enc.write(minMenuItems*2);
  }
  
}

void cursorReset(){
    cursorPos = 0;
    enc.write(0);
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

    DSC(((SCREEN_WIDTH - 80 ) / 2) , 0);
    DPTL("EMG ARM");

    switch (cursorPos){
    case 0:
      DSC(((SCREEN_WIDTH - 64 ) / 2) , ((SCREEN_HEIGHT - 10 ) / 2) );
      DPTL("MANUAL");
      break;
    case 1:
      DSC(((SCREEN_WIDTH - 35 ) / 2) , ((SCREEN_HEIGHT - 10 ) / 2) );
      DPTL("EMG");
      break;
    case 2:
      DSC(((SCREEN_WIDTH - 95 ) / 2) , ((SCREEN_HEIGHT - 10 ) / 2) );
      DPTL("GESTURES");
      break;  
    }

   CURSORS(0,maxMenuItems);

    display.display();
  
}

void manualScreen(){
  display.setTextSize(2);
  display.clearDisplay();
  // each screen for one finger
  switch (cursorPos) {
    case 0:
      FINGER("THUMB", ruka.getThumbPos());
      break;

    case 1:
      FINGER("INDEX", ruka.getIndexPos());
      break;
    case 2:
      FINGER("MIDDLE", ruka.getMiddlePos());
      break;
    case 3:
      FINGER("RING", ruka.getRingPos());
      break;
    case 4:
      FINGER("PINKY", ruka.getPinkyPos());
      break;
    case 5:  
      FINGER("WRIST",ruka.getWristPos());
      break;
    case 6:
      DSC(((SCREEN_WIDTH - 80 ) / 2) , ((SCREEN_HEIGHT - 10 ) / 2) );
      DPTL("GO BACK"); 
      break;  
  }
  
  CURSORS(0, maxMenuItems);
  
  display.display();
}

void gesturesScreen(){
  display.clearDisplay();
  display.setTextSize(2);
  switch (cursorPos) {
  case 0:
    ICON(fist) ;
    break;
  case 1:
    ICON(hand);
    break;
  case 2:
    ICON(peace);
    break;
  case 3:
    ICON(ok);
    break; 
  case 4:
    ICON(thumbsUp);
    break;
  case 5:
    ICON(callMe);
    break;
  case 6:
    ICON(rock);
    break; 
  case 7:
    ICON(countdown);
    break;
  case 8:
    DSC(((SCREEN_WIDTH - 80 ) / 2) , ((SCREEN_HEIGHT - 10 ) / 2) );
    DPTL("GO BACK");
    break;             
  }

  CURSORS(0, maxMenuItems);

  display.display();

}

void emgScreen(){
  display.clearDisplay();
 
  switch (cursorPos) {
    case 0:
      
      FINGER("STATE", (sensorSwitch == 1 ? "ON" : "OFF"));
      
      break;

    case 1:
      
     
      break;
    case 2:
      
    
      break;
  }
  
  CURSORS(0, maxMenuItems);
  
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
  //  PTL("jsem stale currentmenu 0");
    maxMenuItems = 2;
    minMenuItems = 0;
    if(btnClick == 1){
        switch (cursorPos){
        case 0:
          currentMenu = 1;
          cursorReset();
          break;

        case 1:
          currentMenu = 2;
          cursorReset();
          break;  

        case 2:
          currentMenu = 3;
          cursorReset();
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
          currentMenu = 0;
          settingServo = 0;
          cursorReset();
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
    maxMenuItems = 8;
    minMenuItems = 0;
    if(btnClick == 1){
      switch (cursorPos){
        case 0:
          ruka.closeFist();
          break;

        case 1:
          ruka.openFist();
          break;
        case 2:
          ruka.peace();
          break;

        case 3:
          ruka.ok();
          break;

        case 4:
          ruka.thumbsUp();
          break;

        case 5:
          ruka.callMe();
          break;

        case 6:
          ruka.rock();
          break; 

        case 7:
          ruka.countdown();
          break;

        case 8:
          currentMenu = 0;
          cursorReset();
          
      }
    }
  }
  
}

void sensor (){
  sensorValue = analogRead(A0);
  millivolt = (sensorValue/1023)*5;

  if(sensorValue >= sensorTreshold && sensorFist == 0){
    ruka.closeFist();
    delay(250);
  }
  else if (sensorValue >= sensorTreshold && sensorFist == 1){
    sensorFist = 0;
    delay(250);
  } 
}

void setup(){
  pinMode(SW, INPUT_PULLUP);
  btnPrev = digitalRead(SW);

  // Set the serial baud rate.     
  Serial.begin(115200);
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

  if(sensorSwitch){
    sensor();
    sensorFist == 1 ? ruka.closeFist() : ruka.openFist();
  }

  // Stop moving with cursor position, when user is setting servo
  settingServo  == 1 ? updateServoPos() : updateCursorPos();
  btnCheck();
  menuControl();
  switch (currentMenu) {
    case 0:
      startScreen();
      break;
    case 1:
      manualScreen();
      fingerMov(); 
      break;
    case 2:
      emgScreen();
      break;
    case 3:
      gesturesScreen();
      break;     
  }  

  
 delay(10);
 
}

