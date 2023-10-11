class Motor {
private:
    int encCount;
    
    int encoderA;
    int encoderB;
    int motorSignPin1;
    int motorSignPin2;
    int motorValPin;

    bool flipOutput = false;
    bool flipEncCount = false;

    int ticksPerRev = 1250;
public:
    Motor(int motorSignPin1, int motorSignPin2, int motorValPin, 
        int encoderA, int encoderB) {

        encCount = 0;

        this->encoderA = encoderA;
        this->encoderB = encoderB;
        this->motorSignPin1 = motorSignPin1;
        this->motorSignPin2 = motorSignPin2;
        this->motorValPin = motorValPin;

        pinMode(encoderA, INPUT);
        pinMode(encoderB, INPUT);

        pinMode(motorSignPin1, OUTPUT);
        pinMode(motorSignPin2, OUTPUT);
        pinMode(motorValPin, OUTPUT);
    }

    void reverseOutput(bool flipOutput) {
        this->flipOutput = flipOutput;
    }

    void reverseEnc(bool flipEncCount) {
        this->flipEncCount = flipEncCount;
    }

    void writeToMotor(float voltage) {
        float input = voltage/7.4;
        input = min(input, 1);
        input = max(input, -1);
        input = flipOutput ? -input : input;
        int sig = input >= 0;
        digitalWrite(motorSignPin1, !sig);
        digitalWrite(motorSignPin2, sig);

        float out = abs(input) * 255;
        analogWrite(motorValPin, out);
    }

    void encoderPinChange() {
//        Serial.println("Enc change");
        if(flipEncCount)
            encCount -= digitalRead(encoderA) == digitalRead(encoderB) ? -1 : 1;
        else 
            encCount += digitalRead(encoderA) == digitalRead(encoderB) ? -1 : 1;
    }

    void attachEncInterrupt(void (*functionPtr)()){
        attachInterrupt(digitalPinToInterrupt(encoderB), functionPtr, CHANGE);
    }

    int encVal() {
        return encCount;
    }

    float getEncAngle() {
        return (float) encCount /  (float) ticksPerRev * 2 * PI;
    }

    float getEncAngleDeg() {
        return (float) encCount / (float) ticksPerRev * 360;
    }
};
