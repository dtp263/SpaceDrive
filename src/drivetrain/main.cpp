#include <Arduino.h>
#include <DualMotorController.h>
#include <DrivePacket.h>
#include <Wire.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>

#include<SoftwareSerial.h>
SoftwareSerial mySUART(2, 3); //mySUART(SRX, STX)
DualMotorController motorController = DualMotorController(6, 5);

SSD1306ScreenWriter oledScreenLeft = SSD1306ScreenWriter(0);
SSD1306ScreenWriter oledScreenRight = SSD1306ScreenWriter(1);


DrivePacket drivePacket;
char *drivePacketBuffer = (char*)malloc(DRIVE_PACKET_SIZE);

void setup() {
  Serial.begin(9600);
  mySUART.begin(9600);

  // Start I2C communication with the Multiplexer
  Wire.begin();

  delay(1000);
    
  motorController.Setup();
  oledScreenLeft.Setup();
  oledScreenRight.Setup();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  delay(10);
  digitalWrite(8, LOW);
}


void loop() {
  if (mySUART.available()) {
    mySUART.setTimeout(2000);
    int returnBytes = mySUART.readBytes(drivePacketBuffer, DRIVE_PACKET_SIZE);
    drivePacket = DrivePacket::Deserialize(String(drivePacketBuffer));

    if(drivePacket.Data.leftMotorPower >= 20) digitalWrite(LED_BUILTIN, HIGH);  // switch LED On
    if(drivePacket.Data.leftMotorPower < 20) digitalWrite(LED_BUILTIN, LOW);   // switch LED Off

    Serial.print(drivePacket.Data.leftMotorPower);
    Serial.print(",");
    Serial.print(drivePacket.Data.rightMotorPower);
    Serial.print("\n");

    motorController.WritePowerToMotorAsPercentage(
      drivePacket.Data.leftMotorPower,
      drivePacket.Data.rightMotorPower
    );
  }

  oledScreenLeft.WriteInt(drivePacket.Data.leftMotorPower);
  oledScreenRight.WriteInt(drivePacket.Data.rightMotorPower);
  
  

  // motorController.WritePowerToMotorAsPercentage(0, 0);
}
