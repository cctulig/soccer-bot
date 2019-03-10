#include <SoftwareSerial.h>
#include <Wire.h>

const int L_MOTOR = 12;
const int R_MOTOR = 13;
const int L_BRAKE = 9;
const int R_BRAKE = 8;
const int L_POWER = 3;
const int R_POWER = 11;

int mode = 1;

const int L_HIGH_SPEED = 255;
const int R_HIGH_SPEED = 245;
const int L_LOW_SPEED  = L_HIGH_SPEED / 2;
const int R_LOW_SPEED  = R_HIGH_SPEED / 2;

const boolean FORWARD = HIGH;
const boolean REVERSE = LOW;

void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Starting Soccer Bot");

  // Motor Shield Setup
  pinMode(L_MOTOR, OUTPUT);
  pinMode(L_BRAKE, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  pinMode(R_BRAKE, OUTPUT);

  halt();
  
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent);
}

void loop() {

}

void forward()
{
  digitalWrite(L_MOTOR, FORWARD);
  digitalWrite(R_MOTOR, FORWARD);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_HIGH_SPEED);
  analogWrite(R_POWER, R_HIGH_SPEED); 
}

void halt()
{
  digitalWrite(L_BRAKE, HIGH);
  digitalWrite(R_BRAKE, HIGH);
  analogWrite(L_POWER, 0);
  analogWrite(R_POWER, 0); 

}
void turnRight()
{
  digitalWrite(L_MOTOR, FORWARD);
  digitalWrite(R_MOTOR, REVERSE);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_LOW_SPEED);
  analogWrite(R_POWER, L_LOW_SPEED);
}
void backward()
{
  digitalWrite(L_MOTOR, REVERSE);
  digitalWrite(R_MOTOR, REVERSE);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_HIGH_SPEED);
  analogWrite(R_POWER, R_HIGH_SPEED); 
}
void turnLeft()
{
  digitalWrite(L_MOTOR, REVERSE);
  digitalWrite(R_MOTOR, FORWARD);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_LOW_SPEED);
  analogWrite(R_POWER, L_LOW_SPEED);
}
void forwardTurnLeft()
{
  digitalWrite(L_MOTOR, FORWARD);
  digitalWrite(R_MOTOR, FORWARD);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_LOW_SPEED);
  analogWrite(R_POWER, R_HIGH_SPEED); 
}
void forwardTurnRight()
{
  digitalWrite(L_MOTOR, FORWARD);
  digitalWrite(R_MOTOR, FORWARD);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_HIGH_SPEED);
  analogWrite(R_POWER, R_LOW_SPEED); 
}
void backwardTurnLeft()
{
  digitalWrite(L_MOTOR, REVERSE);
  digitalWrite(R_MOTOR, REVERSE);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_LOW_SPEED);
  analogWrite(R_POWER, R_HIGH_SPEED); 
}
void backwardTurnRight()
{
  digitalWrite(L_MOTOR, REVERSE);
  digitalWrite(R_MOTOR, REVERSE);

  digitalWrite(L_BRAKE, LOW);
  digitalWrite(R_BRAKE, LOW);

  analogWrite(L_POWER, L_HIGH_SPEED);
  analogWrite(R_POWER, R_LOW_SPEED);  
}

void receiveEvent(int howMany)
{
  Serial.println("Connected with eye");
  Serial.println(howMany);
  while(Wire.available() > 0) // loop through all bytes sent
  {
    byte input = Wire.read(); // receive byte as a character
    Serial.print(input);         // print the character
     if (input == 0) {
      turnRight();
    }
    else if (input == 1) {
      forward();        
    }
    else if (input == 2) {
      backward();
    }
    else if (input == 3) {
      turnLeft();
    }
    else if (input == 4) {
      halt();
    }
    else if (input == 5) {
      forwardTurnLeft();
    }
    else if (input == 6) {
      forwardTurnRight();
    }
    else if (input == 7) {
      backwardTurnLeft();
    }
    else if (input == 8) {
      backwardTurnRight();
    }
  }
}

