#include "Battery.h"
#include <Arduino.h>

void Battery::setup() {
    batTimer.setS(BATSAMP);
    batTimer.reset();
}

Battery::Battery(Manchester* s):MapSerial(s) {
    setup();
}

bool Battery::alarm() {
    bool error = false;
    if(vBat <= MINIMAL_V_LEVEL) 
        error = true;
    return error;
}

void Battery::check() {
    int aux = analogRead(VBATPIN);
    vBat = 8.46 * (float) aux / 1023.0;
    //logValue();
}

void Battery::logAlarm() {
    mapSerial->print("vBat LOW!\r\n");
}

void Battery::logValue() {
    //mapSerial->print("vBat: ");
    mapSerial->print(vBat);
    //mapSerial->print("\n");
}

void Battery::update() {
    if(batTimer.event()) {
        check();
        //if(alarm())
         //   logAlarm();
        batTimer.reset();
    }
    batTimer.update();
}
