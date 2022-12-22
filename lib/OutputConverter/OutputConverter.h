#ifndef OUTPUT_CONVERTER_H
#define OUTPUT_CONVERTER_H

#include <JoystickReader.h>
#include <Arduino.h>

struct DualMotorOutputValue {
    int Left;
    int Right;
};


class OutputConverter
{
private:
  
public:

    void Setup()
    {
        
    }

    DualMotorOutputValue ConvertToDualMotorOutput(JoystickPosition joystickPosition) {
        return DualMotorOutputValue{
            Left: 100,
            Right: 100,
        };
    }
};

#endif
