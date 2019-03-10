#include <SPI.h>  
#include <Pixy.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define RxD 2
#define TxD 4

Pixy pixy;
int mode = 2;
const int toggleMode = 3;

SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello Pixy");

  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  pinMode(toggleMode, INPUT);
  setupBlueToothConnection();

 Wire.begin();
}

void loop() {
  Serial.println("loop");
  byte command;

  whichMode();

  byte remote = remoteControlMode();
  byte pix = pixyMode();

  if(mode == 1) {
    
    Serial.print("Pixy: ");
    command = pix;
  }
  else {
    Serial.print("Remote: ");
    command = remote;
  }
  Serial.print(" command=");
  Serial.println(command);

  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(command);       // sends one byte  
  Wire.endTransmission();    // stop transmitting

  delay(50);
}

byte pixyMode() {
  uint16_t blocks = pixy.getBlocks();  
  byte command = 4; //stop by default unless I see something
  if (blocks) //if pixy sees something (doesnt have to be a block xD )
  {
    int width = pixy.blocks[0].width;
    int x     = pixy.blocks[0].x;

    Serial.print("Pixy object: width=");
    Serial.print(width);
    Serial.print(" x=");
    Serial.print(x);

    if (width > 30) {
      if ((x > 100) && (x < 200)){
        command = 1; // forward
      }
      else if (x < 100) {
        command = 5; // forward left
      }
      else if (x > 200) {
        command = 6; // forward right
      }
    }
  }
  return command;
}

byte remoteControlMode() {
  byte input;
  while(blueToothSerial.available()){//check if there's any data sent from the remote bluetooth shield
    input = blueToothSerial.read();
    Serial.print("Received: ");
    Serial.println((byte)input);
  }
  return input;
}


void whichMode() {
  boolean val;
  val = digitalRead(toggleMode);
  if(val == HIGH) {
    mode = 1;
    Serial.print("Remote Mode");
  }
  if(val == LOW) {
    mode = 2; 
    Serial.print("Pixy Mode");
  }
}


void setupBlueToothConnection()
{
  blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STNA=SeeedBTSlave\r\n"); //set the bluetooth name as "SeeedBTSlave"
  blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
  delay(2000); // This delay is required.
  blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
  Serial.println("The slave bluetooth is inquirable!");
  delay(2000); // This delay is required.
  blueToothSerial.flush();
}
