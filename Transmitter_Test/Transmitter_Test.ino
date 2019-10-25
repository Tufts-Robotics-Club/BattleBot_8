//transmitter code
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define TRANSMITTER_ADDRESS 1   
#define RECEIVER_ADDRESS 2

RH_NRF24 RadioDriver;
RHReliableDatagram RadioManager(RadioDriver, TRANSMITTER_ADDRESS);
 
void setup()
{
  Serial.begin(9600); 
  if (!RadioManager.init())
    Serial.println("Failed to initialize the receiver module. Please check the wiring sir.");
}

struct
{
  unsigned int joystickA;
  unsigned int joystickB;
}data;

void loop()
{
  data.joystickA = map(255, 0, 1023, 255, 0);
  data.joystickB = map(255, 0, 1023, 255, 0);
  
  if (RadioManager.sendtoWait((char*)(&data), sizeof(data), RECEIVER_ADDRESS))
  {
    Serial.print("sent joystick values: ");
    Serial.print((int)data.joystickA);
    Serial.println((int)data.joystickB);
  }
}
