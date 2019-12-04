#include "BangServo.h"
#include <Arduino.h>

void BangServo::setup(int pin) {
    value = 1500;
    pinMode(pin, OUTPUT);
    out = pin;
    cnt = 0;
    pulse = new uTimer();
    pulse->setUS(value);
}

void BangServo::update() {
    pulse->update();
    if(pulse->event()) {
        pulse->reset();
        if(cnt % 2 == 0) {
            digitalWrite(out, HIGH);
            pulse->setUS(value);
            ++cnt;
        } else {
            digitalWrite(out, LOW);
            pulse->setUS(20000 - value);
            cnt=0;
        }
    }
}