#!/usr/bin/env bash

pkill simduino
APWD=`pwd`
cd  ~/Develop/simavr/examples/board_simduino
./obj-x86_64-linux-gnu/simduino.elf &
sleep 1
cd $APWD
avrdude -p m328p -c arduino -P /tmp/simavr-uart0 -U flash:w:build-nano328/Three-Phase-Driver-AVR.hex
picocom --imap lfcrlf /tmp/simavr-uart0
