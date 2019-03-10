#include "AcceleroMMA7361.h"
AcceleroMMA7361 accelero;
#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 2
#define TxD 3

const int ledForward = 9;
const int ledRight = 8;
const int ledBackward = 7;
const int ledLeft = 6;

#define DEBUG_ENABLED  1

String retSymb = "+RTINQ=";//start symble when there's any return
String slaveName = ";SeeedBTSlave";// caution that ';'must be included, and make sure the slave name is right.
int nameIndex = 0;
int addrIndex = 0;

int x;
int y;
int z;

String recvBuf;
String slaveAddr;

String connectCmd = "\r\n+CONN=";

SoftwareSerial blueToothSerial(RxD,TxD);

void setup() 
{ 
  pinMode(ledForward, OUTPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledBackward, OUTPUT);
  pinMode(ledRight, OUTPUT);
  Serial.begin(9600);
  accelero.begin(13, 12, 11, 10, A0, A4, A2);
  accelero.setARefVoltage(5); //sets the AREF voltage to 3.3V
  accelero.setSensitivity(LOW); //sets the sensitivity to +/-6G
  accelero.calibrate();
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();
  //wait 1s and flush the serial buffer
  delay(1000);
  Serial.flush();
  blueToothSerial.flush();
} 

void loop()
{
  char message = 0;
  x = accelero.getXAccel();
  y = accelero.getYAccel();
  z = accelero.getZAccel(); 
  if (y <= -50) {
    message = 1;
    backward();
    if (x >= 25) {
      message = 5;
      backwardLeft();
    }
    if (x <= -25) {
      message = 6;
      backwardRight();
    }
  }
  else if (y >= 50) {
    message = 2;
    forward();
    if (x >= 25) {
      message = 7;
      forwardLeft();
    }
    if (x <= -25) {
      message = 8;
      forwardRight();
    }
  }
  else if ((y < 50)&&(y > -50)) {
    if (x >= 50) {
      message = 3;
      left();
    }
    else if (x <= -50) {
      message = 0;
      right();
    }
    else {
      message = 4;
      halt();
    }
  }
  blueToothSerial.write(message);
  Serial.print("x = ");
  Serial.print(x);
  Serial.print(", y = ");
  Serial.print(y);
  Serial.print(", message = ");
  Serial.println(message);
  delay(250);
}

void forward() {
  digitalWrite(ledForward, HIGH);
  digitalWrite(ledBackward, LOW);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
}
void backward() {
  digitalWrite(ledForward, LOW);
  digitalWrite(ledBackward, HIGH);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
}
void halt() {
  digitalWrite(ledForward, LOW);
  digitalWrite(ledBackward, LOW);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
}
void right() {
  digitalWrite(ledForward, LOW);
  digitalWrite(ledBackward, LOW);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, HIGH);
}
void left () {
  digitalWrite(ledForward, LOW);
  digitalWrite(ledBackward, LOW);
  digitalWrite(ledLeft, HIGH);
  digitalWrite(ledRight, LOW);
}
void forwardRight() {
  digitalWrite(ledForward, HIGH);
  digitalWrite(ledBackward, LOW);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, HIGH);
}
void forwardLeft() {
  digitalWrite(ledForward, HIGH);
  digitalWrite(ledBackward, LOW);
  digitalWrite(ledLeft, HIGH);
  digitalWrite(ledRight, LOW);
}
void backwardRight() {
  digitalWrite(ledForward, LOW);
  digitalWrite(ledBackward, HIGH);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, HIGH);
}
void backwardLeft() {
  digitalWrite(ledForward, LOW);
  digitalWrite(ledBackward, HIGH);
  digitalWrite(ledLeft, HIGH);
  digitalWrite(ledRight, LOW);
}

void setupBlueToothConnection()
{
  blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.print("\r\n+STWMOD=1\r\n");//set the bluetooth work in master mode
  blueToothSerial.print("\r\n+STNA=SeeedBTMaster\r\n");//set the bluetooth name as "SeeedBTMaster"

  blueToothSerial.print("\r\n+STAUTO=0\r\n");// Auto-connection is forbidden here
  delay(2000); // This delay is required.
  blueToothSerial.flush();
  blueToothSerial.print("\r\n+INQ=1\r\n");//make the master inquire
  Serial.println("Master is inquiring!");
  delay(2000); // This delay is required.

  //find the target slave
  char recvChar;
  while(1){
    if(blueToothSerial.available()){
      recvChar = blueToothSerial.read();
      recvBuf += recvChar;
      nameIndex = recvBuf.indexOf(slaveName);//get the position of slave name
      Serial.println(nameIndex);
      //nameIndex -= 1;//decrease the ';' in front of the slave name, to get the position of the end of the slave address
      if ( nameIndex != -1 ){
        //Serial.print(recvBuf);
        addrIndex = (recvBuf.indexOf(retSymb,(nameIndex - retSymb.length()- 18) ) + retSymb.length());//get the start position of slave address	 		
        slaveAddr = recvBuf.substring(addrIndex, nameIndex);//get the string of slave address 			
        break;
      }
    }
  }
  //form the full connection command
  connectCmd += slaveAddr;
  connectCmd += "\r\n";
  int connectOK = 0;
  Serial.print("Connecting to slave:");
  Serial.print(slaveAddr);
  Serial.println(slaveName);
  //connecting the slave till they are connected
  do{
    blueToothSerial.print(connectCmd);//send connection command
    recvBuf = "";
    while(1){
      if(blueToothSerial.available()){
        recvChar = blueToothSerial.read();
        recvBuf += recvChar;
        if(recvBuf.indexOf("CONNECT:OK") != -1){
          connectOK = 1;
          Serial.println("Connected!");
          blueToothSerial.print("Connected!");
          break;
        }
        else if(recvBuf.indexOf("CONNECT:FAIL") != -1){
          Serial.println("Connect again!");
          break;
        }
      }
    }
  }
  while(0 == connectOK);


}
