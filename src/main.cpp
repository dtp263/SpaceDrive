#include <Arduino.h>
#include <LCDScreenWriter.h>
#include <JoystickReader.h>
#include <Multiplexer.h>
#include <OutputConverter.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

LCDScreenWriter lcdScreenWriter = LCDScreenWriter(0);
JoystickReader joystickReader = JoystickReader();
OutputConverter outputConverter = OutputConverter();

JoystickPosition currentJoystickPosition = JoystickPosition{};

SSD1306ScreenWriter oledScreen1 = SSD1306ScreenWriter(1);


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void setup()
{

  Serial.begin(115200);

  // Start I2C communication with the Multiplexer
  Wire.begin();

  oledScreen1.Setup();
  oledScreen1.Update("102");

  lcdScreenWriter.Setup();
}

void loop()
{
  setTCA9548ABus(0);
  currentJoystickPosition = joystickReader.ReadPosition();

  lcdScreenWriter.CurrentPositionX = currentJoystickPosition.X;
  lcdScreenWriter.CurrentPositionY = currentJoystickPosition.Y;

  outputConverter.ConvertToDualMotorOutput(currentJoystickPosition);

  lcdScreenWriter.CurrentPowerLeft = currentJoystickPosition.X / float(1024) * 100;
  lcdScreenWriter.CurrentPowerRight = currentJoystickPosition.Y / float(1024) * 100;

  lcdScreenWriter.Update();

  delay(1000);
}
