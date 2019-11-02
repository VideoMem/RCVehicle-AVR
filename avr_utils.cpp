#include <Arduino.h>

void initPins(unsigned char* pins, unsigned char mode, unsigned char num) {
    unsigned char i = 0;
    for(i=0; i < num; i++)
        pinMode(pins[i],mode);
}