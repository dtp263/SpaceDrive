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
    void SetThrottleMultiplier(int throttlePercentage) {
        throttleMultiplier = throttlePercentage / (float)100;
    }

    float advancedLerp(float start, float end, float accelChange, float deccelChange) {
        float t;
        if (start < end) {
            t = accelChange;
        } else if (start > end) {
            t = deccelChange;
        }

        return lerp(start, end, t);
    }

    float lerp(float start, float end, float t) {
        return (1 - t) * start + t * end;
    }

    DualMotorOutputValue ConvertToDualMotorOutput(
        RelativeJoystickPosition previousJoystickPosition,
        RelativeJoystickPosition currentJoystickPosition,
        int selectedDirection
    ) {
        float lerpedForward = advancedLerp(previousJoystickPosition.Y, currentJoystickPosition.Y, 0.5, 0.8);
        float lerpedTurn = lerp(previousJoystickPosition.X, currentJoystickPosition.X, 0.5);

        DualMotorOutputValue tmpOutputValue = DualMotorOutputValue{
            LeftPowerPercentage: 0,
            RightPowerPercentage: 0,
        };

        float turnDamping = 5;
        float forward = static_cast<float>(lerpedForward) / currentJoystickPosition.GetScale();
        float turn = static_cast<float>(lerpedTurn) / currentJoystickPosition.GetScale();

        turn /= 1 + turnDamping * forward * forward;
        tmpOutputValue.LeftPowerPercentage = (selectedDirection * (forward + turn) * throttleMultiplier) * 100;
        tmpOutputValue.RightPowerPercentage = (selectedDirection * (forward - turn) * throttleMultiplier) * 100;

        return tmpOutputValue;
    }
};

#endif
