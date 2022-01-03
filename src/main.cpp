#include <Arduino.h>
#include <PololuMaestro.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include "Arm.hpp"
#include "icons.hpp"
#include "Battery.h"
#include <VoltageReference.h>

/* PINS

ENCODER:
GND -> GND 
+ -> 5V
SW -> 4
DT -> 2
CLK -> 3


OLED:
GND -> GND 
VIN -> 5V 
SCL -> A5
SDA -> A4

BUZZER:
GND -> GND 
+ -> 8

RGB LED OPTIONAL:
GND -> GND 
RED -> 7
GREEN -> 6 
BLUE -> 5

MICRO MAESTRO:
GND -> GND 
RX -> 11
TX -> 10

EMG SENSOR:
GND -> GND
+ -> 5V
SIGNAL -> A0

*/


/* On boards with a hardware serial port available for use, use
that port to communicate with the Maestro. For other boards,
create a SoftwareSerial object using pin D9 to receive (RX) and
pin D8 to transmit (TX).

Maestro RX -> 8 (TX)
Maestro TX -> 9 (RX)

 */

#ifdef SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
  #include <SoftwareSerial.h>
  SoftwareSerial maestroSerial(9, 11);
#endif

// Define fingers servo pin, connected to micro meastro
#define WRIST 0
#define THUMB 1
#define INDEX 2
#define MIDDLE 3
#define RING 4
#define PINKY 5

// Define servos speed and acceleration 
#define SPEED 0 // 0 is fastest
#define ACCELERATION 0 

//A typical RC hobby servo responds to pulses between 1 ms (4000) and 2 ms (8000).
#define OPEN 8000 // value when is finger is fully open
#define CLOSE 3968 // value when is fully closed

// OLED pins
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_ADDR   0x3C // OLED I2C address

// Encoder pins 
#define CLK 2 
#define DT 3
#define SW 4 // this port is used for button function on encoder

// Buzzer pin
#define BUZZER 8

// Macros to make coding easier 
#define PT(s) Serial.print(s)  
#define PTL(s) Serial.println(s)

#define DPT(s) display.print(s)
#define DPTL(s) display.println(s)
#define DSC(x,y) display.setCursor(x,y)

// Draw text at the center top of the display
#define CENTERTOP(text)                                                          \
        display.getTextBounds(text, 0, 0, &x1, &y1, &width, NULL);               \
        DSC((SCREEN_WIDTH - width) / 2, 1);                                      \ 
        DPTL(F(text))

// Draw text at the center of the display
#define CENTER(text)                                                             \
        display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);            \
        DSC((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);           \ 
        DPTL(F(text))     
// Draw text at center bottom of the display
#define CENTERBOTTOM(text)                                                      \
        display.getTextBounds(text, 0, 0, &x1, &y1, &width, NULL);              \
        DSC((SCREEN_WIDTH - width) / 2,  45 );                                  \ 
        DPTL(F(text))                                                           

// Draw finger name at the top and its current position 
#define FINGER(finger, value)                                                   \
      CENTERTOP(finger);                                                        \
      DSC(52,50);                                                               \ 
      display.setTextSize(1);                                                   \
      DPTL(value);                                                              \
      display.setTextSize(2)

// Draw icon in the center of the screen   
#define ICON(gesture) display.drawBitmap(((SCREEN_WIDTH - 45 ) / 2), ((SCREEN_HEIGHT - 45) / 2), gesture, 45, 45, WHITE)  

          
uint8_t currentMenu = 0;
uint16_t servoCon = 0;
uint8_t currentFinger = -1;
uint8_t maxMenuItems = 0; 
bool btnClick = 0;
int8_t oldPosition = -99;
int8_t cursorPos = 0;
uint8_t servoPos = 0;
uint8_t btnPrev;
uint16_t sensorValue = 0;
uint16_t sensorTreshold = 450;
bool sensorFist = 0;
bool sensorSwitch = 0;
uint8_t encoderMode = 0;
long prevMillis = 0; 
uint16_t sensInterval = 500; // miliseconds

// these are for CENTER/TOP/BOTTOM to find out width and height of the text to calculate the center postion 
int16_t x1;
int16_t y1;
uint16_t width;
uint16_t height;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Declaration for an Micro meastro connected to serial pins(9, 11)
MicroMaestro maestro(maestroSerial);

// Declaration for an arm class 
Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);

// Declaration for an encoder class
Encoder enc(CLK, DT);

// Declaration for an battery clas to get battery percentage and volatage
Battery battery(7410, 8400, A1); // min battery voltage, max battery volatage, analog pin  

// To get a more accurate interval voltage of Arduino 
VoltageReference vRef; 


