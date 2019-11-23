/*
*  VFD.h
*
*/

#ifndef VFD_H
#define VFD_H
#define VFD_VERSION "0"
#include "MapSerial.h"
#include "Timers.h"
#include "Motor.h"
#include <inttypes.h>

#define VFD_MOTOR_POLE_PAIRS 1
#define VFD_SPD_SAMP_MS 100
#define VFD_SPD_RAMP_MS 1
#define VFD_SPD_LOG_MS 3000
#define FCPU_CLK 16e6
#define TMRPRESCALE 1024

class VFD : public MapSerial {
    public:
        VFD(Manchester* s);
        void setup();
        void update();
        void target(unsigned char ch, int speed) { tspd[ch] = speed; }
    protected:
        void speed();
        void spdReg();
        float spd[VFD_MOTOR_LOOPS];
        uint16_t  tspd[VFD_MOTOR_LOOPS];
        uint16_t  rspd[VFD_MOTOR_LOOPS];
        void setOCR1A(uint16_t reg);
        uint16_t getOCR1A() { return OCR1A; }
        Timer* speedSample;
        Timer* logSample;
        Timer* rampSample;
        long lastMotorPos[VFD_MOTOR_LOOPS];
};


#endif