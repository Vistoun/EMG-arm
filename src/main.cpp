#include <Arduino.h>
#include <PololuMaestro.h>
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


const int WRIST = 0;
const int THUMB = 1;
const int INDEX = 2;
const int MIDDLE = 3;
const int RING = 4;
const int PINKY = 5;
 
const int SPEED = 0; // 0 is fastest
const int ACCELERATION = 0;

const int OPEN = 8000;
const int CLOSE = 3968;


MicroMaestro maestro(maestroSerial);

Arm ruka(WRIST,THUMB,INDEX,MIDDLE,RING,PINKY,SPEED,ACCELERATION,OPEN,CLOSE);




void setup(){

  // Set the serial baud rate.
  maestroSerial.begin(9600);
  
}

void loop(){
 
  ruka.closeFist();
  delay(2000);
  ruka.openFist();
  delay(2000);
  
}