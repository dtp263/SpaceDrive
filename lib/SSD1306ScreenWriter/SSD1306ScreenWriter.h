#ifndef SSD1306_SCREEN_WRITER_H
#define SSD1306_SCREEN_WRITER_H

#include <Multiplexer.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

class SSD1306ScreenWriter
{
private:
    int I2CBusIndex;
    
public:
    // char output[];

    SSD1306ScreenWriter(int i2cBusIndex)
    {
        I2CBusIndex = i2cBusIndex;
    }

    void Setup()
    {
        setTCA9548ABus(I2CBusIndex);
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
        {
            Serial.println(F("SSD1306 allocation failed"));
            for (;;)
            ;
        }
        // Clear the buffer
        display.clearDisplay();
    }

    void Update(char output[])
    {
        setTCA9548ABus(I2CBusIndex);
        display.clearDisplay();
        display.setTextSize(4);
        display.setTextColor(WHITE);
        display.setCursor(25, 25);
        display.println(output);
        display.display(); 
    }
};

#endif