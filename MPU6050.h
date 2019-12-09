/*
*  battery.h
*
*/

#ifndef MPU6050_H
#define MPU6050_H
#define MPU6050_H_VERSION "0"
#include "Module.h"
#include "MapSerial.h"


class MPU6050: public MapSerial {
    public:
        MPU6050(Manchester *s);
        void update();
        void setup();

};

#endif