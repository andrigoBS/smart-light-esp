#include "listHour/ListHour.cpp"

class Light {
private:
    bool lightOn = true;
    bool autoOn = true;
    long lastOnIn = 0;
    ListHour* offHours = new ListHour();
    ListHour* onHours = new ListHour();

public:
    bool getLightOn() {
        return this->lightOn;
    }
    void setLightOn(bool lightOn) {
        this->lightOn = lightOn;
    }

    bool getAutoOn() {
        return this->autoOn;
    }
    void setAutoOn(bool autoOn) {
        this->autoOn = autoOn;
    }

    long getLastOnIn() {
        return this->lastOnIn;
    }
    long setLastOnIn(long lastOnIn) {
        this->lastOnIn = lastOnIn;
    }

    ListHour* getOffHours() {
        return this->offHours;
    }

    ListHour* getOnHours() {
        return this->onHours;
    }
};