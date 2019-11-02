//receiver code
#include <RHReliableDatagram.h>
#include <RH_NRF24.h> 
#include <SPI.h>

#define TRANSMITTER_ADDRESS 1   
#define RECEIVER_ADDRESS 2

RH_NRF24 RadioDriver;
RHReliableDatagram RadioManager(RadioDriver, RECEIVER_ADDRESS);

#define MotorPWM_R 5
#define MotorINA_R 4
#define MotorINB_R 3

#define MotorPWM_L 6
#define MotorINA_L 7
#define MotorINB_L 2

void runMotorL(int Speed) {
  if (Speed < 0) 
  {
    digitalWrite(MotorINA_L,HIGH);
    digitalWrite(MotorINB_L,LOW);
    analogWrite(MotorPWM_L,-Speed);
  }
  else if (Speed > 0) 
  {
    digitalWrite(MotorINA_L,LOW);
    digitalWrite(MotorINB_L,HIGH);
    analogWrite(MotorPWM_L,Speed);
  }
  else 
  {
    digitalWrite(MotorINA_L,LOW);
    digitalWrite(MotorINB_L,LOW);
    analogWrite(MotorPWM_L,0);
  }
}

void runMotorR(int Speed) {
  if (Speed < 0) {
    digitalWrite(MotorINA_R,HIGH);
    digitalWrite(MotorINB_R,LOW);
    analogWrite(MotorPWM_R,-Speed);
  }
  else if (Speed > 0) {
    digitalWrite(MotorINA_R,LOW);
    digitalWrite(MotorINB_R,HIGH);
    analogWrite(MotorPWM_R,Speed);
  }
  else {
    
    digitalWrite(MotorINA_R,LOW);
    digitalWrite(MotorINB_R,LOW);
    analogWrite(MotorPWM_R,0);
  }
}


void initPin() 
{  
  pinMode(MotorPWM_R,OUTPUT);
  pinMode(MotorINA_R,OUTPUT);
  pinMode(MotorINB_R,OUTPUT);

  pinMode(MotorPWM_L,OUTPUT);
  pinMode(MotorINA_L,OUTPUT);
  pinMode(MotorINB_L,OUTPUT);  
  
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  
  runMotorR(0);
  runMotorL(0);
}

void setup()
{
  Serial.begin(9600); 
  if (!RadioManager.init())
    Serial.println("Failed to initialize the transmitter module. Please check the wiring sir.");

  initPin(); 
}

struct
{
  int motorSpeed_R;
  int motorSpeed_L;
  char drillServoAngle;
}data;

unsigned long timoutTime= 2000;
unsigned long lastReceivedTime = 0; //milliseconds

void loop()
{

  //if the robot hasnt received any data for a long 
  //time, stop the motors please
  if(millis() - lastReceivedTime > timoutTime)
  {
   runMotorL(0);
   runMotorR(0);
  }
  
  if (RadioManager.available())
  {
    lastReceivedTime = millis();
    char len,from;
    if (RadioManager.recvfromAck((char*)(&data), &len, &from))
    {
      Serial.println(data.motorSpeed_R);
      Serial.println(data.motorSpeed_L);
      Serial.println(" ");

      if(data.motorSpeed_R >= 255)
      data.motorSpeed_R = 255;
      if(data.motorSpeed_L >= 255)
      data.motorSpeed_L = 255;


     runMotorR(data.motorSpeed_R); 
     runMotorL(data.motorSpeed_L);  
    }

  } 
}
