#ifndef JOYSTICK_POSITION_H
#define JOYSTICK_POSITION_H

// A RelativeJoystickPosition
class RelativeJoystickPosition
{
    public:
        int X;
        int Y;
        int Scale;

        RelativeJoystickPosition(int x, int y, int scale) {
            X = x;
            Y = y;
            Scale = scale;
        }

        int GetScale() {
            return Scale;
        }
};

class AbsoluteJoystickPosition
{   
    private:
        unsigned int Scale;

    public:
        unsigned int X;
        unsigned int Y;

        AbsoluteJoystickPosition(unsigned int x, unsigned int y, unsigned int scale) {
            X = x;
            Y = y;
            Scale = scale;
        }

        RelativeJoystickPosition ToRelativePositionFromCenter() {
            int newScale = Scale / 2;
            int relativeLeft = int(X) - newScale;
            int relativeRight = int(Y) - newScale;
            return RelativeJoystickPosition(relativeLeft, relativeRight, newScale);
        }
};

#endif
