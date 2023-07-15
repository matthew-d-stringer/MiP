//bool LED_STATE = true;
#include "Wire.h" // This library allows you to communicate with I2C devices.

#define outputA 12
#define outputB 3
#define outputA2 13
#define outputB2 2

// IMU STUFF
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
float theta;
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str; 

}

// ENCONDER STUFF
int counter1 = 0;  
int counter2 = 0; 

float x1;
float x2;

//Control Stuff
float prevu = 0;
float u = 0;

float prevreftheta = 0;
float reftheta = 0;

float prevtheta = 0;

float prevx1 = 0;

float ftheta = 0;
float prevftheta = 0;

float etheta;
float prevetheta;


//GAIN
int K = 0;

  
void setup() {
  // setup:
  //pinMode(LED_BUILTIN, OUTPUT);
  cli();
  //Timer 1 (100ms)
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000100;
  TIMSK1 |= B00000010;
  OCR1A = 6250;
  
  sei();
  // setup pins and stuff:
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //Pinmodes w/ PR
  DDRL = B10101010; 
  DDRB = B00010000;
  DDRE = B00000000;
  DDRH = B01100000;


  //Encoder Interrupts
  
  attachInterrupt(digitalPinToInterrupt(outputB), encoder1PinChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(outputB2), encoder2PinChange, CHANGE);
  

}

void loop() {
  // main shit here (data processing):

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers

  // read IMU
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  // Calculate theta [rads]:
  theta = atan2(accelerometer_x,accelerometer_y);
  
  prevftheta = ftheta;
  ftheta = 0.9*prevftheta + 0.1*theta;
  Serial.print(theta); Serial.print(", "); Serial.println(ftheta);

  if (theta > pi/3) 
    x1 = 0;



  // MOTOR calcs
  prevx1 = x1;
  x1 = -counter1/1200.0*2*3.14159;
  x2 = counter2/1200.0*2*3.14159;

  prevreftheta = reftheta;
  reftheta = 3.1152*prevreftheta + 41.2773*(-x1) - 49.5016*(-prevx1);
  
  prevetheta = etheta;
  etheta = reftheta - ftheta;
  
  
  
  //Serial.print(theta);  Serial.print(", "); Serial.println(ftheta);
  //Serial.println(digitalRead(outputB2));

}

ISR(TIMER1_COMPA_vect){ // 100ms
  TCNT1 = 0;

  

  u = 3.4904*(u) - 70.8202*(etheta) + 119.5462 * (prevetheta);
  //Serial.println(u);
  //u *= 170;

  if (u > 0) 
  {
    //Set Motor Pins to high/low (direction) using PR
    PORTL = B10001000;

    
    if (u > 255){
      u = 255;
    }


    
    
    prevreftheta = reftheta;
    //Serial.println(u);  //Serial.print(", "); Serial.println(ftheta);

    analogWrite(9, u); analogWrite(8, u);// -theta*360.);
    //Serial.println(theta*360.);
  }
  else if (u < 0)
  {
    //Set Motor Pins to high/low (direction) using PR
    PORTL = B00100010;

    
    if (u < -255){
      u = -255;
    }

    prevu = u;
    prevreftheta = reftheta;
    //Serial.println(u);

    
    analogWrite(9, -u); analogWrite(8, -u);// theta*360.);
  }
}


void encoder1PinChange() {
  counter1 += digitalRead(outputA) == digitalRead(outputB) ? -1 : 1;
  //Serial.println("yep1");
}

void encoder2PinChange() {
  //Serial.println("yep2");  
  counter2 += digitalRead(outputA2) == digitalRead(outputB2) ? -1 : 1;
}