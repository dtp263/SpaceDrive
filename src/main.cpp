#include <Arduino.h>
#include <DigiPot.h>
#include <LCDScreenWriter.h>
#include <JoystickReader.h>
#include <Multiplexer.h>
#include <OutputConverter.h>
#include <SPI.h>
#include <SSD1306ScreenWriter.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

DigiPot digiPot = DigiPot(7);

SSD1306ScreenWriter oledScreen1 = SSD1306ScreenWriter(2);
SSD1306ScreenWriter oledScreen2 = SSD1306ScreenWriter(1);
LCDScreenWriter lcdScreenWriter = LCDScreenWriter(0);

JoystickPosition currentJoystickPosition = JoystickPosition{};
JoystickReader joystickReader = JoystickReader();

OutputConverter outputConverter = OutputConverter();


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void setup()
{
  Serial.begin(9600);

  // Start I2C communication with the Multiplexer
  Wire.begin();

  digiPot.Setup();
  oledScreen1.Setup();
  oledScreen2.Setup();
  oledScreen1.Update("000");
  oledScreen2.Update("000");

  lcdScreenWriter.Setup();
}

int convertToScale(int inputScale, int outputScale, int value) {
  float conversion = float(value) / float(inputScale);
  return conversion * outputScale;
}

void loop()
{
  setTCA9548ABus(0);
  currentJoystickPosition = joystickReader.ReadPosition();

  lcdScreenWriter.CurrentPositionX = currentJoystickPosition.X;
  lcdScreenWriter.CurrentPositionY = currentJoystickPosition.Y;

  outputConverter.ConvertToDualMotorOutput(currentJoystickPosition);

  lcdScreenWriter.CurrentPowerLeft = convertToScale(1024, 100, currentJoystickPosition.X);
  lcdScreenWriter.CurrentPowerRight = convertToScale(1024, 100, currentJoystickPosition.Y);

  char buffer[16];
  itoa(convertToScale(1024, 256, currentJoystickPosition.X), buffer, 10);
  oledScreen1.Update(buffer);

  itoa(convertToScale(1024, 256, currentJoystickPosition.Y), buffer, 10);
  oledScreen2.Update(buffer);

  lcdScreenWriter.Update();

  digiPot.WriteResistanceValue(0, lcdScreenWriter.CurrentPowerLeft);
  digiPot.WriteResistanceValue(1, lcdScreenWriter.CurrentPowerLeft);
  digiPot.WriteResistanceValue(2, lcdScreenWriter.CurrentPowerLeft);
  digiPot.WriteResistanceValue(3, lcdScreenWriter.CurrentPowerLeft);

  // Serial.println("finished loop... waiting...");
  // delay(3000);
}
