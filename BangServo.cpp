#include "BangServo.h"

void BangServo::setup(int pin) {
    value = 1500;
    pinMode(pin, OUTPUT);
    out = pin;
    cnt = 0;
    pulse = new uTimer();
    pulse->setUS(value);
    setBlocking(false);
}

void BangServo::update() {
    pulse->update();
    if(pulse->event()) {
        pulse->reset();
        if(cnt % 2 == 0) {
            digitalWrite(out, HIGH);
            pulse->setUS(value);
            ++cnt;
            if(blocking) {
                while(!pulse->event())
                    pulse->update();
                update();
            }
        } else {
            digitalWrite(out, LOW);
            pulse->setUS(20000 - value);
            cnt=0;
        }
    }
}