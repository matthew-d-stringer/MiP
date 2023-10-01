class LPassFilter {
private:
    float alpha;
    float prevValue;
    bool started = false;
public:
    LPassFilter(float alpha) {
        this->alpha = alpha;
    }

    float filter(float value) {
        if(!started){
            started = true;
            prevValue = value;
            return value;
        }
        prevValue = value * alpha + prevValue * (1-alpha);
        return prevValue;
    }

    float getVal() {
        return prevValue;
    }
};