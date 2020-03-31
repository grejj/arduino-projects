# Digital Watch

Example implementation of a reaction timer on the Arduino microcontroller for embedded systems class project. The reaction timer measures the reaction time of a user in response to a visual stimulus of an LED.

## Hardware

The implementation of the reaction timer includes the following hardware:
1. 1 x Arduino microcontroller
2. 1 x push button
3. 1xLED
4. 1 x LCD screen
5. 1 x 10K resistor
6. 1 x 1K resistor

Push button is on digital I/O pin 10 with a 10K pulldown resistor when switch is open. LED is on digital I/O pin 9 with a 1K resistor to limit the current on the diode.
The LCD needs to be wired to the microcontroller power, ground, and digital I/O pins (12, 11, 5, 4, 3, and 2).

## Software

All code for the project can be found in the reaction_timer.ino file.

## Implementation

To implement the reaction timer, upload the code to the Arduino, connect all of the hardware, and follow these steps:

1. Upon startup, the Arduino will turn on the LED between 2 seconds and 4 seconds of startup.
2. After the LED turns on, the push the push button to display the reaction time (in
microseconds) on the LCD.
3. The LED will then turn off and a new cycle begins.

Note: If the user waits longer than 8 seconds to push the button after the LED turns on, the LCD will display a “TIMEOUT” message, and the system will reset after another 8 seconds.