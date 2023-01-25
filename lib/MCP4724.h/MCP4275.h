#ifndef MCP4725_H
#define MCP4725_H

#include <Adafruit_MCP4725.h>
#include <Multiplexer.h>
#include <Wire.h>
 
#define MCP4725In A0
 
Adafruit_MCP4725 MCP4725;

class MCP4725Writer {
    private:
        int I2CBusIndex;

    public:
        MCP4725Writer(int i2cBusIndex)
        {
            I2CBusIndex = i2cBusIndex;
        }

        void setup() {
            setTCA9548ABus(I2CBusIndex);
            MCP4725.begin(0x60);
        }

        void SetVoltage(uint16_t output) {
            setTCA9548ABus(I2CBusIndex);
            MCP4725.setVoltage(output, false);
        }
};

#endif
