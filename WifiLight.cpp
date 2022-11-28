#include <Arduino.h>
#include <WiFiManager.h>

class WifiLight {
private:
    WiFiManager wm;

public:
    void setup() {
        this->wm.setDebugOutput(true);
//        this->wm.resetSettings();
        if(!this->wm.autoConnect("SmartLamp","123456789")) {
            Serial.println("Failed to connect");
            //ESP.restart();
        }

        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
};