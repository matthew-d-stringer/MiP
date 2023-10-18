#if 0
#include "Motor.h"
#include "MPU.h"
#include "LPassFilter.h"
#include "Derivative.h"
#include "ModernController.h"
#include "ZTransformController.h"

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
// ModernController controller;

float numVals[] = {2881.769393, -5512.656553, 2635.884492};
float demVals[] = {1, -1.7759, 0.7884};

ZTransformController classicalController(
  numVals, 3, 
  demVals, 3
);

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
  // Serial.println("Raw Gyro Acc");
  // while(mpu.angleFromAcc() > 10)
  //   mpu.readMPUData();
  
  pTime = millis();

  classicalController.reset();
}

void loop() {
  mpu.readMPUData();
  compFilterOfTheta.filter(mpu.angleFromAcc(), mpu.angularRateFromGyro());

  // Serial.print(mpu.angleFromAcc());
  // Serial.print(',');
  // Serial.println(compFilterOfTheta.getVal());

  float theta = compFilterOfTheta.getVal() * PI/180;
  // float thetaRate = mpu.angularRateFromGyro() * PI/180;

  // float phi = (lMotor.getEncAngle() + rMotor.getEncAngle())/2 + theta;
  // float phiRate = phiDot.differentiate(phi) + thetaRate;

  // float voltage = controller.control(theta, phi, thetaRate, phiRate);
  float voltage = classicalController.control(theta);

  if(abs(theta) > 30 * PI/180) 
    voltage = 0;

  int cTime = millis();
  // Serial.print(cTime - pTime);
  // Serial.print(",");
  Serial.print(theta * 180/PI);
  Serial.print(",");
  // Serial.print(phi * 180/PI);
  // Serial.print(",");
  Serial.println(voltage);

  // mpu.printAccData();

  lMotor.writeToMotor(voltage);
  rMotor.writeToMotor(voltage);

  pTime = cTime;
}
#elif 0 
#include "CircVector.h"

int counter = 0;
CircVector test(3);

void setup() {
  Serial.begin(115200);

  for(int something = 0; something < 20; something++) {
    test.increment();
    test[0] = counter;

    Serial.print("{");
    for(int i = 0; i < test.len(); i++) {
        Serial.print(test[i]);
        if(i != test.len() - 1)
            Serial.print(",");
    }
    Serial.println("}");
    counter++;
  }
}

void loop() {
}
#else 
#include "ZTransformController.h"

// float numVals[] = {2881.769393, -5512.656553, 2635.884492};
// float demVals[] = {1, -1.7759, 0.7884};

float numVals[] = {2881.769393, -5512.656553, 2635.884492};
float demVals[] = {1, -1.7759, 0.7884};
// float numVals[] = {3, -5, 3};
// float demVals[] = {1, -2, 1};

ZTransformController classicalController(
  numVals, 3, 
  demVals, 3
);

int count = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if(count < 10) {
    Serial.println(classicalController.control(1));
    count++;
  }
}

#endif