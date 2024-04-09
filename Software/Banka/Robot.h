#pragma once

#include <Arduino.h>

#include "Button.h"
#include "ConnectionList.h"
#include "LedArray.h"
#include "MotorDriver.h"

#define WRONG_VOLTAGE_MAX_TIME 1000

class Robot{
    private:
    bool error = false;
    double voltage;
    long long wrongVoltageTimer;
    int sensors[6] = {S1, S2, S3, S4, S5, S6};
    int sensorsVal[6];

    public:
    Button buttonStartStop = Button(BUTTON_START_STOP);
    Button buttonInc = Button(BUTTON_INC);
    Button buttonDec = Button(BUTTON_DEC);

    LedArray ledArr = LedArray(LEDARR_DATA, LEDARR_LATCH, LEDARR_CLK);

    MotorDriver motor1 = MotorDriver(MOTOR1_PIN1, MOTOR1_PIN2);
    MotorDriver motor2 = MotorDriver(MOTOR2_PIN1, MOTOR2_PIN2);

    Robot();
    void wait(int);
    int getSensor(int);
    void setMotors(int, int);
    bool getError();
    double getVoltage();
};


Robot::Robot(){
    //LEDs
    pinMode(LED_ERROR, OUTPUT);
    pinMode(LED_WORK, OUTPUT);

    //Sensors
    for(int i = 0; i < 6; i++)
        pinMode(sensors[i], INPUT);
    //Voltage sense
    pinMode(VIN_SENSE, INPUT);

    //UART
    //Serial.begin(115200);
}

void Robot::wait(int delayTime){
    long long timer = millis();

    do {
        buttonStartStop.update();
        buttonInc.update();
        buttonDec.update();
        motor1.update();
        motor2.update();

        for(int i = 0; i < 6; i++)
            sensorsVal[i] = analogRead(sensors[i]);

        voltage = analogRead(VIN_SENSE)*0.021484375;
        if(voltage > 8 && voltage < 9.5) wrongVoltageTimer = millis();
        
        if(millis() - wrongVoltageTimer > WRONG_VOLTAGE_MAX_TIME)error = true;
        else error = false;

        delay(1);
    } while(millis() - timer < delayTime);

    digitalWrite(LED_ERROR, error);
}

int Robot::getSensor(int index){
    return sensorsVal[index];
}

void Robot::setMotors(int speed1, int speed2){
    if(error) return;
    motor1.setSpeed(speed1);
    motor2.setSpeed(-speed2);
}

bool Robot::getError(){
    return error;
}

double Robot::getVoltage(){
    return voltage;
}
