#include <Arduino.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include "VFD.h"

void VFD::setOCR1A(uint16_t reg) {
    OCR1A = reg;
}

void VFD::spdReg() {
    float f0 = FCPU_CLK * VFD_MOTOR_POLE_PAIRS / TMRPRESCALE;
    float fx = f0 / OCR1A;
    float sp0 = 3000;
    uint16_t tg0 = (FCPU_CLK * VFD_MOTOR_POLE_PAIRS * 60) / (TMRPRESCALE * sp0 * VFD_3PH_STATES);
    mapSerial->printn("sp0: ", sp0);
    mapSerial->printn("tg0: ", tg0);
}

void VFD::setup() {
    pinMode(UPH, OUTPUT);
    pinMode(VPH, OUTPUT);
    pinMode(WPH, OUTPUT);

    //timer1 isr setup
    TCCR1A = 0; // no waveform generation
    setOCR1A(52);
    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match
    // Mode 4, CTC on OCR1A
    // set prescaler to 1024 and start the timer
    TCCR1B = 0b00001101;
    TCNT1 = 0;
    sei();

    speedSample = new Timer();
    logSample = new Timer();
    speedSample->setMS(VFD_SPD_SAMP_MS);
    logSample->setMS(VFD_SPD_LOG_MS);
}

void VFD::speed() {
    long diff[VFD_MOTOR_LOOPS];
    diff[0] = Motor::pointer->Position[0] - lastMotorPos[0];

    spd[0] = 1000.0 / (float) VFD_SPD_SAMP_MS;
    spd[0] *= 60.0;
    spd[0] *= diff[0];
    spd[0] /= (float) (VFD_3PH_STATES * VFD_MOTOR_POLE_PAIRS);

    lastMotorPos[0] = Motor::pointer->Position[0];
}

void VFD::update() {
    if(speedSample->event()) {
        speed();
        speedSample->reset();
    }

    if(logSample->event()) {
        spdReg();
        mapSerial->print("spd: ");
        mapSerial->print(spd[0]);
        mapSerial->print("\n");
        mapSerial->print("Pos: ");
        mapSerial->print(Motor::pointer->Position[0]);
        mapSerial->print("\n");
        logSample->reset();
    }

    speedSample->update();
    logSample->update();
}

VFD::VFD(Manchester *s):MapSerial(s) {
    lastMotorPos[0] = 0;
    //setup();
}
