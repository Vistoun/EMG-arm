#include <Arduino.h>
#include <PololuMaestro.h>
#include "Arm.hpp"
#include <string.h>

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
 
const unsigned int SPEED = 0; // 0 is fastest
const unsigned int ACCELERATION = 0;

const unsigned int OPEN = 8000;
const unsigned int CLOSE = 3968;

int userFinger = 0;
int userPos = 0;


MicroMaestro maestro(maestroSerial);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);

int stage = 0;


//int value = 0;
void setup(){

  // Set the serial baud rate.     
  Serial.begin(9600);
  maestroSerial.begin(9600);
  if(stage == 0){
    PTL("Pro pohyb s rukou zadejte Y");
  }
  
}

void loop(){

  char input = Serial.read();
  if(input == 'Y' || input == 'y' ){
    delay(100);
    stage += 1;
  }

  if(stage == 1){
    PTL("Zadejte kanal prstu, ktery chcete pouzivat: ");
    userFinger = Serial.read();
    stage += 1;
    delay(100);
  }

  if(stage == 2){
    PTL("Zadejte pozici: ");
    userPos = Serial.read();
    stage += 1;    
    delay(100);
  }

  if(stage == 3){
    maestro.setTarget(userFinger, userPos);
    PTL("Pokud chcete pokracovat, tak zadejte Y: ");
    if(input == 'Y' || input == 'y' ){
      stage = 1;
    }
    delay(100);
  }
 delay(1000);
}