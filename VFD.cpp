#include <Arduino.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include "VFD.h"

void VFD::setOCR1A(uint16_t reg) {
    if(reg < OCR1A) {
       TCNT1 = 0;
    }
    OCR1A = reg;
}

void VFD::spdReg() {
    //float f0 = FCPU_CLK * VFD_MOTOR_POLE_PAIRS / TMRPRESCALE;
    //float fx = f0 * 10 / OCR1A;
    float sp0 = rspd[0]; // ) ? fx-0.1: fx+0.1;
    //int16_t ocr = OCR1A;
    //float spx = (FCPU_CLK * VFD_MOTOR_POLE_PAIRS * 60) / (TMRPRESCALE * ocr * VFD_3PH_STATES);
    uint16_t tg0 = (FCPU_CLK * VFD_MOTOR_POLE_PAIRS * 60) / (TMRPRESCALE * sp0 * VFD_3PH_STATES);
    setOCR1A(tg0);
    //mapSerial->printn("spx: ", fx);
    //mapSerial->printn("tg0: ", tspd[0]);
}

void VFD::setup() {
    pinMode(UPH, OUTPUT);
    pinMode(VPH, OUTPUT);
    pinMode(WPH, OUTPUT);
    tspd[0] = 33;
    rspd[0] = 33;
    //timer1 isr setup
    TCCR1A = 0; // no waveform generation
    setOCR1A(52);
    //spdReg();
    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match
    // Mode 4, CTC on OCR1A
    // set prescaler to 1024 and start the timer
    TCCR1B = 0b00001101;
    TCNT1 = 0;
    sei();

    speedSample = new Timer();
    logSample = new Timer();
    rampSample = new Timer();
    speedSample->setMS(VFD_SPD_SAMP_MS);
    logSample->setMS(VFD_SPD_LOG_MS);
    rampSample->setMS(VFD_SPD_RAMP_MS);
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
        mapSerial->print("CNT: ");
        mapSerial->print(TCNT1);
        mapSerial->print("\n");
        mapSerial->print("OCR: ");
        mapSerial->print(OCR1A);
        mapSerial->print("\n");
        mapSerial->print("tsp: ");
        mapSerial->print(tspd[0]);
        mapSerial->print("\n");
        mapSerial->print("rsp: ");
        mapSerial->print(rspd[0]);
        mapSerial->print("\n");
        mapSerial->print("spd: ");
        mapSerial->print(spd[0]);
        mapSerial->print("\n");
        mapSerial->print("pos: ");
        mapSerial->print(Motor::pointer->Position[0]);
        mapSerial->print("\n");
        logSample->reset();
    }

    if(rampSample->event()) {
        rspd[0] = rspd[0] < tspd[0] ? rspd[0]+1: rspd[0]-1;
        if(tspd[0] < 900) rspd[0] = tspd[0];
        //    rspd[0] = tspd[0];
        //rspd[0] = tspd[0] < 1000 ? tspd[0]: rspd[0];
        spdReg();
        rampSample->reset();
    }

    speedSample->update();
    logSample->update();
    rampSample->update();
}

VFD::VFD(Manchester *s):MapSerial(s) {
    lastMotorPos[0] = 0;
    //setup();
}
