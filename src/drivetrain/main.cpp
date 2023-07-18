#include <Arduino.h>
#include <DualMotorController.h>
#include <DrivePacket.h>
#include <RelayBoard.h>
#include <Wire.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>

#include<SoftwareSerial.h>

// #define RS_485_PIN 2

#define LEFT_MOTOR_REVERSE_RELAY 6
#define RIGHT_MOTOR_REVERSE_RELAY 7

SerialTransfer myTransfer;

MyTestStruct testStruct;

DrivePacketData drivePacketData;

DualMotorController motorController = DualMotorController(1, 0);

SSD1306ScreenWriter oledScreenLeft = SSD1306ScreenWriter(7);
SSD1306ScreenWriter oledScreenRight = SSD1306ScreenWriter(6);

SSD1306ScreenWriter biosScreen = SSD1306ScreenWriter(2);

int relayPins[8] = {3,4,5,6,9,10,11,12};
EightChannelRelayBoard relayBoard = EightChannelRelayBoard(relayPins);

bool isChangingDirectionLeft;
bool isChangingDirectionRight;

DrivePacket previousDrivePacket;
DrivePacket drivePacket;
char *drivePacketBuffer = (char*)malloc(DRIVE_PACKET_SIZE);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  myTransfer.begin(Serial2);

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

  previousDrivePacket.Data.leftMotorPower = 0;
  previousDrivePacket.Data.rightMotorPower = 0;

  relayBoard.Setup();
}

void loop() {
  Serial.println("Begin loop...");
  biosScreen.WriteInt(1);
  
  if (Serial2.available()) {
    biosScreen.WriteInt(2);
    // Serial2.setTimeout(1000);
    // Serial2.readBytes(drivePacketBuffer, DRIVE_PACKET_SIZE);
    
    // previousDrivePacket = drivePacket;
    // drivePacket = DrivePacket::Deserialize(String(drivePacketBuffer));
    // Serial.println(String(drivePacketBuffer));

    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize = 0;

    recSize = myTransfer.rxObj(testStruct, recSize);
    Serial.println(testStruct.z);
    Serial.println(testStruct.y);

    Serial.println(recSize);
    biosScreen.WriteInt(3);

    drivePacket.Data = drivePacketData;

    if(drivePacket.Data.leftMotorPower >= 20) digitalWrite(LED_BUILTIN, HIGH);  // switch LED On
    if(drivePacket.Data.leftMotorPower < 20) digitalWrite(LED_BUILTIN, LOW);   // switch LED Off
    
    if (drivePacket.Data.leftMotorPower < 0)  {
      isChangingDirectionLeft = true;
      relayBoard.SetRelayOn(LEFT_MOTOR_REVERSE_RELAY);
      drivePacket.Data.leftMotorPower *= -1;
    } else {
      isChangingDirectionLeft = false;
      relayBoard.SetRelayOff(LEFT_MOTOR_REVERSE_RELAY); // switch left reverse switch off
    }

    if(drivePacket.Data.rightMotorPower < 0) {
      isChangingDirectionRight = true;
      relayBoard.SetRelayOn(RIGHT_MOTOR_REVERSE_RELAY); // switch right reverse switch on
      drivePacket.Data.rightMotorPower *= -1;
    } else {
      isChangingDirectionRight = false;
      relayBoard.SetRelayOff(RIGHT_MOTOR_REVERSE_RELAY); // switch right reverse switch off
    }

    // if (isChangingDirectionRight || isChangingDirectionLeft) {
    //   motorController.WritePowerToMotorAsPercentage(0,0);
    //   delay(200);
    // }

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

  delay(100);
}
