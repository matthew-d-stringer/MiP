#include "Motor.h"
#include "MPU.h"
#include "LPassFilter.h"
#include "Derivative.h"
#include "ModernController.h"

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

Derivative phiDot;
ModernController controller;

int pTime = 0;

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
  while(mpu.angleFromAcc() > 10)
    mpu.readMPUData();
  
  pTime = millis();
}

void loop() {
  mpu.readMPUData();
  compFilterOfTheta.filter(mpu.angleFromAcc(), mpu.angularRateFromGyro());

  Serial.print(mpu.angleFromAcc());
  Serial.print(',');
  Serial.println(compFilterOfTheta.getVal());

  float theta = compFilterOfTheta.getVal() * PI/180;
  float thetaRate = mpu.angularRateFromGyro() * PI/180;

  float phi = (lMotor.getEncAngle() + rMotor.getEncAngle())/2;
  float phiRate = phiDot.differentiate(phi);

  float voltage = controller.control(theta, phi, thetaRate, phiRate);

  if(abs(theta) > 30 * PI/180) 
    voltage = 0;

  int cTime = millis();
  Serial.print(cTime - pTime);
  Serial.print(",");
  Serial.print(theta * 180/PI);
  Serial.print(",");
  Serial.print(phi * 180/PI);
  Serial.print(",");
  Serial.println(voltage);

  // mpu.printAccData();

  lMotor.writeToMotor(voltage);
  rMotor.writeToMotor(voltage);

  pTime = cTime;
}