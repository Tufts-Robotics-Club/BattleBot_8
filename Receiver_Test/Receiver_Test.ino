//receiver code
#include <RHReliableDatagram.h>
#include <RH_NRF24.h> 
#include <SPI.h>

#define TRANSMITTER_ADDRESS 1   
#define RECEIVER_ADDRESS 2

RH_NRF24 RadioDriver;
RHReliableDatagram RadioManager(RadioDriver, RECEIVER_ADDRESS);

void runMotorL(int Speed) {
  if (Speed < 0) {
    digitalWrite(7,LOW);
    analogWrite(9,-1*Speed);
  }
  else if (Speed > 0) {
    digitalWrite(7,HIGH);
    analogWrite(9,Speed);
  }
  else {
    analogWrite(9, 0);
  }
}

void runMotorR(int Speed) {
  if (Speed < 0) {
    digitalWrite(8,LOW);
    analogWrite(10,-1*Speed);
  }
  else if (Speed > 0) {
    digitalWrite(8,HIGH);
    analogWrite(10,Speed);
  }
  else {
    analogWrite(10,0);
  }
}

void initPin() {
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);

  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);

  digitalWrite(4,HIGH);
  
  analogWrite(10,0);
  analogWrite(9,0);
}

void setup(){
  initPin();
  Serial.begin(9600); 
  if (!RadioManager.init())
    Serial.println("Failed to initialize the transmitter module. Please check the wiring sir.");
}

struct {
  unsigned int joystickA;
  unsigned int joystickB;
}data;

void loop() {
  if (RadioManager.available()) {
    char len,from;
    if (RadioManager.recvfromAck((char*)(&data), &len, &from)) {
      Serial.print("received: ");
      Serial.print(data.joystickA);
      Serial.print(' ');
      Serial.println(data.joystickB);
      runMotorL(data.joystickA);
      runMotorR(data.joystickB);
    }
  } 
}
