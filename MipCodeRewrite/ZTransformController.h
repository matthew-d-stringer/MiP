#include "CircVector.h"

class ZTransformController {
private:
    float *num, *dem;
    int prevInLen, prevOutLen;

    CircVector prevInputs;
    CircVector prevOutputs;

public:
    ZTransformController(float *num, int numLen, float *dem, int demLen)
    :prevInputs(numLen), prevOutputs(demLen) 
    {
        this->num = num;
        this->dem = dem;
        this->prevInLen = numLen;
        this->prevOutLen = demLen;

        reset();
    }
    void reset() {
        prevInputs.reset();
        prevOutputs.reset();
    }
    float control(float input) {
        Serial.print("y-1 = ");
        prevOutputs.printVals();

        float output = 0;

        prevInputs.increment();
        prevInputs[0] = input;

        prevOutputs.increment();

        Serial.print("u = ");
        prevInputs.printVals();
        Serial.print("y0 = ");
        prevOutputs.printVals();

        for(int i = 0; i < prevInLen; i++){
            output += num[i] * prevInputs[i];
        }
        Serial.print("Output0 = ");
        Serial.println(output);
        for(int i = 1; i < prevOutLen; i++){
            output -= dem[i] * prevOutputs[i];
        }
        Serial.print("Output1 = ");
        Serial.println(output);
        output /= dem[0];
        Serial.print("Output2 = ");
        Serial.println(output);

        prevOutputs[0] = output;

        Serial.print("y = ");
        prevOutputs.printVals();

        return output;
    }
};