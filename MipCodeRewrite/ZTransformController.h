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
        float output = 0;
        prevInputs.increment();
        prevInputs[0] = input;

        for(int i = 0; i < prevInLen; i++)
            output += num[i] * prevInputs[i];
        for(int i = 1; i < prevOutLen; i++)
            output -= dem[i] * prevOutputs[i];
        output /= dem[0];

        prevOutputs.increment();
        prevOutputs[0] = output;

        return output;
    }
};