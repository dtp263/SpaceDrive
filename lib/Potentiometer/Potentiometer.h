#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>

class Potentiometer
{
private:
    int pin;
    int maxValue = 1000;
    int minValue = 30;
  
public:
    Potentiometer(int initPin) {
        pin = initPin;
    }

    void Setup() {
        pinMode(pin, INPUT);
    }

    int ReadPercentage() {
        int value = analogRead(pin);
        int percentage = map(value, minValue, maxValue, 0, 100);

        return percentage;
    }
};

#endif
