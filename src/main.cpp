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
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{

  // Serial.begin(9600);
  Serial.begin(115200);

  // // put your setup code here, to run once:
  //  Serial.begin(9600);

  // Start I2C communication with the Multiplexer
  Wire.begin();

  oledScreen1.Setup();

  // oledScreen1.output = "102";
  oledScreen1.Update("102");

   // Init OLED display on bus number 2
  // setTCA9548ABus(1);
  // if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  // {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for (;;)
  //     ;
  // }
  // Clear the buffer
  // display.clearDisplay();

  // setTCA9548ABus(1);
  // Write to OLED on bus number 1
  // display.clearDisplay();
  // display.setTextSize(4);
  // display.setTextColor(WHITE);
  // display.setCursor(25, 25);
  // // Display static text
  // display.println("101");
  // display.display(); 

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
