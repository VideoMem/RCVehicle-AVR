#include "MPU6050.h"
#include <Wire.h>

MPU6050::MPU6050(Manchester* s):MapSerial(s) {
}

void MPU6050::update() {
    poll();
    logPRY();
}

void MPU6050::setup(int a) {
    addr = a;
    c=0;
    mode=0;
    gyroAngleX = 0;
    gyroAngleY = 0;
    gyroAngleZ = 0;
    accPitch = 0;
    accRoll = 0;
    accYaw = 0;
    AccErrorYaw = 0;
    AccErrorY = 0;
    AccErrorX = 0;
    pitch=0;
    roll=0;
    yaw=0;
    init();
}

void MPU6050::init() {
    Wire.begin();                      // Initialize comunication
    Wire.beginTransmission(addr);       // Start communication with MPU6050 // MPU=0x68
    Wire.write(0x6B);                  // Talk to the register 6B
    Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
    Wire.endTransmission(true);        //end the transmission
    reconfSens();
    calculate_IMU_error();
}

void MPU6050::accelSens(char fs_sel) {
    // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
    Wire.beginTransmission(addr);
    Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
    switch (fs_sel) {
        case 1:
            Wire.write(0x08);                  //Set the register bits as 00001000 (+/- 8g full scale range)
            break;
        case 2:
            Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
            break;
        case 3:
            Wire.write(0x18);                  //Set the register bits as 00011000 (+/- 8g full scale range)
            break;
        default:
            Wire.write(0x00);                  //Set the register bits as 00000000 (+/- 2g full scale range)
            break;
    }
    Wire.endTransmission(true);
}

void MPU6050::gyroSens(char fs_sel) {
    // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
    Wire.beginTransmission(addr);
    Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
    switch (fs_sel) {
        case 1:
            Wire.write(0x08);                   // Set the register bits as 00001000 (500deg/s full scale)
            break;
        case 2:
            Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
            break;
        case 3:
            Wire.write(0x18);                   // Set the register bits as 00011000 (2000deg/s full scale)
            break;
        default:
            Wire.write(0x00);                   // Set the register bits as 00000000 (250deg/s full scale)
            break;
    }
    Wire.endTransmission(true);
}

void MPU6050::reconfSens() {
    accelSens();
    gyroSens();
    delay(100);
}

void MPU6050::pollAccel() {
    // === Read acceleromter data === //
    Wire.beginTransmission(addr);
    Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(addr, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
    AccX -= AccErrorX;
    AccY -= AccErrorY;
    AccZ -= AccErrorYaw;

    accPitch = 180 * atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) / PI;

    if(AccZ !=0)
        accRoll  = 180 * atan(-AccX / AccZ) / PI;
    else
        accRoll = 0;

    if(abs(AccX) > 0.1 && abs(AccY) > 0.1)
        accYaw = 180 * atan(AccY / AccX) / PI;
    else
        accYaw = 0;
}

void MPU6050::pollGyro() {
    // === Read gyroscope data === //
    previousTime = currentTime;        // Previous time is stored before the actual time read
    currentTime = millis();            // Current time actual time read
    elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
    Wire.beginTransmission(addr);
    Wire.write(0x43); // Gyro data first register address 0x43
    Wire.endTransmission(false);
    Wire.requestFrom(addr, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
    GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
    GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
    GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
    // Correct the outputs with the calculated error values
    GyroX -= GyroErrorX; // GyroErrorX
    GyroY -= GyroErrorY; // GyroErrorY
    GyroZ -= GyroErrorZ; // GyroErrorZ
    // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
    gyroAngleX += GyroX * elapsedTime; // deg/s * s = deg
    gyroAngleY += GyroY * elapsedTime;
    gyroAngleZ += GyroZ * elapsedTime;
}

void MPU6050::logYaw() {
    //mapSerial->printn("P", gyroAngleZ);
}

void MPU6050::logPRY() {
    // Print the values on the serial monitor
    mapSerial->print("N");
    mapSerial->print(accPitch);
    mapSerial->print("O");
    mapSerial->print(accRoll);
    logYaw();
}

void MPU6050::logErrors() {
    // Print the error values on the Serial Monitor
    mapSerial->printn("|AccErrorX:",  AccErrorX);
    mapSerial->printn("|AccErrorY:",  AccErrorY);
    mapSerial->printn("|AccErrorYaw:",  AccErrorYaw);
    mapSerial->printn("|GyroErrorX:", GyroErrorX);
    mapSerial->printn("|GyroErrorY:", GyroErrorY);
    mapSerial->printn("|GyroErrorZ:", GyroErrorZ);
}

void MPU6050::calculate_IMU_error() {
    c=0;
    while (c < MPU_ERROR_SAMPLES) {
        pollAccel();
        AccErrorX += AccX;
        AccErrorY += AccY;
        AccErrorYaw += AccZ;
        c++;
        delay(5);
    }

    AccErrorX /= (float) MPU_ERROR_SAMPLES;
    AccErrorY /= (float) MPU_ERROR_SAMPLES;
    AccErrorYaw /= (float) MPU_ERROR_SAMPLES;
    c = 0;
    while (c < MPU_ERROR_SAMPLES) {
        pollGyro();
        GyroErrorX += GyroX;
        GyroErrorY += GyroY;
        GyroErrorZ += GyroZ;
        c++;
        delay(5);
    }
    //Divide the sum by 200 to get the error value
    GyroErrorX /= (float) MPU_ERROR_SAMPLES;
    GyroErrorY /= (float) MPU_ERROR_SAMPLES;
    GyroErrorZ /= (float) MPU_ERROR_SAMPLES;
    logErrors();
}