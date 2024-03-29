#include <Arduino.h>
#include <DualMotorController.h>
#include <DrivePacket.h>
#include <RelayBoard.h>
#include <Wire.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>

#include<SoftwareSerial.h>

// #define RS_485_PIN 2

boolean DEBUG_MODE = false;

#define LEFT_MOTOR_REVERSE_RELAY 6
#define RIGHT_MOTOR_REVERSE_RELAY 7

DualMotorController motorController = DualMotorController(1, 0);

SSD1306ScreenWriter oledScreenLeft = SSD1306ScreenWriter(7);
SSD1306ScreenWriter oledScreenRight = SSD1306ScreenWriter(6);

SSD1306ScreenWriter biosScreen = SSD1306ScreenWriter(2);

int relayPins[8] = {3,4,5,6,9,10,11,12};
EightChannelRelayBoard relayBoard = EightChannelRelayBoard(relayPins);

bool success;
DrivePacket drivePacket;
char *drivePacketBuffer = (char*)malloc(DRIVE_PACKET_SIZE);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.print("Begin setup...");

  // Start I2C communication with the Multiplexer
  Wire.begin();
  motorController.Setup();
  oledScreenLeft.Setup();
  oledScreenRight.Setup();
  biosScreen.Setup();

  biosScreen.WriteInt(0);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // pinMode(RS_485_PIN, OUTPUT);
  // digitalWrite(RS_485_PIN, LOW);

  relayBoard.Setup();
}

void loop() {
  Serial.println("Begin loop...");
  biosScreen.WriteInt(1);
  
  if (Serial2.available()) {
    biosScreen.WriteInt(2);
    Serial2.setTimeout(1000);
    char inChar = Serial2.read();
    int attempts = 0;
    while (inChar != *START_DELIMITER || attempts < DRIVE_PACKET_SIZE)
    {
      inChar = Serial2.read();
      attempts++;
    }

    Serial2.readBytes(drivePacketBuffer, DRIVE_PACKET_SIZE);
    
    success = DrivePacket::Deserialize(String(drivePacketBuffer), &drivePacket);

    if (!success) {
      biosScreen.WriteInt(3);
      Serial.println("Failed to deserialize packet");
      delay(1000);
      return;
    }

    if (drivePacket.Data.direction == 0 || drivePacket.Data.direction == 1) {
      relayBoard.SetRelayOff(LEFT_MOTOR_REVERSE_RELAY);
      relayBoard.SetRelayOff(RIGHT_MOTOR_REVERSE_RELAY); 
    } else {
      relayBoard.SetRelayOn(LEFT_MOTOR_REVERSE_RELAY);
      relayBoard.SetRelayOn(RIGHT_MOTOR_REVERSE_RELAY);
    }
    
    if(drivePacket.Data.leftMotorPower >= 20) digitalWrite(LED_BUILTIN, HIGH);  // switch LED On
    if(drivePacket.Data.leftMotorPower < 20) digitalWrite(LED_BUILTIN, LOW);   // switch LED Off

    motorController.WritePowerToMotorAsPercentage(
      drivePacket.Data.leftMotorPower,
      drivePacket.Data.rightMotorPower
    );

    if (DEBUG_MODE) {
      Serial.print(drivePacket.Data.direction);
      Serial.print(",");
      Serial.print(drivePacket.Data.leftMotorPower);
      Serial.print(",");
      Serial.print(drivePacket.Data.rightMotorPower);
      Serial.print("\n");
    }
  }

  oledScreenLeft.WriteInt(drivePacket.Data.leftMotorPower);
  oledScreenRight.WriteInt(drivePacket.Data.rightMotorPower);

  if (DEBUG_MODE) {
    delay(100);
  }
}
