#include <Arduino.h>
#include <LCDScreenWriter.h>
#include <JoystickReader.h>
#include <OutputDifferential.h>
#include <DualMotorController.h>
#include <DrivePacket.h>
#include <SerialTransfer.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Utils.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define RS_485_PIN 8

#define JOYSTICK_POSITION_COUNT 1024

SSD1306ScreenWriter oledScreenLeft = SSD1306ScreenWriter(1);
SSD1306ScreenWriter oledScreenRight = SSD1306ScreenWriter(0);
SSD1306ScreenWriter oledScreenLeftBottom = SSD1306ScreenWriter(2);
SSD1306ScreenWriter oledScreenRightBottom = SSD1306ScreenWriter(3);

LCDScreenWriter lcdScreenWriter = LCDScreenWriter(4);


RelativeJoystickPosition currentJoystickPosition = RelativeJoystickPosition(0, 0, JOYSTICK_POSITION_COUNT);
JoystickReader joystickReader = JoystickReader();

OutputDifferential outputConverter = OutputDifferential();

DualMotorController motorController = DualMotorController(5, 6);
DualMotorOutputValue motorOutputValue = DualMotorOutputValue{
  LeftPowerPercentage: 0,
  RightPowerPercentage: 0
};

DrivePacket drivePacket;
DrivePacket *testDrivePacket;


char* drivePacketBuffer = (char*)malloc(DRIVE_PACKET_SIZE);

String packetBuffer;

void setup()
{
  Serial.println("begin setup...");
  Serial.begin(9600);
  Serial1.begin(9600);

  // Start I2C communication with the Multiplexer
  Wire.begin();

  motorController.Setup();

  oledScreenLeft.Setup();
  oledScreenRight.Setup();
  oledScreenLeftBottom.Setup();
  oledScreenRightBottom.Setup();

  // lcdScreenWriter.Setup();
  delay(10);
  pinMode(RS_485_PIN, OUTPUT);
  digitalWrite(RS_485_PIN, HIGH);
}

void loop()
{
  currentJoystickPosition = joystickReader.ReadRelativePosition();
  motorOutputValue = outputConverter.ConvertToDualMotorOutput(currentJoystickPosition);

  drivePacket.Data.leftMotorPower = motorOutputValue.LeftPowerPercentage;
  drivePacket.Data.rightMotorPower = motorOutputValue.RightPowerPercentage;
  packetBuffer = DrivePacket::Serialize(&drivePacket);
  packetBuffer.toCharArray(drivePacketBuffer, DRIVE_PACKET_SIZE);
  Serial1.write(drivePacketBuffer, DRIVE_PACKET_SIZE);
  Serial.println(packetBuffer);

  drivePacket = DrivePacket::Deserialize(packetBuffer);
  
  motorController.WritePowerToMotorAsPercentage(motorOutputValue.LeftPowerPercentage, motorOutputValue.RightPowerPercentage);

  // Do display work.
  lcdScreenWriter.CurrentPositionX = currentJoystickPosition.X;
  lcdScreenWriter.CurrentPositionY = currentJoystickPosition.Y;

  lcdScreenWriter.CurrentPowerLeft = motorOutputValue.LeftPowerPercentage;
  lcdScreenWriter.CurrentPowerRight = motorOutputValue.RightPowerPercentage;
  lcdScreenWriter.Update();

  MotorVoltages voltages = motorController.GetMotorVoltages();
  oledScreenLeft.WriteFloat(voltages.Left);
  oledScreenRight.WriteFloat(voltages.Right);

  // MotorAnalogOutput analogOutputs = motorController.GetAnalogOutputs();
  oledScreenLeftBottom.WriteInt(drivePacket.Data.leftMotorPower);
  oledScreenRightBottom.WriteInt(drivePacket.Data.rightMotorPower);

  Serial.println("finished loop... waiting...");
  delay(100);
}
