/*
*  battery.h
*
*/

#ifndef MPU6050_H
#define MPU6050_H
#define MPU6050_H_VERSION "0"
#include "Module.h"
#include "MapSerial.h"
#define MPU_ERROR_SAMPLES 200

#define DEFMPUADDR 0x68

class MPU6050: public MapSerial {
    public:
        MPU6050(Manchester *s);
        void update();
        void setup() { setup(DEFMPUADDR); }
        void setup(int a);
        void accelSens(char mode);
        void accelSens() { accelSens(0); }
        void gyroSens(char mode);
        void gyroSens() { gyroSens(0); }
        void reconfSens();
        void init();
        void calculate_IMU_error();
        void pollAccel();
        void pollGyro();
        void poll() { pollAccel(); pollGyro(); }
        void logPRY();
        void logYaw();
        void logErrors();
        float getYaw() { return gyroAngleZ; }

    private:
        int addr;
        char mode;
        float AccX, AccY, AccZ;
        float GyroX, GyroY, GyroZ;
        float accPitch, accRoll, accYaw, gyroAngleX, gyroAngleY, gyroAngleZ;
        float roll, pitch, yaw;
        float AccErrorX, AccErrorY, AccErrorYaw, GyroErrorX, GyroErrorY, GyroErrorZ;
        float elapsedTime, currentTime, previousTime;
        int c;
};

#endif