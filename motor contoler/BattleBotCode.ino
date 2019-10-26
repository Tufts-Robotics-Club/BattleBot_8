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

void setup() 
{
  initPin();
}

void loop() 
{
  
  if () {
    // Move forwards
    runMotorL(255);
    runMotorR(255);
  }
  else if () {
    // Move backwards
    runMotorL(-255);
    runMotorR(-255);
  }
  else if () {
    // Spin left
    runMotorL(-255);
    runMotorR(255);
  }
  else if () {
    // Spin right
    runMotorL(255);
    runMotorR(-255);
  }
}
