#include <Arduino.h>
#include "Timers.h"         //non blocking timers
#include "Toggle.h"         //modal switchable
#include "Battery.h"        //read battery voltage
#include "Manchester.h"     //serial line encoding for IR and cheap SAW RF
#include "MapSerial.h"      //Serial extension to allow line coding
#include "BangServo.h"      //Non blocking servo control
#include "Pty.h"            //Serial terminal & G code reader
#include "MPU6050.h"        //Accelerometer & Gyro device communication

#define UPOWER  9   //Motor U power PWM pin
#define VPOWER  10  //Motor V power PWM pin
#define UDIRF   2   //Motor U dir Forward pin
#define UDIRR   4   //Motor U Reverse dir pin
#define VDIRF   7   //Motor V dir Forward pin
#define VDIRR   8   //Motor V dir Reverse pin
#define FKICK   12  //Kick Servo PWM pin
#define RKICK   11  //Kick Servo PWM pin

#define GYRO_ENABLED true   //Set to true to setup and allow Gyro+Accel enabled
#define SERVO_ENABLED true  //Set to true to enable kick servos
#define BOOT_DELAY 100
#define NO_ERROR 500        //led blinking intervals
#define ON_ERROR 250        //
#define MINUTE 60
#define BAUDRATE 9600       //default baudrate of the serial port

const unsigned char ledPin = 13;    // LED connected to digital pin 13
static unsigned char busy = 0;

//start of globals

Timer blinkTimer;
Manchester* mapSerial = new Manchester();
Battery battery(mapSerial);
Pty* pty = new Pty(mapSerial);
T_gcode code;
Toggle blinker;
BangServo servoMotorA;
BangServo servoMotorB;
MPU6050* MPU = new MPU6050(mapSerial);

//main app procedures

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

    if(SERVO_ENABLED) {
        servoMotorA.setup(FKICK);
        servoMotorB.setup(RKICK);
    }

    stopAll();
    Serial.begin(BAUDRATE);

    mapSerial->setDirect(true);
    mapSerial->print("Initializing ... \n");
    pinMode(ledPin, OUTPUT);
	blinkTimer.setMS(NO_ERROR);
    battery.check();

    if(GYRO_ENABLED) MPU->setup();
    Serial.flush();
    pty->gstatus();
    pty->setEcho(false);
    mapSerial->print("Done\n");
    unsigned long step = 0;
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

void loop() {
    ledDrive();
    battery.update();
    errorDrive();
    pty->update();
    pty->gcode(code);

    if(SERVO_ENABLED) {
        servoMotorA.update();
        servoMotorB.update();
    }

    if(pty->getLast() == 98) { //B
        mapSerial->print("B");
        battery.check();
        battery.logValue();
        pty->flush();
    }

    if(code.m == 8 && SERVO_ENABLED) {
        servoMotorA.write(90);
        servoMotorB.write(45);
    }

    if (code.m == 9 && SERVO_ENABLED) {
        servoMotorA.write(45);
        servoMotorB.write(90);
    }

    if (code.m == 10 && GYRO_ENABLED) {
        MPU->update();
        pty->resetMcode();
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

