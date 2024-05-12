# Banka robot
This repository contains frimware, PCB design and 3D models for creating Banka robot

"Banka" robots are used as moving obstacles in robotics competitions. This obstacles follow black line, changing direction at each white point. Competitor`s robots should follow the same black line from start to finish without touching obstacles. The fastest robot to do so wins.

Working time with middle speed setting ~6cm/s is more than 11 hours.
Working time with high speed setting ~18cm/s is more than 7 hours.

This is 2.0 version

Frimware is written for ATmega 328p with Arduino Nano bootloader

![Banka obstacle on the line](https://github.com/ilusta/Banka/assets/53097569/0f489ca3-9ea4-4f60-a427-c0d966306ce1)

## List of components:
In the following table listed main components for building one robot. Only main PCB components are included.
| Position | Quantity | Link |
| ------------- | ------------- | ------------- |
| Mi Power Bank 3 Ultra Compact (PB1022ZM) | 1 | - |
| Pololu 150:1 6V Micro Metal Gearmotor | 4 | [Link](https://www.pololu.com/product/3066) |
| RW2i | 4 | [Link](https://www.pololu.com/product/1127) |
| ATmega328P | 1 | [Link](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) |
| DRV8833 | 1 | [Link](https://www.ti.com/lit/ds/symlink/drv8833.pdf?ts=1715452574521&ref_url=https%253A%252F%252Fwww.google.com%252F) |
| CP2102 | 1 | [Link](https://www.silabs.com/documents/public/data-sheets/CP2102-9.pdf) |
| TCRT5000 | 6 | [Link](https://www.vishay.com/docs/83760/tcrt5000.pdf) |


## Instruction
Code can be compiled and uploaded using Arduino IDE. Select Arduino Nano as board type and upload using devug header on the PCB.

* During initialization short animation will be shown on the strip of green LEDs.
* Under normal conditions green LED will blink slowly indicating waiting state. If motor driver error occures (overheat or too high current), red LED will light up indicating error state.
* Robot can be started or stoped using corresponding button. In working state green LED will blink fast. Robot will continuously follow black line between two white dots on it`s center.
* If error occures in working state robot will stop.
* Robot`s speed can be changed between one of 8 levels using corresponding buttons. Current speed is shown on the strip of green LEDs.
* Robot can move alon 3cm or 5 cm line. To switch between line width hold start button for longer than 10 seconds. After 10 seconds current mode will be shown on the strip of green LEDs (two central LEDs - 3cm line, all LEDs except first and last - 5cm)
