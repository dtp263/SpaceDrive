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
    float throttleMultiplier = 1.0;
  
public:

    void Setup()
    {
        
    }

    void SetThrottleMultiplier(int throttlePercentage) {
        throttleMultiplier = throttlePercentage / (float)100;
    }

    DualMotorOutputValue ConvertToDualMotorOutput(RelativeJoystickPosition joystickPosition, int selectedDirection) {
        int forwardPower = ConvertToScale(joystickPosition.GetScale(), 100, joystickPosition.Y);
        DualMotorOutputValue tmpOutputValue = DualMotorOutputValue{
            LeftPowerPercentage: forwardPower,
            RightPowerPercentage: forwardPower,
        };

        float leftRightAdjuster = float(joystickPosition.X) / float(joystickPosition.GetScale());

        // Sigmoid function
        leftRightAdjuster = 1 / (1 + exp(-leftRightAdjuster)) * 2 - 1;
        leftRightAdjuster = leftRightAdjuster * 1.5;

        // Tanh function
        // leftRightAdjuster = tanh(leftRightAdjuster) * 2;

        Serial.print("Left Right Adjuster: ");
        Serial.println(leftRightAdjuster);

        if (leftRightAdjuster < 0) {
            tmpOutputValue.LeftPowerPercentage = tmpOutputValue.LeftPowerPercentage - (tmpOutputValue.LeftPowerPercentage * (leftRightAdjuster * -1));
        } else {
            tmpOutputValue.RightPowerPercentage = tmpOutputValue.RightPowerPercentage -  tmpOutputValue.RightPowerPercentage * (leftRightAdjuster);
        }

        if (selectedDirection == -1) {
            tmpOutputValue.LeftPowerPercentage = tmpOutputValue.LeftPowerPercentage * -1;
            tmpOutputValue.RightPowerPercentage = tmpOutputValue.RightPowerPercentage * -1;
        }

        tmpOutputValue.LeftPowerPercentage = static_cast<float>(tmpOutputValue.LeftPowerPercentage) * throttleMultiplier;
        tmpOutputValue.RightPowerPercentage = static_cast<float>(tmpOutputValue.RightPowerPercentage) * throttleMultiplier;

        return tmpOutputValue;
    }
};

#endif
