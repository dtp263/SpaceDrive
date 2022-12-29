#ifndef DIGI_POT_H
#define DIGI_POT_H

#include <Arduino.h>
#include <Multiplexer.h>
#include <Wire.h>

#define DigiPotAddr 0x2C
#define DigiPotChannelCount 4

class DigiPot
{
private:
    int I2CBusIndex;
    int ResistanceValues[DigiPotChannelCount] = {100, 100, 100, 100};

    void beginTransmission()
    {
        setTCA9548ABus(I2CBusIndex);
        // Start I2C transmission
        Wire.beginTransmission(DigiPotAddr);
    }

    void endTransmission()
    {
        // Stop I2C transmission
        Wire.endTransmission();
    }

    void printResistanceChannelInfo(int potIndex, int value) {
        char buffer[30];
        sprintf(buffer, "Resistance Channel-%d: %d K", potIndex, value);
        Serial.println(buffer);
    }

public:
    DigiPot(int i2cBusIndex)
    {
        I2CBusIndex = i2cBusIndex;
    }

    void Setup()
    {
        setTCA9548ABus(I2CBusIndex);
        SetAllValues();
    }

    void WriteResistanceValue(int potIndex, int value)
    {
        beginTransmission();

        // Send instruction for POT [potIndex]
        Wire.write(potIndex);
        // set resistance value
        Wire.write(value);

        ResistanceValues[potIndex] = value;
        endTransmission();

        printResistanceChannelInfo(potIndex, value);
    }

    void SetAllValues()
    {
        beginTransmission();

        for (int i = 0; i < DigiPotChannelCount; i++)
        {
            // Send instruction for POT [i]
            Wire.write(i);
            // set resistance value
            Wire.write(ResistanceValues[i]);

            printResistanceChannelInfo(i, ResistanceValues[i]);
        }

        endTransmission();
    }
};

#endif
