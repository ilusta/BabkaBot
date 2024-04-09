#include "Robot.h"
#include "Useful.h"
#include <EEPROM.h>

#define K   0.2
#define GREY_TRESHOLD   400

#define STATE_WAITING false
#define STATE_MOVING true

Robot robot = Robot();

void setup() {
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

  Serial.begin(115200);
}

void loop() {
  bool state = STATE_WAITING;
  int speed = EEPROM.read(0);
  long long widthChangeTimer = millis();
  bool width = EEPROM.read(1);
  bool direction = 1;
  long long dirTimer = millis();
  int val = 0;
  int u = 0;
  int sp = 0;

  while (true) {

    bool spFlag = false;
    if (robot.buttonInc.isClicked()) {
      speed += 10;
      spFlag = true;
      if (speed > 80) speed = 80;
    }
    if (robot.buttonDec.isClicked()) {
      speed -= 10;
      spFlag = true;
      if (speed < 10) speed = 10;
    }
    if (spFlag) EEPROM.write(0, speed);
    robot.ledArr.show(speed / 10);

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
      if (width) val = robot.getSensor(5);
      else val = robot.getSensor(4);
    }
    else {
      if (width) val = robot.getSensor(2);
      else val = robot.getSensor(1);
    }

    u = (val - GREY_TRESHOLD) * K;
    if (abs(u) > 50) u = sign(u) * 50;
    sp = map(speed, 10, 80, 20, 120);




    if (state) {
      digitalWrite(LED_WORK, (millis() % 300) < 150);

      if (direction) robot.setMotors(sp + u, sp - u);
      else robot.setMotors(-sp - u, -sp + u);


      if ((!direction && (robot.getSensor(0) < GREY_TRESHOLD) || direction && (robot.getSensor(3) < GREY_TRESHOLD)) && millis() - dirTimer > 2000) {
        direction = !direction;
        dirTimer = millis();
      }

    }
    else {
      if (!robot.getError()) digitalWrite(LED_WORK, (millis() % 1500) > 1400);
      else digitalWrite(LED_WORK, 0);
      robot.setMotors(0, 0);
    }

    robot.wait(5);
    Serial.print("Voltage: " + String(robot.getVoltage()));
    Serial.print("  Width: " + String(width ? "5cm" : "3cm"));
    Serial.print("  Direction: " + String(direction ? "forward" : "backward"));
    Serial.print("  Speed: " + String(sp));
    Serial.print("  Val: " + String(val));
    Serial.print("  U: " + String(u));

    Serial.print("  Sensors: ");
    for (int i = 0; i < 6; i++)
      Serial.print(String(robot.getSensor(i)) + " ");
    Serial.println();
  }
}
