# Banka robot
This repository contains frimware, PCB design and 3D models for creating Banka robot

"Banka" robots are used as moving obstacles in robotics competitions. This obstacles follow black line, changing direction at each white point. Competitor`s robots should follow the same black line from start to finish without touching obstacles. The fastest robot to do so wins.

This is 1.0 version

Frimware is written for ATmega 328p with Arduino Nano bootloader

![Banka obstacle on the line](https://github.com/ilusta/Banka-Software/assets/53097569/fada17c3-299c-468b-aab5-f4e726ebad1e)
![Robot itself](https://github.com/ilusta/Banka-Software/assets/53097569/6df76e79-b856-4f35-95cc-2d21a84091d1)

## Instruction
Code can be compiled and uploaded using Arduino IDE. Select Arduino Nano as board type and upload using devug header on the PCB.

* During initialization short animation will be shown on the strip of green LEDs.
* Under normal conditions green LED will blink slowly indicating waiting state. If input voltage isn`t equal to 9v, red LED will light up indicating error state.
* Robot can be started or stoped using corresponding button. In working state green LED will blink fast. Robot will continuously follow black line between two white dots on it`s center.
* If error occures in working state robot will stop.
* Robot`s speed can be changed between one of 8 levels using corresponding buttons. Current speed is shown on the strip of green LEDs.
* Robot can move alon 3cm or 5 cm line. To switch between line width hold start button for longer than 10 seconds. After 10 seconds current mode will be shown on the strip of green LEDs (two central LEDs - 3cm line, all LEDs except first and last - 5cm)
