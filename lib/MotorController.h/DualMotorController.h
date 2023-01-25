#ifndef DUAL_MOTOR_CONTROLLER_H
#define DUAL_MOTOR_CONTROLLER_H

#define START_THROTTLE_VOLTAGE .871
#define END_THROTTLE_VOLTAGE 3.99

#include <OutputConverter.h>
#include <MCP4275.h>
#include <Utils.h>

float convertToVoltageRange(float startVoltage, float endVoltage, int percentage) {
  float voltagePerPercentagePoint = (endVoltage - startVoltage) / 100;
  return (voltagePerPercentagePoint * percentage) + startVoltage;
}

int voltageToArduinoAnalogValue(float voltage) {
    return (voltage / float(5)) * 255;
}

uint16_t convertTo12BitRange(int value, int inputScale) {
    int tmp = ConvertToScale(inputScale, 4096, value);
    return uint16_t(tmp);
}

struct MotorVoltages {
    float Left;
    float Right;
};

struct MotorAnalogOutput {
    int Left;
    int Right;
};

class DualMotorController {
    private:
        int leftMotorPin;
        int rightMotorPin;

        float leftMotorVoltage = 0.0;
        float rightMotorVoltage = 0.0;

        int leftArduinoOutputValue = 0;
        int rightArduinoOutputValue = 0;

        MCP4725Writer leftMotorWriter = MCP4725Writer(0);
        MCP4725Writer rightMotorWriter = MCP4725Writer(0);

    public:
        DualMotorController(int leftPin, int rightPin) 
        {
            leftMotorPin = leftPin;
            rightMotorPin = rightPin;

            leftMotorWriter = MCP4725Writer(leftPin);
            rightMotorWriter = MCP4725Writer(rightPin);

        }

        void Setup() {
            leftMotorWriter.setup();
            rightMotorWriter.setup();
        }

        void WritePowerToMotor(int leftPower, int rightPower, int scale) {
            leftMotorVoltage = convertToVoltageRange(START_THROTTLE_VOLTAGE, END_THROTTLE_VOLTAGE, leftPower);
            rightMotorVoltage = convertToVoltageRange(START_THROTTLE_VOLTAGE, END_THROTTLE_VOLTAGE, rightPower);

            leftArduinoOutputValue = convertTo12BitRange(leftPower, scale);
            rightArduinoOutputValue = convertTo12BitRange(rightPower, scale);

            leftMotorWriter.SetVoltage(leftArduinoOutputValue);
            rightMotorWriter.SetVoltage(rightArduinoOutputValue);
        }

        void WritePowerToMotorAsPercentage(DualMotorOutputValue outputValues) {
            WritePowerToMotor(outputValues.LeftPowerPercentage, outputValues.RightPowerPercentage, 100);
        }

        MotorVoltages GetMotorVoltages() {
            return MotorVoltages{
                Left: leftMotorVoltage,
                Right: rightMotorVoltage,
            };
        }

        MotorAnalogOutput GetAnalogOutputs() {
            return MotorAnalogOutput{
                Left: leftArduinoOutputValue,
                Right: rightArduinoOutputValue,
            };
        }
};

#endif