// Handle the postion reading of encoder
void encoder(){
    // Read data from encoder 
    int16_t pos = enc.read() / 2; // Devided by 2, becouse the reading is by multiplied by 2 

    // if the postion is different than the old one 
    if(pos != oldPosition){

      //Change the behavior of encoder, by changing the encoderMode 
      switch(encoderMode){

        // Use encoder as cursor to switch screens on display
        case 0:
          cursorPos = pos;

          //Min and max screen pages limit 
          if(cursorPos > maxMenuItems){
            cursorPos = maxMenuItems;
            enc.write(maxMenuItems*2);
          }
  
          else if(cursorPos < 0){
            cursorPos = 0;
            enc.write(0);
          }
          break;

        // Use encoder for changing the servo positon  
        case 1:
          if(oldPosition < pos ){
            servoCon >= OPEN ? servoCon = OPEN : servoCon += 200;
          } else if (oldPosition > pos){
            servoCon <= CLOSE ? servoCon = CLOSE : servoCon -= 200;
          }
          break;

        // Use encoder for changing the sensorTreshold 
        case 2:
          oldPosition < pos ? sensorTreshold += 1 : sensorTreshold -= 1;
          break;
          
        }
      oldPosition = pos;
  } 
}

// Display cursors to improve orientation between screen pages 
void cursors(){
  if(cursorPos == 0) {              
        DSC(116,((SCREEN_HEIGHT - 15) / 2) );       
        DPT(F(">"));                                
        }                                           
        else if(cursorPos == maxMenuItems ){        
          DSC(0,((SCREEN_HEIGHT - 15) / 2) );        
          DPT(F("<"));                               
        }                                           
      else{                                          
        DSC(116,((SCREEN_HEIGHT - 15) / 2) );        
        DPT(F(">"));                                 
        DSC(0,((SCREEN_HEIGHT - 15 ) / 2) );         
        DPT(F("<"));                                
      }
}

// Reset the cursor values to 0 
void cursorReset(){
    encoderMode = 0;
    cursorPos = 0;
    enc.write(0);
}

// Handle the encoder button 
void encButton(){
  uint8_t btnCurr = digitalRead(SW);
  btnClick = 0;

  if (btnCurr == LOW && btnPrev == HIGH){
    btnClick = 1;
  }

  btnPrev = digitalRead(SW);
}

