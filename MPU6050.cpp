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
    gyroAngleX = 0;
    gyroAngleY = 0;
    gyroAngleZ = 0;
    accAngleX = 0;
    accAngleY = 0;

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
    delay(200);
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
    // Calculating Roll and Pitch from the accelerometer data
    accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI); // - AccErrorX; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
    accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI); // - AccErrorY; // AccErrorY ~(-1.58)
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
    //GyroX -= GyroErrorX; // GyroErrorX
    //GyroY -= GyroErrorY; // GyroErrorY
    //GyroZ -= GyroErrorZ; // GyroErrorZ
    // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
    gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
    gyroAngleY = gyroAngleY + GyroY * elapsedTime;
    yaw =  yaw + GyroZ * elapsedTime;
    // Complementary filter - combine acceleromter and gyro angle values
    float rollDrift = gyroAngleX - accAngleX;
    float pitchDrift = gyroAngleY - accAngleY;
    gyroAngleX-=(0.1*rollDrift);
    gyroAngleY-=(0.1*pitchDrift);
    //roll = gyroAngleX;
    //roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
    //pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
}
void MPU6050::logPRY() {
    // Print the values on the serial monitor
    mapSerial->print("N");
    mapSerial->print(gyroAngleX);
    mapSerial->print("O");
    mapSerial->print(gyroAngleY);
    mapSerial->print("Q");
    mapSerial->print(accAngleX);
    mapSerial->print("R");
    mapSerial->print(accAngleY);
    mapSerial->print("\r\n");

}

void MPU6050::calculate_IMU_error() {
    // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
    // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
    // Read accelerometer values 200 times

    while (c < 2000) {
        Wire.beginTransmission(addr);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(addr, 6, true);
        AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
        AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
        AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
        // Sum all readings
        AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
        AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
        c++;
    }
    //Divide the sum by 200 to get the error value
    AccErrorX = AccErrorX / 2000;
    AccErrorY = AccErrorY / 2000;
    c = 0;
    // Read gyro values 200 times
    while (c < 2000) {
        Wire.beginTransmission(addr);
        Wire.write(0x43);
        Wire.endTransmission(false);
        Wire.requestFrom(addr, 6, true);
        GyroX = Wire.read() << 8 | Wire.read();
        GyroY = Wire.read() << 8 | Wire.read();
        GyroZ = Wire.read() << 8 | Wire.read();
        // Sum all readings
        GyroErrorX = GyroErrorX + (GyroX / 131.0);
        GyroErrorY = GyroErrorY + (GyroY / 131.0);
        GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
        c++;
    }
    //Divide the sum by 200 to get the error value
    GyroErrorX = GyroErrorX / 2000;
    GyroErrorY = GyroErrorY / 2000;
    GyroErrorZ = GyroErrorZ / 2000;

    // Print the error values on the Serial Monitor
    mapSerial->printn("AccErrorX: ", AccErrorX);
    mapSerial->printn("AccErrorY: ", AccErrorY);
    mapSerial->printn("GyroErrorX: ", GyroErrorX);
    mapSerial->printn("GyroErrorY: ", GyroErrorY);
    mapSerial->printn("GyroErrorZ: ", GyroErrorZ);
}