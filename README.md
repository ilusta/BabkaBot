# Software for Banka robot
"Banka" robots are used as moving obstacles in robotics competitions

"Banka" project can be found here:
https://github.com/ilusta/Banka

This is frimware of 1.0 version for 1.0 version of PCB

This frimware is written for ATmega 328p with Arduino Nano bootloader

## Instruction
Code can be compiled and uploaded using Arduino IDE. Select Arduino Nano as board type and upload using devug header on the PCB.

* During initialization short animation will be shown on the strip of green LEDs.
* Under normal conditions green LED will blink slowly indicating waiting state. If input voltage isn`t equal to 9v, red LED will light up indicating error state.
* Robot can be started or stoped using corresponding button. In working state green LED will blink fast. Robot will continuously follow black line between two white dots on it`s center.
* If error occures in working state robot will stop.
* Robot`s speed can be changed between one of 8 levels using corresponding buttons. Current speed is shown on the strip of green LEDs.
* Robot can move alon 3cm or 5 cm line. To switch between line width hold start button for longer than 10 seconds. After 10 seconds current mode will be shown on the strip of green LEDs (two central LEDs - 3cm line, all LEDs except first and last - 5cm)
