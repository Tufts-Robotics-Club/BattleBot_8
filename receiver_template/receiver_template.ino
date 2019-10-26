//receiver code
#include <RHReliableDatagram.h>
#include <RH_NRF24.h> 
#include <SPI.h>

#define TRANSMITTER_ADDRESS 1   
#define RECEIVER_ADDRESS 2

RH_NRF24 RadioDriver;
RHReliableDatagram RadioManager(RadioDriver, RECEIVER_ADDRESS);

void setup()
{
  Serial.begin(9600); 
  if (!RadioManager.init())
    Serial.println("Failed to initialize the transmitter module. Please check the wiring sir.");
}

struct
{
  unsigned int joystickA;
  unsigned int joystickB;
}data;

void loop()
{
  if (RadioManager.available())
  {
    char len,from;
    if (RadioManager.recvfromAck((char*)(&data), &len, &from))
    {
      Serial.print("received: ");
      Serial.print(data.joystickA);
      Serial.print(' ');
      Serial.println(data.joystickB);
    }
  } 
}
