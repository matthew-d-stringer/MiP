#include "Wire.h" // This library allows you to communicate with I2C devices.

#define encoder1A 12
#define encoder1B 3
#define encoder2A 13
#define encoder2B 2

// IMU STUFF
const int MPU_ADDR = 0x68;                                 // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z;                            // variables for gyro raw data
int16_t temperature;                                       // variables for temperature data
float theta;
char tmp_str[7]; // temporary variable used in convert function

float thetaLowPass = 0;
float prevLowPass = 0;

float oldWheelAngle = 0;

float u = 0;

// converts int16 to string.
// Moreover, resulting strings will have the same length in the debug monitor.
char *convert_int16_to_str(int16_t i) {
    sprintf(tmp_str, "%6d", i);
    return tmp_str;
}

int encCount1 = 0, encCount2 = 0;

int oldMillis = 0;

void setupTimer() {
    cli();
    // Timer 1 (100ms)
    TCCR1A = 0;
    TCCR1B = 0;
    // TCCR1B |= B00000100;
    // TIMSK1 |= B00000010;
    OCR1A = 6250; // 6250

    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // TCCR1B |= 1024;  
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

void setup() {
    // Encoders
    pinMode(encoder1A, INPUT);
    pinMode(encoder1A, INPUT);
    pinMode(encoder2A, INPUT);
    pinMode(encoder2B, INPUT);

    // Motor driver
    pinMode(42, OUTPUT);
    pinMode(44, OUTPUT);
    pinMode(46, OUTPUT);
    pinMode(48, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(8, OUTPUT);

    Serial.begin(115200);

    // setupTimer();

    attachInterrupt(digitalPinToInterrupt(encoder1B), encoder1PinChange, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoder2B), encoder2PinChange, CHANGE);
}

void loop() {
    float theta = getIMUAngle();

    thetaLowPass = theta*0.1 + 0.9*thetaLowPass;

    float wheelAngle = getWheelAngle(encCount1);

//    writeToLeftMotor(theta);
//    writeToRightMotor(theta);

    Serial.print("Left Wheel: ");
    Serial.print(wheelAngle);
    Serial.print("\tRight Wheel: ");
    Serial.println(getWheelAngle(encCount2));

    timedLoop(wheelAngle);
    
    // if (theta > PI / 3)
    //     wheelAngle = 0;
    
    // Serial.print("Theta: ");
    // Serial.print(theta * 180/PI);
    // Serial.print(", Theta LP: ");
    // Serial.println(thetaLowPass * 180/PI);
}

void timedLoop(float wheelAngle) {
    int time = millis();

    if(time - oldMillis < 5) 
        return;


    // Serial.print("Timer check (");
    // Serial.print(time - oldMillis);
    // Serial.println(")");

    
    float speed = (wheelAngle - oldWheelAngle)/(time- oldMillis);
    // Serial.print("Wheel Speed: ");
//    Serial.print(time);
//    Serial.print(",");
//    Serial.println(speed * 1000);

    oldWheelAngle = wheelAngle;

    // Serial.println(wheelAngle / (2*PI));

    // u = -3.4904*(u) - 70.8202*(thetaLowPass) + 119.5462 * (prevLowPass);

    oldMillis = time;
    prevLowPass = thetaLowPass;

    // Serial.print("Theta: ");
    // Serial.print(thetaLowPass);
    // Serial.print(", motor: ");
    // Serial.println(u);

    // writeToLeftMotor(u/255);
    // writeToRightMotor(u/255);
}

void encoder1PinChange() {
    encCount1 += digitalRead(encoder1A) == digitalRead(encoder1B) ? -1 : 1;
}

void encoder2PinChange() {
    encCount2 += digitalRead(encoder2A) == digitalRead(encoder2B) ? -1 : 1;
}

float getIMUAngle() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);                        // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
    Wire.endTransmission(false);             // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
    Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

    // read IMU
    accelerometer_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    accelerometer_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    accelerometer_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    temperature = Wire.read() << 8 | Wire.read();     // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    gyro_x = Wire.read() << 8 | Wire.read();          // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    gyro_y = Wire.read() << 8 | Wire.read();          // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    gyro_z = Wire.read() << 8 | Wire.read();          // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
    
    // Calculate theta [rads]:
    float theta = atan2(accelerometer_x, accelerometer_y);
    return theta;
}

float getWheelAngle(int encoderCount) {
    return encoderCount / 1200.0 * 2 * PI;
}

void writeToLeftMotor(float input) {
    input = min(input, 1);
    input = max(input, -1);
    int sig = input >= 0;
    digitalWrite(42, !sig);
    digitalWrite(44, sig);

    float out = abs(input) * 255;
    analogWrite(8, out);
}

void writeToRightMotor(float input) {
    input = min(input, 1);
    input = max(input, -1);
    int sig = input >= 0;
    digitalWrite(46, !sig);
    digitalWrite(48, sig);

    float out = abs(input)*255;
    analogWrite(9, out);
}
