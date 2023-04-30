#include <Arduino.h>
#include <DualMotorController.h>
#include <DrivePacket.h>
#include <RelayBoard.h>
#include <Wire.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>

#include<SoftwareSerial.h>
// SoftwareSerial Serial2(2, 3); //Serial2(SRX, STX)
DualMotorController motorController = DualMotorController(1, 0);

SSD1306ScreenWriter oledScreenLeft = SSD1306ScreenWriter(7);
SSD1306ScreenWriter oledScreenRight = SSD1306ScreenWriter(6);

int relayPins[8] = {12,11,10,9,8,7,6,5};
EightChannelRelayBoard relayBoard = EightChannelRelayBoard(relayPins);


DrivePacket drivePacket;
char *drivePacketBuffer = (char*)malloc(DRIVE_PACKET_SIZE);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  // Start I2C communication with the Multiplexer
  Wire.begin();
    
  motorController.Setup();
  oledScreenLeft.Setup();
  oledScreenRight.Setup();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // relayBoard.Setup();
}


void loop() {
  if (Serial2.available()) {
    Serial2.setTimeout(2000);
    Serial2.readBytes(drivePacketBuffer, DRIVE_PACKET_SIZE);
    drivePacket = DrivePacket::Deserialize(String(drivePacketBuffer));

    if(drivePacket.Data.leftMotorPower >= 20) digitalWrite(LED_BUILTIN, HIGH);  // switch LED On
    if(drivePacket.Data.leftMotorPower < 20) digitalWrite(LED_BUILTIN, LOW);   // switch LED Off

    // if(drivePacket.Data.leftMotorPower >= 20) relayBoard.SetRelayOn(6);  // switch LED On
    // if(drivePacket.Data.leftMotorPower < 20) relayBoard.SetRelayOff(6);   // switch LED Off

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
