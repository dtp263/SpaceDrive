#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <Arduino.h>
#include <Wire.h> 

void setTCA9548ABus(uint8_t bus)
{
    Wire.beginTransmission(0x70); // TCA9548A address
    Wire.write(1 << bus);         // send byte to select bus
    Wire.endTransmission();
}

#endif
