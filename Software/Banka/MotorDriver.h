#pragma once

#include <Arduino.h>

#include "Useful.h"


#define ACCELERATION 1


class MotorDriver{
    private:
    int pin1, pin2;
    double speed = 0;
    double prevSpeed = 0;
    long long timer = 0;

    public:
    MotorDriver(int, int);
    void setSpeed(int);
    void update();
};

MotorDriver::MotorDriver(int pin1, int pin2){
    this->pin1 = pin1;
    this->pin2 = pin2;

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);

    digitalWrite(pin1, 1);
    digitalWrite(pin2, 1);
}

void MotorDriver::setSpeed(int speed){
  this->speed = speed;
}

void MotorDriver::update(){

    prevSpeed = prevSpeed + sign(speed - prevSpeed) * (millis() - timer) * ACCELERATION;
    int sp = prevSpeed;
    timer = millis(); 
  
    bool dir = true;
    
    if(sp < 0) dir = false;
    sp = abs(sp);
    if(sp > 255) sp = 255;

    if(dir){
        digitalWrite(pin1, 1);
        analogWrite(pin2, 255-sp);
    }
    else {
        digitalWrite(pin2, 1);
        analogWrite(pin1, 255-sp);
    }
}
