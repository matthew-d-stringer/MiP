#include "Motor.h"
#include "MPU.h"
#include "LPassFilter.h"

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

LPassFilter theta(0.1);

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
  Serial.println("Theta");
}

void loop() {
  if(millis() <= 3000){
    lMotor.writeToMotor(0);
    rMotor.writeToMotor(0);
  } else if(millis() <= 6000) {
    lMotor.writeToMotor(0);
    rMotor.writeToMotor(0);
  } else {
    lMotor.writeToMotor(0);
    rMotor.writeToMotor(0);
  }

  mpu.readMPUData();
  // theta.filter(mpu.calcTheta());

  // Serial.print(mpu.calcTheta() * 180/PI);
  // Serial.print(",");
  // Serial.print(theta.getVal() * 180/PI);
  // Serial.println();

  // Serial.print(mpu.calcTheta() * 180/PI);
  // Serial.print(",");
  // Serial.println(mpu.compFilterTheta() * 180/PI);

  mpu.printGyroData();
  mpu.printAccData();

  // Serial.print("Right Encoder: ");
  // Serial.print(rMotor.getEncAngleDeg());
  // Serial.print("\tLeft Encoder: ");
  // Serial.print(lMotor.getEncAngleDeg());
  // Serial.println();

  // Serial.print("Right Encoder: ");
  // Serial.print(rMotor.encVal());
  // Serial.print("\tLeft Encoder: ");
  // Serial.print(lMotor.encVal());
  // Serial.println();
}
