class CompFilter {
private:
    float alpha;
    float pValue;
    bool started = false;
public:
    CompFilter(float alpha) {
        this->alpha = alpha;
    }

    float filter(float lowFreqVal, float highFreqVal) {
        if(!started) {
            started = true;
            pValue = lowFreqVal;
            return pValue;
        }
        pValue = (1-alpha) * (pValue + highFreqVal) + alpha * lowFreqVal;
    }
    
    float getVal() {
        return pValue;
    }
};