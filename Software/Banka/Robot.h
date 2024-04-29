#pragma once

#include <Arduino.h>

#include "Button.h"
#include "ConnectionList.h"
#include "LedArray.h"
#include "MotorDriver.h"

#define WRONG_VOLTAGE_MAX_TIME  1000

#define VCC_SENSE_COEF  0.007102


#define VOLTAGE_ERROR   0
#define DRIVER_ERROR    1

class Robot{
    private:
    uint8_t error = false;
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
    uint8_t getError();
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

    //Motor driver fault pin
    pinMode(MOTOR_FAULT_PIN, INPUT_PULLUP);
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
 
        long result;
        // Read 1.1V reference against AVcc
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
        delay(2); // Wait for Vref to settle
        ADCSRA |= _BV(ADSC); // Convert
        while (bit_is_set(ADCSRA,ADSC));
        result = ADCL;
        result |= ADCH<<8;
        result = 1126400L / result; // Back-calculate AVcc in mV
        voltage = result / 1000.0;

        //voltage = analogRead(VIN_SENSE)*VCC_SENSE_COEF;
        if(voltage > 4.5 && voltage < 5.5) wrongVoltageTimer = millis();
        
        error &= ~(1 << VOLTAGE_ERROR);
        if(millis() - wrongVoltageTimer > WRONG_VOLTAGE_MAX_TIME) error |= (1 << VOLTAGE_ERROR);

        error &= ~(1 << DRIVER_ERROR);
        if(!digitalRead(MOTOR_FAULT_PIN)) error |= (1 << DRIVER_ERROR);

        digitalWrite(LED_ERROR, error);
    } while(millis() - timer < delayTime);
}

int Robot::getSensor(int index){
    return sensorsVal[index];
}

void Robot::setMotors(int speed1, int speed2){
    motor1.setSpeed(speed1);
    motor2.setSpeed(-speed2);
}

uint8_t Robot::getError(){
    return error;
}

double Robot::getVoltage(){
    return voltage;
}
