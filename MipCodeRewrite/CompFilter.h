class CompFilter {
private:
    float alpha;
    float pValue;
    float pMillis;
    bool started = false;
public:
    CompFilter(float alpha) {
        this->alpha = alpha;
    }

    float filter(float lowFreqVal, float highFreqVal) {
        if(!started) {
            started = true;
            pValue = lowFreqVal;
            pMillis = millis();
            return pValue;
        }
        float cMillis = millis();
        float dt = (cMillis - pMillis)/1000.0;
        pValue = alpha * (highFreqVal*dt + pValue) + (1-alpha) * lowFreqVal;

        pMillis = cMillis;
        return pValue;
    }
    
    float getVal() {
        return pValue;
    }
};