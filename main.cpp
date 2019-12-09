#include <Arduino.h>
#include "Timers.h"
#include "Toggle.h"
#include "Battery.h"
#include "Manchester.h"
#include "MapSerial.h"
#include "BangServo.h"
#include "Pty.h"
#include "MPU6050.h"

//#ifdef __cplusplus
//extern "C" {
//#include <avr/avr_mcu_section.h>
//AVR_MCU(F_CPU, "atmega328p");
//}
//#endif

#define BOOT_DELAY 100
#define NO_ERROR 500
#define ON_ERROR 250
#define MINUTE 60
#define BAUDRATE 9600

const unsigned char ledPin = 13;                             // LED connected to digital pin 13
static unsigned char busy = 0;

Timer blinkTimer;
Manchester* mapSerial = new Manchester();
Battery battery(mapSerial);
//Motor* Motor::pointer;//
//volatile Motor mstate;
//volatile int counter = 0;
//VFD* Drive = new VFD(mapSerial);
Pty* pty = new Pty(mapSerial); //, Drive);
T_gcode code;
Toggle blinker;
BangServo servoMotorA;
BangServo servoMotorB;

//ISR (TIMER1_COMPA_vect) {
//    Motor::pointer->advance(0);
//}

#define UPOWER  9
#define VPOWER  10
#define UDIRF   2
#define UDIRR   4
#define VDIRF   7
#define VDIRR   8
#define FKICK   12
#define RKICK   11

void stopAll() {
    analogWrite(UPOWER, 0);
    analogWrite(VPOWER, 0);
    digitalWrite(UDIRF, 0);
    digitalWrite(UDIRR, 0);
    digitalWrite(VDIRF, 0);
    digitalWrite(VDIRR, 0);
}

void setup() {
    pinMode(VBATPIN, INPUT);  //vbat
    pinMode(UPOWER,  OUTPUT); //upower
    pinMode(VPOWER,  OUTPUT); //vpower
    pinMode(UDIRF,   OUTPUT); // u > 0
    pinMode(UDIRR,   OUTPUT); // u < 0
    pinMode(VDIRF,   OUTPUT); // v > 0
    pinMode(VDIRR,   OUTPUT); // v < 0
    servoMotorA.setup(FKICK);
    servoMotorB.setup(RKICK);
    //servoMotorA.setBlocking(true);
    //servoMotorB.setBlocking(true);
    //pinMode(KICK,    OUTPUT);
    //pinMode(RKICK,   OUTPUT);
    stopAll();
    Serial.begin(BAUDRATE);
    //while(true) {
    //    if(Serial.available())
    //        Serial.write(Serial.read());
    //}

    mapSerial->setDirect(true);
    mapSerial->print("Initializing ... \n");
    pinMode(ledPin, OUTPUT);
	blinkTimer.setMS(NO_ERROR);
    battery.check();


    Serial.flush();
  //  Drive->setup();
    pty->gstatus();
    pty->setEcho(false);
    mapSerial->print("Done\n");
    unsigned long step = 0;

    /*while (true) {

        step = millis();
        step %= 800;
        step += 1000;
        servoMotorA.write(step);
        servoMotorA.update();
        //delay(1000);
    }*/

}

void ledDrive() {
    if(blinker.value() == true)
        digitalWrite(ledPin, HIGH);   // set the LED on
    else
        digitalWrite(ledPin, LOW);    // set the LED off

    if(blinkTimer.event()) {
        blinker.change();
        blinkTimer.reset();
    }
    
    blinkTimer.update();
}

void errorDrive() {
    if(busy == 1) {
        blinkTimer.setMS(ON_ERROR);         
    } else {
        blinkTimer.setMS(NO_ERROR);         
    }
}

//void motorDrive() {
//    Drive->update();
//}

void loop() {
    ledDrive();
    battery.update();
    errorDrive();
    //motorDrive();
    pty->update();
    pty->gcode(code);
    servoMotorA.update();
    servoMotorB.update();
    if(pty->getLast() == 98) { //B
        mapSerial->print("B");
        battery.check();
        battery.logValue();
        pty->flush();
    }

    if(code.m == 8) {
        servoMotorA.write(90);
        servoMotorB.write(45);
    } else {
        servoMotorA.write(45);
        servoMotorB.write(90);
    }


    if(code.u == 0 && code.v == 0) {
        busy=0;
        stopAll();
    } else {
        busy=1;
    }

    if(code.u > 0) {
        digitalWrite(UDIRF,1); digitalWrite(UDIRR, 0);
        analogWrite(UPOWER, code.u);
    } else {
        digitalWrite(UDIRF,0); digitalWrite(UDIRR, 1);
        analogWrite(UPOWER, -code.u);
    }

    if(code.v > 0) {
        digitalWrite(VDIRF,1); digitalWrite(VDIRR, 0);
        analogWrite(VPOWER, code.v);
    } else {
        digitalWrite(VDIRF,0); digitalWrite(VDIRR, 1);
         analogWrite(VPOWER, -code.v);
    }

}

int main(void) {
    init();
    setup();

    for (;;) {
       loop();
    }
}

