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
#define OPEN 8000  // value when is finger is fully open
#define CLOSE 3968 // value when is fully closed

// OLED pins
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_ADDR 0x3C   // OLED I2C address

// Encoder pins
#define CLK 3
#define DT 2
#define SW 4 // this port is used for button function on encoder

#define PT(s) Serial.print(s) //makes life easier
#define PTL(s) Serial.println(s)

#define DPT(s) display.print(s)
#define DPTL(s) display.println(s)
#define DSC(x, y) display.setCursor(x, y)

#define CENTERTOP(text)                                      \
  display.getTextBounds(text, 0, 0, &x1, &y1, &width, NULL); \
  DSC((SCREEN_WIDTH - width) / 2, 1);                        \
  \ 
        DPTL(F(text))

#define CENTER(text)                                             \
  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);  \
  DSC((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2); \
  \ 
        DPTL(F(text))

#define CENTERBOTTOM(text)                                   \
  display.getTextBounds(text, 0, 0, &x1, &y1, &width, NULL); \
  DSC((SCREEN_WIDTH - width) / 2, 45);                       \
  \ 
        DPTL(F(text))

#define FINGER(finger, value)                                 \
  CENTERTOP(finger);                                          \
  DSC(((SCREEN_WIDTH - 55) / 2), ((SCREEN_HEIGHT + 30) / 2)); \
  DPTL(value)

#define CURSORS()                             \
  if (cursorPos == 0)                         \
  {                                           \
    DSC(116, ((SCREEN_HEIGHT - 15) / 2));     \
    DPT(F(">"));                              \
  }                                           \
  \ 
        else if (cursorPos == maxMenuItems)   \
  {                                           \
    \ 
          DSC(0, ((SCREEN_HEIGHT - 15) / 2)); \
    \ 
          DPT(F("<"));                        \
    \ 
                                                \
  }                                           \
  else                                        \
  {                                           \
    \ 
        DSC(116, ((SCREEN_HEIGHT - 15) / 2)); \
    \ 
        DPT(F(">"));                          \
    \ 
        DSC(0, ((SCREEN_HEIGHT - 15) / 2));   \
    \ 
        DPT(F("<"));                          \
  }

#define ICON(gesture) display.drawBitmap(((SCREEN_WIDTH - 45) / 2), ((SCREEN_HEIGHT - 45) / 2), gesture, 45, 45, WHITE)

uint8_t currentMenu = 0;
uint16_t servoCon = 0;
uint8_t currentFinger = -1;
uint8_t maxMenuItems = 0;
bool btnClick = 0;
int8_t oldPosition = -99;
uint8_t cursorPos = 0;
uint8_t servoPos = 0;
uint8_t btnPrev;
uint16_t sensorValue = 0;
uint16_t sensorTreshold = 300;
bool sensorFist = 0;
bool sensorSwitch = 0;
uint16_t valMax = 450;
uint8_t valMin = 100;
uint8_t encoderMode = 0;
long prevMillis = 0;
uint16_t sensInterval = 250; // miliseconds

int16_t x1;
int16_t y1;
uint16_t width;
uint16_t height;

// Declaration for an Micro meastro connected to serial pins(9, 8)
MicroMaestro maestro(maestroSerial);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Declaration for an arm class
Arm ruka(WRIST, THUMB, INDEX, MIDDLE, RING, PINKY, SPEED, ACCELERATION, OPEN, CLOSE);

// Declaration for and encoder class
Encoder enc(DT, CLK);

void encoder()
{
  int16_t pos = enc.read() / 2;

  if (pos != oldPosition)
  {
    switch (encoderMode)
    {
    case 0:
      cursorPos = pos;
      if (cursorPos >= maxMenuItems)
      {
        cursorPos = maxMenuItems;
        enc.write(maxMenuItems * 2);
      }
      else if (cursorPos <= 0)
      {
        cursorPos = 0;
        enc.write(0);
      }
      break;

    case 1:
      if (oldPosition < pos)
      {
        servoCon >= OPEN ? servoCon = OPEN : servoCon += 200;
      }
      else if (oldPosition > pos)
      {
        servoCon <= CLOSE ? servoCon = CLOSE : servoCon -= 200;
      }
      oldPosition = pos;
      break;

    case 2:
      oldPosition < pos ? sensorTreshold += 1 : sensorTreshold -= 1;
      break;
    }
    oldPosition = pos;
  }
}

void cursorReset()
{
  encoderMode = 0;
  cursorPos = 0;
  enc.write(0);
}

void btnCheck()
{
  uint8_t btnCurr = digitalRead(SW);
  btnClick = 0;

  if (btnCurr == LOW && btnPrev == HIGH)
  {
    btnClick = 1;
  }

  btnPrev = digitalRead(SW);
}

void fingerMov()
{
  if (encoderMode == 1)
  {
    switch (currentFinger)
    {
    case 0:
      ruka.moveFinger(THUMB, servoCon);
      break;

    case 1:
      ruka.moveFinger(INDEX, servoCon);
      break;

    case 2:
      ruka.moveFinger(MIDDLE, servoCon);
      break;

    case 3:
      ruka.moveFinger(RING, servoCon);
      break;

    case 4:
      ruka.moveFinger(PINKY, servoCon);
      break;

    case 5:
      ruka.moveFinger(WRIST, servoCon);
      break;
    }
  }
}

void startScreen()
{
  display.clearDisplay();
  display.setTextSize(2);

  //CENTERTOP("EMG - HAND");

  switch (cursorPos)
  {
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

  CURSORS();

  display.display();
}

void manualScreen()
{
  display.setTextSize(2);
  display.clearDisplay();
  // each screen for one finger
  switch (cursorPos)
  {
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
    FINGER("WRIST", ruka.getWristPos());
    break;
  case 6:
    CENTER("GO BACK");
    break;
  }

  CURSORS();

  display.display();
}

void gesturesScreen()
{
  display.clearDisplay();
  display.setTextSize(2);
  switch (cursorPos)
  {
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

  CURSORS();

  display.display();
}

void emgScreen()
{
  display.setTextSize(2);
  display.clearDisplay();

  switch (cursorPos)
  {
  case 0:
    CENTERTOP("SENSOR");

    if (sensorSwitch)
    {
      CENTERBOTTOM("ON");
    }
    else
    {
      CENTERBOTTOM("OFF");
    }
    break;

  case 1:
    CENTERTOP("TRESHOLD");
    DSC(50, ((SCREEN_HEIGHT - 10) / 2));
    DPT(sensorTreshold);

    break;

  case 2:
    CENTERTOP("VALUE");

    DSC(60, ((SCREEN_HEIGHT - 10) / 2));
    DPT(sensorValue);

    display.setTextSize(1);
    DSC(5, 50);
    DPT("MAX:");
    DSC(30, 50);
    DPTL(valMax);

    DSC(80, 50);
    DPT("MIN:");
    DSC(105, 50);
    DPT(valMin);
    display.setTextSize(2);

    break;

  case 3:
    CENTER("GO BACK");
    break;
  }

  CURSORS();

  display.display();
}

void menuControl()
{

  /*
  currentMenu = 0 -> Main menu
  currentMenu = 1 -> Manual handel menu 
  currentMenu = 2 -> EMG senzor menu
  currentMenu = 3 -> Gestures menu
*/

  //Main menu
  if (currentMenu == 0)
  {
    maxMenuItems = 2;
    if (btnClick == 1)
    {
      switch (cursorPos)
      {
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
  else if (currentMenu == 1)
  {
    maxMenuItems = 6;
    if (btnClick == 1)
    {
      encoderMode != 1 ? encoderMode = 1 : encoderMode = 0;
      servoCon = 0;

      switch (cursorPos)
      {
      case 0:
        currentFinger = 0;
        enc.write(currentFinger * 2);
        servoCon += ruka.getThumbPos();
        break;

      case 1:
        currentFinger = 1;
        enc.write(currentFinger * 2);
        servoCon += ruka.getIndexPos();
        break;

      case 2:
        currentFinger = 2;
        enc.write(currentFinger * 2);
        servoCon += ruka.getMiddlePos();
        break;

      case 3:
        currentFinger = 3;
        enc.write(currentFinger * 2);
        servoCon += ruka.getRingPos();
        break;

      case 4:
        currentFinger = 4;
        enc.write(currentFinger * 2);
        servoCon += ruka.getPinkyPos();
        break;

      case 5:
        currentFinger = 5;
        enc.write(currentFinger * 2);
        servoCon += ruka.getWristPos();
        break;

      case 6:
        currentMenu = 0;
        cursorReset();
        break;
      }
    }
  }

  // EMG screen
  else if (currentMenu == 2)
  {
    maxMenuItems = 3;
    if (btnClick == 1)
    {
      switch (cursorPos)
      {
      case 0:
        sensorSwitch != 1 ? sensorSwitch = 1 : sensorSwitch = 0;
        break;

      case 1:
        encoderMode != 2 ? encoderMode = 2 : encoderMode = 0;
        if (enc.read() / 2 != 2)
          enc.write(2);

        break;

      case 2:
        valMax = 350;
        valMin = 0;
        break;

      case 3:

        currentMenu = 0;
        cursorReset();
        break;
      }
    }
  }

  //Gestures screen
  else if (currentMenu == 3)
  {
    maxMenuItems = 8;
    if (btnClick == 1)
    {
      switch (cursorPos)
      {
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

void sensor()
{
  sensorValue = analogRead(A0);
  unsigned long currentMillis = millis();

  if (sensorValue >= sensorTreshold && sensorFist == 0)
  {
    //sensorFist = 1;
    if (currentMillis - prevMillis > sensInterval)
    {
      sensorFist = 1;
    }
  }
  else if (sensorValue >= sensorTreshold && sensorFist == 1)
  {
    //sensorFist = 0;
    if (currentMillis - prevMillis > sensInterval)
    {
      sensorFist = 0;
    }
  }

  if (sensorValue > valMax)
  {
    valMax = sensorValue;
  }
  else if (sensorValue < valMin)
  {
    valMin = sensorValue;
  }
}

void setup()
{
  pinMode(SW, INPUT_PULLUP);
  btnPrev = digitalRead(SW);

  // Set the serial baud rate.
  Serial.begin(115200);
  maestroSerial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, loadIcon, 128, 64, WHITE);
  display.display();
  delay(2000);

  ruka.openFist();
}

void loop()
{

  if (sensorSwitch)
  {
    sensor();
    sensorFist == 1 ? ruka.closeFist() : ruka.openFist();
  }

  encoder();
  btnCheck();
  menuControl();

  switch (currentMenu)
  {
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
  //PTL(sensorFist);
  delay(10);
}
