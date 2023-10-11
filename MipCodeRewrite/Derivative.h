class Derivative {
private:
    float pValue;
    float pMillis;
    float out;
    bool started = false;
public:
    Derivative() {
    }

    float differentiate(float val) {
        if(!started) {
            started = true;
            pMillis = millis();
            pValue = val;
            return 0;
        }
        float cMillis = millis();
        float dt = (cMillis - pMillis)/1000;
        
        out = (val - pValue) / dt;

        pValue = val;
        pMillis = cMillis;

        return out;
    }

    float getValue() {
        return out;
    }
};
