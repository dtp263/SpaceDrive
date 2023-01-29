#ifndef OUTPUT_DIFFERENTIAL_H
#define OUTPUT_DIFFERENTIAL_H

#include <JoystickReader.h>
#include <Arduino.h>
#include <Utils.h>

struct DualMotorOutputValue {
    int LeftPowerPercentage;
    int RightPowerPercentage;
};


class OutputDifferential
{
private:
  
public:

    void Setup()
    {
        
    }

    DualMotorOutputValue ConvertToDualMotorOutput(RelativeJoystickPosition joystickPosition) {
        int forwardPower = ConvertToScale(joystickPosition.GetScale(), 100, joystickPosition.Y);
        DualMotorOutputValue tmpOutputValue = DualMotorOutputValue{
            LeftPowerPercentage: forwardPower,
            RightPowerPercentage: forwardPower,
        };

        float leftRightAdjuster = float(joystickPosition.X) / float(joystickPosition.GetScale());

        if (leftRightAdjuster < 0) {
            tmpOutputValue.LeftPowerPercentage = tmpOutputValue.LeftPowerPercentage - (tmpOutputValue.LeftPowerPercentage * (leftRightAdjuster * -1));
        } else {
            tmpOutputValue.RightPowerPercentage = tmpOutputValue.RightPowerPercentage -  tmpOutputValue.RightPowerPercentage * (leftRightAdjuster);
        }

        return tmpOutputValue;
    }
};

#endif
