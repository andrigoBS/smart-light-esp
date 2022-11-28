#include <WebServer.h>
#include "../light/LightList.cpp"
#include "WebLightData.cpp"

WebServer server(80);

WebLightData* webLightData = nullptr;

void requestBrightness(){
    if (server.method() == HTTP_GET){
        server.send(200, "application/json", webLightData->getBrightness());
        return;
    }
    if(server.method() == HTTP_POST){
        server.send(200, "application/json", webLightData->postBrightness(server.arg("plain")));
        return;
    }
}

void getLight(){
    server.send(200, "application/json", webLightData->getLight());
}

void postLightOn() {
    server.send(200, "application/json", webLightData->postLightOn(server.arg("plain")));
}

void postAutoOn() {
    server.send(200, "application/json", webLightData->postAutoOn(server.arg("plain")));
}

void postMaxOnTime() {
    server.send(200, "application/json", webLightData->postMaxOnTime(server.arg("plain")));
}

void requestOffTime(){
    if (server.method() == HTTP_GET){
        server.send(200, "application/json", webLightData->getOffTime());
        return;
    }
    if(server.method() == HTTP_POST){
        server.send(200, "application/json", webLightData->postOffTime(server.arg("plain")));
        return;
    }
    Serial.println(server.method());
    if(server.method() == HTTP_DELETE) {
        server.send(200, "application/json", webLightData->deleteOffTime(server.arg("plain")));
        return;
    }
}

void requestOnTime(){
    if (server.method() == HTTP_GET){
        server.send(200, "application/json", webLightData->getOnTime());
        return;
    }
    if(server.method() == HTTP_POST){
        server.send(200, "application/json", webLightData->postOnTime(server.arg("plain")));
        return;
    }
    if(server.method() == HTTP_DELETE) {
        server.send(200, "application/json", webLightData->deleteOnTime(server.arg("plain")));
        return;
    }
}

void WebLightSetup(LightList* lightList){
    if(webLightData == nullptr) {
        webLightData = new WebLightData(lightList);
    }

    server.on("/brightness", requestBrightness);
    server.on("/light", getLight);
    server.on("/light-on", postLightOn);
    server.on("/auto-on", postAutoOn);
    server.on("/max-on-time", postMaxOnTime);
    server.on("/off-time", requestOffTime);
    server.on("/on-time", requestOnTime);
    server.begin();
}

void WebLightListen() {
    server.handleClient();
}