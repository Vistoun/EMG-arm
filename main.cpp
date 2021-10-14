#include <Arduino.h>
#include <PololuMaestro.h>

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

MicroMaestro maestro(maestroSerial);


void setup()
{

  // Set the serial baud rate.
  maestroSerial.begin(9600);
}

void loop()
{
 
  /* setTarget takes the channel number you want to control, and
     the target position in units of 1/4 microseconds. A typical
     RC hobby servo responds to pulses between 1 ms (4000) and 2
     ms (8000). */

  maestro.setTarget(1, 6000);
  /* maestro.setTarget(2, 6000);
  maestro.setTarget(3, 6000);
  maestro.setTarget(4, 6000);
  maestro.setTarget(5, 6000); */
  delay(2000);

  // Set the target of channel 0 to 1750 us, and wait 2 seconds.
  maestro.setTarget(1, 7000);
  /* maestro.setTarget(2, 7000);
  maestro.setTarget(3, 7000);
  maestro.setTarget(4, 7000);
  maestro.setTarget(5, 7000); */
  delay(2000);

  // Set the target of channel 0 to 1250 us, and wait 2 seconds.
  maestro.setTarget(1, 5000);
 /* maestro.setTarget(2, 5000);
  maestro.setTarget(3, 5000);
  maestro.setTarget(4, 5000);
  maestro.setTarget(5, 5000); */ 
  delay(2000);
}