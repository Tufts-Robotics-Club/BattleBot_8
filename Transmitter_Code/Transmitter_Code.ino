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
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(7, LOW);
  Serial.begin(9600); 
  if (!RadioManager.init())    
    Serial.println("Failed to initialize the receiver module. Please check the wiring sir.");
}

struct
{
  int motorSpeed_R;
  int motorSpeed_L;
  char drillServoAngle;
}data;

//when the joystick is at the center position, the analog 
//reading will not read exactly center since it might be slightly off 
//the center even if we do not touch the joystick. So zeroOffset()
//function make the reading equal to the center if it is between center + offset
//and center - offset
int zeroOffset(int val,int offset, int center)
{
  if(val > center - offset && val < center + offset) {
    if (center < 512) {
      return val + (512 - center);
    }
    if (center > 512) {
      return val - (center - 512);
    }
  }
   return val;
}
int arcade_drive_L(int throttle, int pitch)
{
   int value = throttle+pitch;
  if (value > 255)
    return 255;
  if (value < -255)
    return -255;
  return value;
}
int arcade_drive_R(int throttle, int pitch)
{
  int value = throttle-pitch;
  if (value > 255)
    return 255;
  if (value < -255)
    return -255;
  return value;
}
void loop()
{
  int joyStickRight_X = analogRead(A0);
  int joyStickRight_Y = analogRead(A1);
  int joyStickLeft_X  = analogRead(A2);
  int joyStickLeft_Y = analogRead(A3);
  int offset = 40, R_center = 467, L_center = 533, center = 512;
  joyStickRight_X = zeroOffset(joyStickRight_X,offset,R_center);
  joyStickRight_Y = zeroOffset(joyStickRight_Y,offset,R_center);
  joyStickLeft_X = zeroOffset(joyStickLeft_X,offset,L_center);
  joyStickLeft_Y = zeroOffset(joyStickLeft_Y,offset,L_center);
  //set the sensitivity of the joystick by using the readings from the pots
  
  //write to the LED
  int pot = analogRead(A4);
  analogWrite(6, (pot/4)*(3.3/5.0));

  //joystick readings are between 0 and 1024
  //scale the readings to -255 and 255
  int throttle = -((joyStickRight_X - 512)/2);
  int pitch = -((joyStickLeft_Y - 512)/2);
  
  //meld the values for arcade drive
  data.motorSpeed_R = arcade_drive_R(throttle, pitch);
  data.motorSpeed_L = arcade_drive_L(throttle, pitch);
  
  //scale with pot
  data.motorSpeed_R = (int)(((float)data.motorSpeed_R * (float)pot)/1023.0f);
  data.motorSpeed_L = (int)(((float)data.motorSpeed_L * (float)pot)/1023.0f);
  //shoot the data
  RadioManager.sendtoWait((char*)(&data), sizeof(data), RECEIVER_ADDRESS);

  //delay is recommended to prevent sending millions of data packets
  //all the time so that packets do not get stacked in the receivers buffer
  delay(100);
}
