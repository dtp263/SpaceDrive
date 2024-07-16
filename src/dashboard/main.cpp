#include <Arduino.h>
#include <LCDScreenWriter.h>
#include <JoystickReader.h>
#include <OutputDifferential.h>
#include <DualMotorController.h>
#include <DrivePacket.h>
#include <Potentiometer.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Utils.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FWD_DRIVE_SWITCH_PIN 2
#define REV_DRIVE_SWITCH_PIN 3

#define TUNING_KNOB_PIN 7

#define RS_485_PIN 8

#define JOYSTICK_POSITION_COUNT 1024

boolean DEBUG_MODE = false;

LCDScreenWriter lcdScreenWriter = LCDScreenWriter(0);

RelativeJoystickPosition previousJoystickPosition = RelativeJoystickPosition(0, 0, JOYSTICK_POSITION_COUNT);
RelativeJoystickPosition currentJoystickPosition = RelativeJoystickPosition(0, 0, JOYSTICK_POSITION_COUNT);
JoystickReader joystickReader = JoystickReader();

Potentiometer tuningKnob = Potentiometer(7);

OutputDifferential outputConverter = OutputDifferential();

DualMotorOutputValue motorOutputValue = DualMotorOutputValue{
  LeftPowerPercentage: 0,
  RightPowerPercentage: 0
};

DrivePacket drivePacket;

char* drivePacketBuffer = (char*)malloc(DRIVE_PACKET_SIZE);

String packetBuffer;

void setup()
{
  Serial.println("begin setup...");
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(FWD_DRIVE_SWITCH_PIN, INPUT);
  pinMode(REV_DRIVE_SWITCH_PIN, INPUT);

  tuningKnob.Setup();

  // Start I2C communication with the Multiplexer
  Wire.begin();

  lcdScreenWriter.Setup();
  delay(10);
  pinMode(RS_485_PIN, OUTPUT);
  digitalWrite(RS_485_PIN, HIGH);
}

void loop()
{
  drivePacket.Data.direction = 0;
  if (digitalRead(FWD_DRIVE_SWITCH_PIN) == HIGH)
  {
    drivePacket.Data.direction = 1;
  }
  else if (digitalRead(REV_DRIVE_SWITCH_PIN) == HIGH)
  {
    drivePacket.Data.direction = -1;
  }

  int tuningNumber = tuningKnob.ReadPercentage();

  outputConverter.SetThrottleMultiplier(tuningNumber);

  currentJoystickPosition = joystickReader.ReadRelativePosition();
  motorOutputValue = outputConverter.ConvertToDualMotorOutput(previousJoystickPosition, currentJoystickPosition, drivePacket.Data.direction);

  previousJoystickPosition = currentJoystickPosition;

  drivePacket.Data.leftMotorPower = motorOutputValue.LeftPowerPercentage;
  drivePacket.Data.rightMotorPower = motorOutputValue.RightPowerPercentage;

  packetBuffer = DrivePacket::Serialize(&drivePacket);
  packetBuffer.toCharArray(drivePacketBuffer, DRIVE_PACKET_SIZE);
  Serial1.write(drivePacketBuffer, DRIVE_PACKET_SIZE);

  // Do display work.
  lcdScreenWriter.CurrentPositionX = currentJoystickPosition.X;
  lcdScreenWriter.CurrentPositionY = currentJoystickPosition.Y;

  lcdScreenWriter.CurrentPowerLeft = motorOutputValue.LeftPowerPercentage;
  lcdScreenWriter.CurrentPowerRight = motorOutputValue.RightPowerPercentage;
  lcdScreenWriter.Update();

  if (DEBUG_MODE == true) {
    Serial.println("Tuning number: ");
    Serial.println(tuningNumber);
    Serial.println("drivePacketBuffer: ");
    Serial.println(packetBuffer);

    Serial.println(digitalRead(FWD_DRIVE_SWITCH_PIN));
    Serial.println(digitalRead(REV_DRIVE_SWITCH_PIN));
    Serial.println("finished loop... waiting... ");
    Serial.println();
    delay(1000);
  }
}
