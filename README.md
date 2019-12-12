# RC Vehicle Control - AVR

RC control Atmega 328p (Nano/ Uno /Micro)

Select your board editing the BOARD_TAG in MakeFile 

As an example:
    BOARD_TAG = nano328

$ make show_boards 

To show all the supported boards

## Prerequisites
Install the required AVR+Arduino stack, you also will need Python 2.7 for the console.
More info: https://github.com/sudar/Arduino-Makefile

### Upload firmware
Connect nano/uno/micro trough usb serial port and execute

$ make upload

### Console (Manchester line coding) Optional if mapSerial->setDirect(false); in main.h

$ python console/console.py

Manchester line code is intended for use with IR and RF SAW modules (disabled per default)
It will slow down your baudrate in half but improves clock recovery and signal quality.

### TODO

Link to main Android Bluetooth controller app 


