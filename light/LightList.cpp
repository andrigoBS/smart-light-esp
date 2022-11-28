#include "Light.cpp"

class LightList {
private:
    const static unsigned short int numOfLamps = 2;
    Light* light[numOfLamps];
    unsigned short int brightness[2] = {12, 100};
    unsigned int maxOnTime = 5000;

public:
    LightList() {
        for(unsigned short int i = 0; i < numOfLamps; i++) {
            this->light[i] = new Light();
        };
    }

    unsigned short int getNumOfLamps() {
        return numOfLamps;
    }

    Light* getLight(int index) {
        return light[index];
    }

    unsigned short int getMinBrightness() {
        return this->brightness[0];
    }
    unsigned short int getMaxBrightness() {
        return this->brightness[1];
    }
    void setBrightness(unsigned short int minBrightness, unsigned short int maxBrightness) {
        this->brightness[0] = minBrightness;
        this->brightness[1] = maxBrightness;
    }

    unsigned int getMaxOnTime() {
        return this->maxOnTime;
    }
    void setMaxOnTime(unsigned int maxOnTime) {
        this->maxOnTime = maxOnTime;
    }
};