// Move with the finger with the encoder 
void fingerMove(){
  if(encoderMode == 1){
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

// Display the main menu 
void menuScreen(){
    display.clearDisplay();
    display.setTextSize(2);
    
    // Switch the screen pages by changing cursor position 
    switch (cursorPos){
    case 0:
      CENTER("MANUAL");
      break;
    case 1:
      CENTER("EMG");
      break;
    case 2:
      CENTER("GESTURES");
      break; 
    }

  cursors();
  display.display();
}

// Display the Manual menu 
void manualScreen(){
  display.setTextSize(2);
  display.clearDisplay();

  // one screen for one finger
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
      CENTER("GO BACK");
      break;  
  }
  
  cursors();
  display.display();
}


// Display the gestures screen
void gesturesScreen(){
  display.clearDisplay();
  display.setTextSize(2);
  switch (cursorPos) {
  case 0:
    ICON(fist);
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
    CENTER("GO BACK");
    break;             
  }

  cursors();
  display.display();
}


// Display the emg screen 
void emgScreen(){
  display.setTextSize(2);
  display.clearDisplay();

  switch (cursorPos) {
  case 0:
      
    CENTERTOP("TRESHOLD");  
    display.setTextSize(1);
    DSC(52, ((SCREEN_HEIGHT - 10) / 2) );
    DPT(sensorTreshold);
    display.setTextSize(2);
    break;
  
  case 1:
    sensorFist? ICON(fist): ICON(hand);
    DSC(52,55);                                                              
    display.setTextSize(1);                                                 
    DPTL(sensorValue);                                                             
    display.setTextSize(2);
    break;
  }
 
  cursors();
  display.display();
}


void menuControl() {
/*
  currentMenu = 0 -> Main menu
  currentMenu = 1 -> Manual  menu 
  currentMenu = 2 -> EMG  menu
  currentMenu = 3 -> Gestures menu
*/

  //Main menu
  if(currentMenu == 0){
    // Set the maximum menu pages 
    maxMenuItems = 2;
    if(btnClick == 1){
        switch (cursorPos){
         // Manual 
        case 0:
          currentMenu = 1; // change the menu to Manual
          cursorReset(); // reset the cursor
          break;
        // EMG
        case 1:
          currentMenu = 2;
          sensorSwitch = 1; // change the sensor flag, to activate control by EMG sensors 
          cursorReset();
          break;  
        // Gestures
        case 2:
          currentMenu = 3;
          cursorReset();
          break;
      }   
    }
  }

  // Manual menu
  else if (currentMenu == 1){
    /
    maxMenuItems = 6;
    if(btnClick == 1){
      // change the encoderMode when you click the encoder, to set the servo values 
      encoderMode != 1 ? encoderMode = 1 : encoderMode = 0;
      servoCon = 0;
     
      switch (cursorPos){
        // Thumb 
        case 0:
            currentFinger = 0;
            enc.write(currentFinger*2); // Set the cursor values back to current screen page
            servoCon += ruka.getThumbPos();            
          break;
        // Index  
        case 1:        
            currentFinger = 1;
            enc.write(currentFinger*2);
            servoCon += ruka.getIndexPos();                     
          break;  
        // Middle
        case 2:                   
            currentFinger = 2;
            enc.write(currentFinger*2);
            servoCon += ruka.getMiddlePos();                   
          break;  
        // Ring
        case 3:                 
            currentFinger = 3;
            enc.write(currentFinger*2);
            servoCon += ruka.getRingPos();                    
          break;  
        // Pinky
        case 4:                       
            currentFinger = 4;
            enc.write(currentFinger*2);
            servoCon += ruka.getPinkyPos();                    
          break;  
        // Wrist
        case 5:       
            currentFinger = 5;
            enc.write(currentFinger*2);
            servoCon += ruka.getWristPos();  
          break;  
        // Go back to main menu
        case 6:
          currentMenu = 0;
          cursorReset();
          break;  
          
        }
    }
  }

  // EMG screen
  else if (currentMenu == 2){
    maxMenuItems = 2;
    if(btnClick == 1){
      switch (cursorPos){
        // Set sensor treshold
        case 0:
          // Change the encoder mode to set sensor treshold value 
          encoderMode != 2 ? encoderMode = 2 : encoderMode = 0;
          
          // Set the cursor value back to current screen page
          if(enc.read() / 2 != 1){
             enc.write(1);
          }
        break;  
        // Fist or hand icon indicator 
        case 1:
          sensorFist = 0; // reset sensorFist flag 
        break;
          
        // Go back to main menu
        case 2:
          sensorSwitch = 0;
          currentMenu = 0;
          cursorReset();
        break;
      }
    }
  }

  //Gestures screen
  else if (currentMenu == 3){
    maxMenuItems = 8;
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
          break;
      }
    }
  }
}

void sensor (){
  // Read the values from EMG sensor
  sensorValue = analogRead(A0);
  unsigned long currentMillis = millis();

  // Swap for fist flag
  if(sensorValue >= sensorTreshold && sensorFist == 0){
    // Wait some time to prevent unwanted switching
    if(currentMillis - prevMillis > sensInterval){
      sensorFist = 1;
      prevMillis = currentMillis;  
    }
  }
  else if (sensorValue >= sensorTreshold && sensorFist == 1){
    if(currentMillis - prevMillis > sensInterval){
      sensorFist = 0;
      prevMillis = currentMillis;  
    }
  } 

}


void setup(){
  // Encoder button
  pinMode(SW, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  
  // Read the previous button state 
  btnPrev = digitalRead(SW);

  // Set the serial baud rate.     
  Serial.begin(115200);
  // Set the maestro serial baud rate
  maestroSerial.begin(9600);
  
  // Read the interal voltage
  vRef.begin();
  
  battery.begin(vRef.readVcc(), 2.0);

  // display allocation 
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }
  
  // clear the display
  display.clearDisplay();
  // set the the text color of display
  display.setTextColor(WHITE);
  // draw a logo 
  display.drawBitmap(0, 0, loadIcon, 128, 64, WHITE);
  display.display();
  delay(2000);
  
  // Open fist when the program starts 
  ruka.openFist();

}

void loop(){
  // when sensor flag is on, than do the reading from sensor 
  if(sensorSwitch){
    sensor();
    
    sensorFist == 1 ? ruka.closeFist() : ruka.openFist();
  }

  encoder();
  encButton();

  menuControl();
  // Draw menu screen by the currentMenu 
  switch (currentMenu) {
    case 0:
      menuScreen();
      break;
    case 1:
      manualScreen();
      fingerMove(); 
      break;
    case 2:
      emgScreen();
      break;
    case 3:
      gesturesScreen();
      break;     
  } 

  // if the battery level is critical, turn on the buzzer
  battery.level() == 1 ? tone(BUZZER, 1000) : noTone(BUZZER);

  delay(10);
}
