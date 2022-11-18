#ifndef JOYSTICK_READER_H
#define JOYSTICK_READER_H

#include <Arduino.h>


struct JoystickPosition{
   int X;
   int Y;
};

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

    JoystickPosition ReadPosition() {
        int xPosition = analogRead(potPinX);
        int yPosition = analogRead(potPinY);
        if (invertX == true) {
            xPosition = InvertPosition(xPosition);
        }
        if (invertY == true) {
            yPosition = InvertPosition(yPosition);
        }
        return JoystickPosition{
            X: xPosition,
            Y: yPosition,
        };

    }
};

#endif
