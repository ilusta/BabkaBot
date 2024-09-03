#include "Robot.h"
#include "Useful.h"
#include <EEPROM.h>

#define KP   0.15
#define KD   5.0
#define CALIBRATION_TIME  10000

#define EEPROM_SPEED      0
#define EEPROM_WIDTH      1
#define EEPROM_TRESHOLDS  2  

#define STATE_WAITING false
#define STATE_MOVING true

Robot robot = Robot();

bool state = STATE_WAITING;
int speed = EEPROM.read(EEPROM_SPEED);
long long widthChangeTimer = millis();
bool width = EEPROM.read(EEPROM_WIDTH);
bool direction = 1;
long long dirTimer = millis();
long long timer = millis();
int val = 0;
int tresholds[6] = {300, 300, 300, 300, 300, 300};
int err = 0, errOld = 0;
int u = 0;
int sp = 0;


void setup() {
  Serial.begin(115200);
  robot.wait(1);

  for(int i = 0; i < 6; i++){
    tresholds[i] = EEPROM.read(EEPROM_TRESHOLDS + i) * 4;
  }

  for (int i = 0; i <= 8; i++) {
    robot.ledArr.write(1 << i);
    delay(50);
  }
  for (int i = 7; i >= 0; i--) {
    robot.ledArr.write(1 << i);
    delay(50);
  }
  robot.ledArr.show(0);
  delay(100);
}

void loop() {
  while(true){
    robot.wait(5);

    if(robot.buttonInc.isPressed() && robot.buttonDec.isPressed()){
      calibration();
      continue;
    }

    bool spFlag = false;
    if (robot.buttonInc.isReleased()) {
      speed++;
      spFlag = true;
      if (speed > 8) speed = 8;
    }
    if (robot.buttonDec.isReleased()) {
      speed--;
      spFlag = true;
      if (speed < 1) speed = 1;
    }
    if (spFlag) EEPROM.write(0, speed);
    robot.ledArr.show(speed);

    if (!robot.buttonStartStop.isPressed()) widthChangeTimer = millis();
    if (millis() - widthChangeTimer > 10000) {
      width = !width;
      EEPROM.write(1, width);
      if (width) robot.ledArr.write(0b01111110);
      else robot.ledArr.write(0b00011000);

      while (robot.buttonStartStop.isPressed()) {
        if (width) robot.ledArr.write((millis() % 1000) > 500 ? 0 : 0b01111110);
        else robot.ledArr.write((millis() % 1000) > 500 ? 0 : 0b00011000);
        robot.wait(1);
      }
      robot.buttonStartStop.isReleased();
      robot.wait(1);
    }
    else if (robot.buttonStartStop.isReleased()) state = !state;

    if (robot.getError()) state = STATE_WAITING;


    if (direction) {
      if (width) {
        val = robot.getSensor(5);
        err = robot.getSensor(5) - tresholds[5];
      }
      else {
        val = robot.getSensor(4);
        err = robot.getSensor(4) - tresholds[4];
      }
    }
    else {
      if (width) {
        val = robot.getSensor(2);
        err = robot.getSensor(2) - tresholds[2];
      }
      else {
        val = robot.getSensor(1);
        err = robot.getSensor(1) - tresholds[1];
      }
    }

    u = err*KP + (err - errOld)*KD/(millis() - timer);
    errOld = err;
    timer = millis();
    if (abs(u) > 100) u = sign(u) * 100;
    sp = map(speed, 1, 8, 40, 180);


    if (state) {
      digitalWrite(LED_WORK, (millis() % 300) < 150);

      if (direction) robot.setMotors(sp + u, sp - u);
      else robot.setMotors(-sp - u, -sp + u);


      if ((!direction && (robot.getSensor(0) < tresholds[0]) || direction && (robot.getSensor(3) < tresholds[3])) && millis() - dirTimer > 1000) {
        direction = !direction;
        errOld = 0;
        dirTimer = millis();
      }

    }
    else {
      if (!robot.getError()) digitalWrite(LED_WORK, (millis() % 1500) > 1400);
      else digitalWrite(LED_WORK, 0);
      robot.stopMotors();
    }

    /*
    Serial.print("Voltage: " + String(robot.getVoltage()));
    Serial.print("  Width: " + String(width ? "5cm" : "3cm"));
    Serial.print("  Direction: " + String(direction ? "forward" : "backward"));
    Serial.print("  Speed: " + String(sp));
    Serial.print("  Val: " + String(val));
    Serial.print("  U: " + String(u));
    Serial.print("  Error: " + String(robot.getError()));
*/
    Serial.print("  Sensors: ");
    for (int i = 0; i < 6; i++)
      Serial.print(String(robot.getSensor(i)) + " ");
    Serial.println();
  }
}

void calibration(){
  Serial.println("Start calibration");

  robot.setMotors(150, -150);
  uint16_t minVal[6];
  uint16_t maxVal[6];
  for(int i = 0; i < 6; i++){
    minVal[i] = 1024;
    maxVal[i] = 0;
  }

  uint32_t calibrationTimer = millis();
  while(millis() - calibrationTimer < CALIBRATION_TIME){
    robot.wait(1);
    robot.ledArr.show(millis()%500 > 250 ? 8 : 0);
    for(int i = 0; i < 6; i++){
      minVal[i] = min(minVal[i], robot.getSensor(i));
      maxVal[i] = max(maxVal[i], robot.getSensor(i)); 
    }
  }
  robot.stopMotors();

  Serial.print("Calibration result: ");
  for(int i = 0; i < 6; i++){
    Serial.print(String(minVal[i]) + " ");
  }
  Serial.println();
  for(int i = 0; i < 6; i++){
    Serial.print(String(maxVal[i]) + " ");
  }
  Serial.println();
  for(int i = 0; i < 6; i++){
    tresholds[i] = (minVal[i] + maxVal[i]) / 2;
    Serial.print(String(tresholds[i]) + " ");
    EEPROM.write(EEPROM_TRESHOLDS + i, tresholds[i]/4);
  }
  Serial.println("");
  delay(500);
}
