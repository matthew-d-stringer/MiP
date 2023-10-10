class Integrator {
private:
    float pValue;
    float pMillis;
    bool started = false;
public:
    Integrator() {
    }

    float integrate(float val) {
        if(!started) {
            started = true;
            pMillis = millis();
            pValue = val;
            return pValue;
        }
        float cMillis = millis();
        float dt = (cMillis - pMillis)/1000;
        
        pValue = pValue + val * dt;

        pMillis = cMillis;

        return pValue;
    }

    float getValue() {
        return pValue;
    }
};
