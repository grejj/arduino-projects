# Digital Watch

Example digital watch implementation on the Arduino microcontroller for embedded systems class project. The digital watch allows the user to set and display the time. The watch provides other functions like a timer and a stopwatch.

## Hardware

The implementation of the digital watch includes the following hardware:
1. 1 x Arduino microcontroller
2. 1 x push buttons
3. 1 x LCD screen
4. 1 x LED
5. 1 x IR sensor
6. 1 x IR remote
7. 1 x 10K resistor
8. 1 x 1K resistor

Push button is on digital I/O pin 10 with a 10K pulldown resistor when switch is open. LED is on digital I/O pin 9 with a 1K resistor to limit the current on the diode.bThe LCD needs to be wired to the microcontroller power, ground, and digital I/O pins (12, 11, 5, 4, 3, and 2). IR receiver is on digital I/O pin 7.

## Software

All code for the project can be found in the digital_watch.ino file.

## Implementation

To implement the digital watch, upload the code to the Arduino, connect all of the hardware, and follow these steps:

1. IR remote button 1 sets mode to recode date and time
2. Press two number buttons on the IR remote to enter in the hour then push button on breadboard to lock in the number (note if push button is not pressed in two seconds after number values are entered from the remote then user will have a chance to re- enter the two numbers from the remote and lock input with the push button)
3. Repeat step 2 for minutes, seconds, year, month, and day 
4. Press button 0 on the IR remote to display date and time
5. Press button 2 on IR remote to enter lap timer mode
6. Record lap time and reset lap time are mapped to buttons 1 and 2 on the IR remote respectively
7. Press button 0 on the IR remote to exit lap timer
8. Press button 3 on the IR remote to enter countdown timer mode
9. Enter the hours, minutes, and seconds with the IR remote number buttons and locking the input with the push button on the breadboard
10. Press the push button to start the count down; the user can pause and resume the count down by pressing the push button
11. Once count down is done, LED will light up
12. Press 0 on the IR remote to exit count down mode and enter display date and time mode