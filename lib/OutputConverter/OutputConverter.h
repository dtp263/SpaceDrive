#ifndef OUTPUT_CONVERTER_H
#define OUTPUT_CONVERTER_H

#include <JoystickReader.h>
#include <Arduino.h>
#include <Utils.h>

struct DualMotorOutputValue {
    int LeftPowerPercentage;
    int RightPowerPercentage;
};


class OutputConverter 
{
private:
  
public:

    void Setup()
    {
        
    }

    DualMotorOutputValue ConvertToDualMotorOutput(RelativeJoystickPosition joystickPosition) {
        return DualMotorOutputValue{
            LeftPowerPercentage: ConvertToScale(1024, 100, joystickPosition.X),
            RightPowerPercentage: ConvertToScale(1024, 100, joystickPosition.Y),
        };
    }
};

#endif
