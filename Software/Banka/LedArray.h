#pragma once

#include <Arduino.h>


class LedArray {
    private:
    int pinData, pinLatch, pinClock;

    public:
    LedArray(int, int, int);
    void write(uint8_t);
    void show(int);
};

LedArray::LedArray(int pinData, int pinLatch, int pinClock) {
    this->pinData = pinData;
    this->pinLatch = pinLatch;
    this->pinClock = pinClock;

    pinMode(pinData, OUTPUT);
    pinMode(pinLatch, OUTPUT);
    pinMode(pinClock, OUTPUT);
    digitalWrite(pinLatch, HIGH);
}

void LedArray::write(uint8_t data) {
    digitalWrite(pinLatch, LOW);
    shiftOut(pinData, pinClock, LSBFIRST, data);
    digitalWrite(pinLatch, HIGH);
}

void LedArray::show(int data) {
    if(data > 8) data = 8;
    if(data < 0) data = 0;

    LedArray::write(ceil(pow(2, data)) - 1);
}
