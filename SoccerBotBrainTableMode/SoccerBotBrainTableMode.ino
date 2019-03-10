#include <SPI.h>  
#include <Pixy.h>
#include <Wire.h>
#include <SoftwareSerial.h>

Pixy pixy;

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello Pixy");

  Wire.begin();
}

void loop() {
  byte command = pixyMode();
    
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
      if ((x >= 60) && (x <= 210)){
        command = 4; // forward
      }
      else if (x < 60) {
        command = 3; // forward left
      }
      else if (x > 21 0) {
        command = 0; // forward right
      }
    }
  }
  return command;
}





