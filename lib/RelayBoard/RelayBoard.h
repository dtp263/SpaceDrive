#ifndef RELAY_BOARD_H
#define RELAY_BOARD_H

#include <Arduino.h>

class EightChannelRelayBoard {
    private:
        int pins[8];

    public:
        EightChannelRelayBoard(int initPins[])
        {
            for (int i = 0; i < 8; i++)
            {
                pins[i] = initPins[i];
            }
        }

        void Setup()
        {
            for (int i = 0; i < 8; i++)
            {
                pinMode(pins[i], OUTPUT);
                SetRelayOff(i);
            }
        }

        void SetRelayOff(int index)
        {
            digitalWrite(pins[index], HIGH);
        }

        void SetRelayOn(int index)
        {
            digitalWrite(pins[index], LOW);
        }
};

#endif
