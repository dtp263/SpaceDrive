#ifndef LCD_SCREEN_WRITER_H
#define LCD_SCREEN_WRITER_H

#include <Multiplexer.h>
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define COLUMS 20
#define ROWS 4
#define PAGE ((COLUMS) * (ROWS))
#define LCD_SPACE_SYMBOL 0x20 // space symbol from the LCD ROM, see p.9 of GDM2004D datasheet

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

const int SizeOfPercentageEntry = 5;
const int PowerOutputRow = 1;
const char PowerOutputLabel[] = "Output: ";
const char PowerOutputSeparator[] = "/";

const int SizeOfPositionEntry = 4;
const int PositionOutputRow = 2;
const char PositionOutputLabel[] = "Pos: ";
const char PositionOutputSeparator[] = "/";

class LCDScreenWriter
{
private:
    int OutputLabelSize;
    int OutputSeparatorSize;

    int PositionLabelSize;
    int PositionSeparatorSize;

    int I2CBusIndex;

    void PrintOutputLine()
    {
        int leftStart = OutputLabelSize - 1;
        int separatorStart = leftStart + SizeOfPercentageEntry;
        int rightStart = leftStart + SizeOfPercentageEntry + OutputSeparatorSize;

        char emptyEntry[] = "    ";

        lcd.setCursor(leftStart, PowerOutputRow);
        lcd.print(emptyEntry);
        lcd.setCursor(leftStart, PowerOutputRow);
        if (this->CurrentPowerLeft > -1)
        {
            lcd.print("+");
        }
        lcd.print(this->CurrentPowerLeft);

        lcd.setCursor(separatorStart, PowerOutputRow);
        lcd.print(PowerOutputSeparator);

        lcd.setCursor(rightStart, PowerOutputRow);
        lcd.print(emptyEntry);
        lcd.setCursor(rightStart, PowerOutputRow);
        if (this->CurrentPowerRight > -1)
        {
            lcd.print("+");
        }
        lcd.print(this->CurrentPowerRight);
    }

    void PrintPositionLine()
    {
        int leftStart = PositionLabelSize - 1;
        int separatorStart = leftStart + SizeOfPercentageEntry;
        int rightStart = leftStart + SizeOfPercentageEntry + PositionSeparatorSize;

        char emptyEntry[] = "    ";

        lcd.setCursor(leftStart, PositionOutputRow);
        lcd.print(emptyEntry);
        lcd.setCursor(leftStart, PositionOutputRow);
        lcd.print(this->CurrentPositionX);

        lcd.setCursor(separatorStart, PositionOutputRow);
        lcd.print(PositionOutputSeparator);

        lcd.setCursor(rightStart, PositionOutputRow);
        lcd.print(emptyEntry);
        lcd.setCursor(rightStart, PositionOutputRow);
        lcd.print(this->CurrentPositionY);
    }
    
public:
    int CurrentPowerLeft = -100;
    int CurrentPowerRight = -100;

    int CurrentPositionX = 512;
    int CurrentPositionY = 512;

    LCDScreenWriter(int i2cBusIndex)
    {
        I2CBusIndex = i2cBusIndex;
    }

    void Setup()
    {
        setTCA9548ABus(I2CBusIndex);
        OutputLabelSize = sizeof(PowerOutputLabel);
        OutputSeparatorSize = sizeof(sizeof(PowerOutputSeparator));
        PositionLabelSize = sizeof(PositionOutputLabel);
        PositionSeparatorSize = sizeof(sizeof(PositionOutputSeparator));
        while (lcd.begin(COLUMS, ROWS) != 1) // colums - 20, rows - 4
        {
            Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
            delay(5000);
        }

        lcd.print(F("PCF8574 is OK...")); //(F()) saves string to flash & keeps dynamic memory free
        delay(2000);

        lcd.clear();

        /* prints static output text */
        lcd.setCursor(0, PowerOutputRow);
        lcd.print(PowerOutputLabel);

        /* prints static position text */
        lcd.setCursor(0, PositionOutputRow);
        lcd.print(PositionOutputLabel);
    }

    void Update()
    {
        setTCA9548ABus(I2CBusIndex);
        this->PrintOutputLine();
        this->PrintPositionLine();
    }
};

#endif
