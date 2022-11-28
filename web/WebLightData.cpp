#include <Arduino.h>
#include <ArduinoJson.h>

class WebLightData {
private:
    StaticJsonDocument<150> jsonDocument;
    char buffer[150];
    LightList* lightList;

public:
    WebLightData(LightList* lightList){
        this->lightList = lightList;
    }

    String getBrightness(){
        this->jsonDocument.clear();
        this->jsonDocument["minBrightness"] = this->lightList->getMinBrightness();
        this->jsonDocument["maxBrightness"] = this->lightList->getMaxBrightness();
        serializeJson(this->jsonDocument, this->buffer);
        return this->buffer;
    }

    String postBrightness(String body) {
        deserializeJson(this->jsonDocument, body);
        this->lightList->setBrightness(this->jsonDocument["minBrightness"], this->jsonDocument["maxBrightness"]);
        return "{\"success\": \"OK\"}";
    }

    String getLight(){
        this->jsonDocument.clear();
        for(short int i = 0; i < this->lightList->getNumOfLamps(); i++) {
            this->jsonDocument["lights"][i]["lightOn"] = this->lightList->getLight(i)->getLightOn();
            this->jsonDocument["lights"][i]["autoOn"] = this->lightList->getLight(i)->getAutoOn();
        }
        this->jsonDocument["maxOnTime"] = this->lightList->getMaxOnTime();
        serializeJson(this->jsonDocument, this->buffer);
        return this->buffer;
    }

    String postLightOn(String body) {
        deserializeJson(this->jsonDocument, body);
        this->lightList->getLight(this->jsonDocument["lightIndex"])->setLightOn(this->jsonDocument["lightOn"]);
        return "{\"success\": \"OK\"}";
    }

    String postAutoOn(String body) {
        deserializeJson(this->jsonDocument, body);
        this->lightList->getLight(this->jsonDocument["lightIndex"])->setAutoOn(this->jsonDocument["autoOn"]);
        return "{\"success\": \"OK\"}";
    }

    String postMaxOnTime(String body) {
        deserializeJson(this->jsonDocument, body);
        this->lightList->setMaxOnTime(this->jsonDocument["maxOnTime"]);
        return "{\"success\": \"OK\"}";
    }

    String getOffTime(){
        this->jsonDocument.clear();

        for(short int l = 0; l < this->lightList->getNumOfLamps(); l++) {
            for (short int i = 0; i < this->lightList->getLight(l)->getOffHours()->getSize(); i++) {
                Hour *hour = this->lightList->getLight(l)->getOffHours()->getHour(i);
                this->jsonDocument[i]["hoursStart"] = hour->getHoursStart();
                this->jsonDocument[i]["minutesStart"] = hour->getMinutesStart();
                this->jsonDocument[i]["hoursEnd"] = hour->getHoursEnd();
                this->jsonDocument[i]["minutesEnd"] = hour->getMinutesEnd();
                this->jsonDocument[i]["lightIndex"] = l;
            }
        }

        serializeJson(this->jsonDocument, this->buffer);
        return this->buffer;
    }

    String postOffTime(String body) {
        deserializeJson(this->jsonDocument, body);
        bool success = this->lightList->getLight(this->jsonDocument["lightIndex"])->getOffHours()->addHour(
                this->jsonDocument["offHoursStart"],
                this->jsonDocument["offMinutesStart"],
                this->jsonDocument["offHoursEnd"],
                this->jsonDocument["offMinutesEnd"]
        );
        return ((String) "{\"success\": \"") + ((String) success? "OK": "MAXSIZE") + ((String)"\"}");
    }

    String deleteOffTime(String body) {
        deserializeJson(this->jsonDocument, body);
        this->lightList->getLight(this->jsonDocument["lightIndex"])->getOffHours()->deleteHour(this->jsonDocument["timeIndex"]);
        return "{\"success\": \"OK\"}";
    }

    String getOnTime(){
        this->jsonDocument.clear();

        for(short int l = 0; l < this->lightList->getNumOfLamps(); l++) {
            for (short int i = 0; i < this->lightList->getLight(l)->getOnHours()->getSize(); i++) {
                Hour *hour = this->lightList->getLight(l)->getOnHours()->getHour(i);
                this->jsonDocument[i]["hoursStart"] = hour->getHoursStart();
                this->jsonDocument[i]["minutesStart"] = hour->getMinutesStart();
                this->jsonDocument[i]["hoursEnd"] = hour->getHoursEnd();
                this->jsonDocument[i]["minutesEnd"] = hour->getMinutesEnd();
                this->jsonDocument[i]["lightIndex"] = l;
            }
        }

        serializeJson(this->jsonDocument, this->buffer);
        return this->buffer;
    }

    String postOnTime(String body) {
        deserializeJson(this->jsonDocument, body);
        bool success = this->lightList->getLight(this->jsonDocument["lightIndex"])->getOnHours()->addHour(
                this->jsonDocument["onHoursStart"],
                this->jsonDocument["onMinutesStart"],
                this->jsonDocument["onHoursEnd"],
                this->jsonDocument["onMinutesEnd"]
        );
        return ((String) "{\"success\": \"") + ((String) success? "OK": "MAXSIZE") + ((String)"\"}");
    }

    String deleteOnTime(String body) {
        deserializeJson(this->jsonDocument, body);
        this->lightList->getLight(this->jsonDocument["lightIndex"])->getOnHours()->deleteHour(this->jsonDocument["timeIndex"]);
        return "{\"success\": \"OK\"}";
    }
};
