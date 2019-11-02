#!/bin/bash

avr-gcc -MMD -c -I /usr/local/include/simavr -L /usr/local/lib -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=105  -D__PROG_TYPES_COMPAT__ -I /usr/share/arduino/hardware/arduino//cores/arduino -I /usr/share/arduino/hardware/arduino//variants/eightanaloginputs  -Wall -ffunction-sections -fdata-sections -Os -fno-exceptions -std=gnu99 -flto simavr.c -o build-nano328/simavr.c.o

