#include <Arduino.h>
#include <ScreenWriter.h>
#include <JoystickReader.h>

ScreenWriter screenWriter = ScreenWriter();
JoystickReader joystickReader = JoystickReader();

JoystickPosition currentPosition = JoystickPosition{};

void setup()
{
  // // put your setup code here, to run once:
  //  Serial.begin(9600);

  Serial.begin(9600);

  screenWriter.Setup();
}

void loop()
{
  currentPosition = joystickReader.ReadPosition();

  screenWriter.CurrentPositionX = currentPosition.X;
  screenWriter.CurrentPositionY = currentPosition.Y;

  

  screenWriter.CurrentPowerLeft = currentPosition.X / float(1024) * 100;
  screenWriter.CurrentPowerRight = currentPosition.Y / float(1024) * 100;

  
  screenWriter.Update();
  
  delay(1000);

}