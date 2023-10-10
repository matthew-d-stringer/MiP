#include "Motor.h"
#include "MPU.h"
#include "LPassFilter.h"
#include "Integrator.h"

#define LMotorPinSign1 42
#define LMotorPinSign2 44
#define RMotorPinSign1 46
#define RMotorPinSign2 48
  
#define LMotorValPin 8
#define RMotorValPin 9

#define encoder1A 12
#define encoder1B 3
#define encoder2A 13
#define encoder2B 2

Motor lMotor(LMotorPinSign1, LMotorPinSign2, LMotorValPin, encoder1A, encoder1B);
Motor rMotor(RMotorPinSign1, RMotorPinSign2, RMotorValPin, encoder2A, encoder2B);

MPU mpu;

CompFilter compFilterOfTheta(0.98);

static void lMotorPinInterrupt() {
  lMotor.encoderPinChange();
}
static void rMotorPinInterrupt() {
  rMotor.encoderPinChange();
}

void setup() {
  lMotor.attachEncInterrupt(lMotorPinInterrupt);
  rMotor.attachEncInterrupt(rMotorPinInterrupt);

  lMotor.reverseOutput(true);
  rMotor.reverseEnc(true);
  
  Serial.begin(115200);
  mpu.setup();
  mpu.calibrate();
  Serial.println("Raw Gyro Acc");
}

void loop() {
  mpu.readMPUData();
  compFilterOfTheta.filter(mpu.angleFromAcc(), mpu.angularRateFromGyro());

  Serial.print(mpu.angleFromAcc());
  Serial.print(',');
  Serial.println(compFilterOfTheta.getVal());
}
