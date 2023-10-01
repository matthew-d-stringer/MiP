#include <Wire.h>
#define MPU_I2C_Addr 0x68

class MPU {
private:
    float AccX, AccY, AccZ;
    float GyroX, GyroY, GyroZ;
public:
    MPU(){
    }

    void setup() {
        Wire.begin();
        Wire.beginTransmission(MPU_I2C_Addr);
        Wire.write(0x6B);
        Wire.write(0x00);
        Wire.endTransmission(true);
    }
    // char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
    //     sprintf(tmp_str, "%6d", i);
    //     return tmp_str; 
    // }
    
    void readMPUData() {
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

    float calcTheta() {
        return atan2(AccZ, -AccY) + 5.71*PI/180;
    }

    void printAccData() {
        Serial.print("Acc Data (x,y,z):");
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
