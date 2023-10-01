class Motor {
private:
    int encCount;
    
    int encoderA;
    int encoderB;
    int motorSignPin1;
    int motorSignPin2;
    int motorValPin;
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

    void writeToMotor(float input) {
        input = min(input, 1);
        input = max(input, -1);
        int sig = input >= 0;
        digitalWrite(motorSignPin1, !sig);
        digitalWrite(motorSignPin2, sig);

        float out = abs(input) * 255;
        analogWrite(motorValPin, out);
    }

    void encoderPinChange() {
//        Serial.println("Enc change");
        encCount += digitalRead(encoderA) == digitalRead(encoderB) ? -1 : 1;
    }

    void attachEncInterrupt(void* functionPtr){
        attachInterrupt(digitalPinToInterrupt(encoderB), functionPtr, CHANGE);
    }

    int encVal() {
        return encCount;
    }
};
