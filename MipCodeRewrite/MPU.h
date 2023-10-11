#include <Wire.h>
#include "CompFilter.h"
#define MPU_I2C_Addr 0x68

class MPU {
public:
    float AccX, AccY, AccZ;
    float GyroX, GyroY, GyroZ;

    float AccErrorX = 0, AccErrorY = 0, AccErrorZ = 0;
    float GyroErrorX, GyroErrorY, GyroErrorZ;

    float XCalibrationVal = -7;

    CompFilter* Theta;
    CompFilter* Theta2;

    int pMillis = 0;
    int pMillis2 = 0;
public:
    MPU(){
        Theta = new CompFilter(0.9);
        Theta2 = new CompFilter(0.1);
    }

    void setup() {
        Wire.begin();
        Wire.beginTransmission(MPU_I2C_Addr);
        Wire.write(0x6B);
        Wire.write(0x00);
        Wire.endTransmission(true);
    }
    
    void readMPUData() {
        Wire.beginTransmission(MPU_I2C_Addr);
        Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_I2C_Addr, 6, true); 

        AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorX; // X-axis value
        AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorY; // Y-axis value
        AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorZ; 

        Wire.beginTransmission(MPU_I2C_Addr);
        Wire.write(0x43); // Gyro data first register address 0x43
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_I2C_Addr, 6, true); 

        GyroX = (Wire.read() << 8 | Wire.read()) / 131.0 - GyroErrorX; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
        GyroY = (Wire.read() << 8 | Wire.read()) / 131.0 - GyroErrorY;
        GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0 - GyroErrorZ;
    }

    void readRawMPUData() {
        Wire.beginTransmission(MPU_I2C_Addr);
        Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_I2C_Addr, 6, true); 

        AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
        AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
        AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; 

        Wire.beginTransmission(MPU_I2C_Addr);
        Wire.write(0x43); // Gyro data first register address 0x43
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_I2C_Addr, 6, true); 

        GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
        GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
        GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
    }

    void calibrate() {
        int numReadings = 1000;
        for(int c = 0; c < numReadings; c++) {
            readRawMPUData();
            AccErrorX += AccX;
            AccErrorY += AccY;
            AccErrorZ += AccZ - 1;
            GyroErrorX += GyroX;
            GyroErrorY += GyroY;
            GyroErrorZ += GyroZ;
        }
        AccErrorX /=  numReadings;
        AccErrorY /=  numReadings;
        AccErrorZ /=  numReadings;
        GyroErrorX /= numReadings;
        GyroErrorY /= numReadings;
        GyroErrorZ /= numReadings;

        Serial.println("Calibrated!");
    }

    float angleFromAcc() {
        return atan2(AccZ, -AccY) * 180/PI;
    }

    float compFilterTheta() {
        return Theta->filter(angleFromAcc(), GyroX);
    }

    float angularRateFromGyro() {
        return GyroX;
    }

    void printAccData() {
        // Serial.print("Acc Data (x,y,z):");
        Serial.print(angleFromAcc());
        Serial.print(",");
        Serial.print(AccX);
        Serial.print(",");
        Serial.print(AccY);
        Serial.print(",");
        Serial.print(AccZ);
        Serial.println();
    }
    void printGyroData() {
        Serial.print("Gyro Data (x,y,z):");
        Serial.print(GyroX);
        Serial.print(",");
        Serial.print(GyroY);
        Serial.print(",");
        Serial.print(GyroZ);
        Serial.println();
    }
};
