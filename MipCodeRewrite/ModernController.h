class ModernController {
private:
    float k1 = -94.5300;
    float k2 = -0.6301;
    float k3 = -20.4620;
    float k4 = -1.2421;
public:
    ModernController() {
    }

    float control(float theta, float phi, float thetaRate, float phiRate) {
        float u = theta * k1 + phi * k2 + thetaRate * k3 + phiRate * k4;
        return u;
    }
};