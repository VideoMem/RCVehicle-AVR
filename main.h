#ifndef MAIN_H
#define MAIN_H
#define MAIN_VERSION "0"

#include <Arduino.h>
#include "Timers.h"         //non blocking timers
#include "Toggle.h"         //modal switchable
#include "Battery.h"        //read battery voltage
#include "Manchester.h"     //serial line encoding for IR and cheap SAW RF
#include "MapSerial.h"      //Serial extension to allow line coding
#include "BangServo.h"      //Non blocking servo control
#include "Pty.h"            //Serial terminal & G code reader
#include "MPU6050.h"        //Accelerometer & Gyro device communication

#define GYRO_ENABLED  true   //Set to true to setup and allow Gyro+Accel enabled
#define SERVO_ENABLED false  //Set to true to enable kick servos

#define UPOWER  9   //Motor U power PWM pin
#define VPOWER  10  //Motor V power PWM pin
#define UDIRF   2   //Motor U dir Forward pin
#define UDIRR   4   //Motor U Reverse dir pin
#define VDIRF   7   //Motor V dir Forward pin
#define VDIRR   8   //Motor V dir Reverse pin
#define FKICK   12  //Kick Servo PWM pin
#define RKICK   11  //Kick Servo PWM pin

#define BOOT_DELAY 100
#define NO_ERROR 500        //led blinking intervals
#define ON_ERROR 250        //
#define MINUTE 60
#define BAUDRATE 9600       //default baudrate of the serial port

const unsigned char ledPin = 13;    // LED connected to digital pin 13
static unsigned char busy = 0;

//start of globals

float lastTangle;
float lastYaw;
bool noControl;
Timer mpuTimer;
Timer blinkTimer;
Manchester* mapSerial = new Manchester();
Battery battery(mapSerial);
Pty* pty = new Pty(mapSerial);
T_gcode code;
Toggle blinker;
BangServo servoMotorA;
BangServo servoMotorB;
MPU6050* MPU = new MPU6050(mapSerial);
float maxRotationSpeed;

//main app procedures

void stopAll() {
    analogWrite(UPOWER, 0);
    analogWrite(VPOWER, 0);
    digitalWrite(UDIRF, 0);
    digitalWrite(UDIRR, 0);
    digitalWrite(VDIRF, 0);
    digitalWrite(VDIRR, 0);
}

void updateMotors() {
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

void getMaxRotationSpeed() {
    float speed;

    code.u = 0xFF;
    code.v = -0xFF;
    updateMotors();
    delay(500);
    for(int i= 0; i < 200; ++i) {
        lastYaw = MPU->getYaw();
        MPU->poll();
        speed += abs(lastYaw - MPU->getYaw());
        delay(10);
    }

    code.u = -0xFF;
    code.v = 0xFF;
    updateMotors();
    delay(500);
    for(int i= 0; i < 200; ++i) {
        lastYaw = MPU->getYaw();
        MPU->poll();
        speed += abs(lastYaw - MPU->getYaw());
        delay(10);
    }

    maxRotationSpeed = abs(speed / 400);
    code.u = 0;
    code.v = 0;
    updateMotors();
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
    mapSerial->print("Initializing ... \r\n");
    pinMode(ledPin, OUTPUT);
	blinkTimer.setMS(NO_ERROR);
    battery.check();
    noControl = false;
    if(GYRO_ENABLED) {
        MPU->setup();
        mpuTimer.setMS(50);
        lastTangle = 0;
        lastYaw = 0;
        maxRotationSpeed = 0;
        getMaxRotationSpeed();
    }
    Serial.flush();
    pty->gstatus();
    pty->setEcho(false);
    mapSerial->print("Done\r\n");
}

float asDEG(float angle) {
    return 180 * angle / PI;
}

float asRAD(float angle) {
    return PI * angle / 180;
}

float tractionAngle() {
    if(code.u == 0 && code.v == 0) return 0;
    float u = code.u;
    float v = code.v;
    float angle = code.u >= 0? atan(v / u): atan(v / u) + PI;
    return asDEG(angle) - 45;
}

float tractionNorm() {
    float norm = sqrt(pow(code.u, 2) + pow(code.v, 2)) / 0xFF;
    return norm;
}

float rotationSpeed() {
    return tractionNorm() * sin(asRAD(tractionAngle()));
}

void tractionControl() {
    if(noControl) return;
    //float scale = 2;
    float dAngle = rotationSpeed() * maxRotationSpeed; // * scale;
    float dYaw = lastYaw - MPU->getYaw();
    float gain = 10;
    float drift = (dAngle - dYaw) * gain;
    int u = code.u;
    int v = code.v;

    if (drift > 0) {
        u += abs(round(drift));
        v -= abs(round(drift));
    } else {
        u -= abs(round(drift));
        v += abs(round(drift));
    }

    if(u > 255) u = 255;
    if(v > 255) v = 255;
    if(u < -255) u = -255;
    if(v < -255) v = -255;

    code.u = u;
    code.v = v;
}

void tractionDrive() {
    if(mpuTimer.event()) {
        lastYaw = MPU->getYaw();
        lastTangle = tractionAngle();
        MPU->poll();
        if(GYRO_ENABLED) tractionControl();
        mpuTimer.reset();
    }
    mpuTimer.update();
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

void gyroDrive() {
    if (code.m == 10) {
        MPU->logPRY();
        mapSerial->printn("P", rotationSpeed() - lastYaw);
        pty->resetMcode();
    }
}

void servoDrive() {
    servoMotorA.update();
    servoMotorB.update();
    if(code.m == 8) {
        servoMotorA.write(90);
        servoMotorB.write(45);
    }

    if (code.m == 9) {
        servoMotorA.write(45);
        servoMotorB.write(90);
    }
}

void batteryDrive() {
    if(pty->getLast() == 98) { //B
        mapSerial->print("B");
        battery.check();
        //battery.logValue();
        mapSerial->print(maxRotationSpeed);
        pty->flush();
    }
}


void caterpillarDrive() {
    if(code.u == 0 && code.v == 0) {
        busy=0;
        stopAll();
    } else {
        busy=1;
    }
    updateMotors();
}

void bitPrint(bool bit) {
    bit == true? mapSerial->print("1\r\n"): mapSerial->print("0\r\n");
}

void sendFWCapabilites() {
    mapSerial->print("|SERVO:"); bitPrint(SERVO_ENABLED);
    mapSerial->print("|GYRO:"); bitPrint(GYRO_ENABLED);
    pty->resetMcode();
}

void sendFWErr() {
    MPU->logErrors();
    pty->resetMcode();
}

void recalibrateIMU() {
    MPU->calculate_IMU_error();
    pty->resetMcode();
}

void loop() {
    ledDrive();
    battery.update();
    errorDrive();
    pty->update();
    pty->gcode(code);

    batteryDrive();
    if(SERVO_ENABLED) servoDrive();
    if(GYRO_ENABLED)  gyroDrive();
    if(code.m == 99)  sendFWCapabilites();
    if(code.m == 100) sendFWErr();
    if(code.m == 101) recalibrateIMU();
    if(code.m == 20) { noControl = true; pty->resetMcode(); }
    if(code.m == 21) { noControl = false; pty->resetMcode(); }
    tractionDrive();
    caterpillarDrive();
}

#endif