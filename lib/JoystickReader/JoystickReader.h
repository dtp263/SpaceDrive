#ifndef JOYSTICK_READER_H
#define JOYSTICK_READER_H

#include <Arduino.h>
#include <JoystickPosition.h>


class JoystickReader
{
private:
    uint8_t potPinX = A1;
    uint8_t potPinY = A0;

    bool invertX = true;
    bool invertY = true;

    int InvertPosition(int position) {
        return (position - 1024) * -1;
    }

public:

    void Setup()
    {
        
    }

    AbsoluteJoystickPosition ReadAbsolutePosition() {
        int xPosition = analogRead(potPinX);
        int yPosition = analogRead(potPinY);
        if (invertX == true) {
            xPosition = InvertPosition(xPosition);
        }
        if (invertY == true) {
            yPosition = InvertPosition(yPosition);
        }
        return AbsoluteJoystickPosition(xPosition, yPosition, 1024);
    }

    RelativeJoystickPosition ReadRelativePosition() {
        AbsoluteJoystickPosition absolutePosition = ReadAbsolutePosition();
        return absolutePosition.ToRelativePositionFromCenter();
    }
};

#endif
