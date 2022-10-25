#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESPDateTime.h>

#define LDR 35
#define LED 12
#define SENSOR 13
#define PWM1_Ch 0
#define PWM1_Res 12
#define PWM1_Freq 1024

// variaveis do sistema
long lastOnIn = 0;
StaticJsonDocument<150> jsonDocument;
char buffer[150];
String body = "";
unsigned short int nowMinutes = 0;
unsigned short int offTimeStart = 0;
unsigned short int offTimeEnd = 0;

// variaveis do usuario
short int offHoursStart = -1;
short int offMinutesStart = -1;
short int offHoursEnd = -1;
short int offMinutesEnd = -1;
unsigned short int maxBrightness = 100;
unsigned short int minBrightness = 12;
bool lightOn = true;
bool autoOn = true;
unsigned int maxOnTime = 5000;
char* ssid = "WIFI-NOME";      
char* password = "WIFI-PASSWORD"; 

WebServer server(80);

void getBrightness(){
  jsonDocument.clear(); 
  jsonDocument["minBrightness"] = minBrightness;
  jsonDocument["maxBrightness"] = maxBrightness;
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}

void postBrightness() {
  body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  minBrightness = jsonDocument["minBrightness"];
  maxBrightness = jsonDocument["maxBrightness"];
  server.send(200, "application/json", "{\"success\": \"OK\"}");
}

void requestBrightness(){
   if (server.method() == HTTP_GET){
      getBrightness();
      return;
   }
   if(server.method() == HTTP_POST){
      postBrightness();
      return;
   }
}

void getLight(){
  jsonDocument.clear();  
  jsonDocument["lightOn"] = lightOn;
  jsonDocument["autoOn"] = autoOn;
  jsonDocument["maxOnTime"] = maxOnTime;
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}

void postLightOn() {
  body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  lightOn = jsonDocument["lightOn"];
  server.send(200, "application/json", "{\"success\": \"OK\"}");
}

void postAutoOn() {
  body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  autoOn = jsonDocument["autoOn"];
  server.send(200, "application/json", "{\"success\": \"OK\"}");
}

void postMaxOnTime() {
  body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  maxOnTime = jsonDocument["maxOnTime"];
  server.send(200, "application/json", "{\"success\": \"OK\"}");
}

void getOffTime(){
  jsonDocument.clear(); 
  jsonDocument["offHoursStart"] = offHoursStart;
  jsonDocument["offMinutesStart"] = offMinutesStart;
  jsonDocument["offHoursEnd"] = offHoursEnd;
  jsonDocument["offMinutesEnd"] = offMinutesEnd;
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}

void postOffTime() {
  body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  Serial.println(body);
  offHoursStart = jsonDocument["offHoursStart"];
  offMinutesStart = jsonDocument["offMinutesStart"];
  offHoursEnd = jsonDocument["offHoursEnd"];
  offMinutesEnd = jsonDocument["offMinutesEnd"];
  server.send(200, "application/json", "{\"success\": \"OK\"}");
}

void requestOffTime(){
   if (server.method() == HTTP_GET){
      getOffTime();
      return;
   }
   if(server.method() == HTTP_POST){
      postOffTime();
      return;
   }
}

void setupServerRoutes() {         
  server.on("/brightness", requestBrightness);      
  server.on("/light", getLight);
  server.on("/light-on", postLightOn);
  server.on("/auto-on", postAutoOn);
  server.on("/max-on-time", postMaxOnTime);
  server.on("/off-time", requestOffTime);
  server.begin();    
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT);
  pinMode(SENSOR, INPUT);

  ledcAttachPin(LED, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);

  while (WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    for (unsigned short int retries = 0; retries < 10; retries++) {
        delay(500);
        Serial.print(".");
        if(WiFi.status() == WL_CONNECTED) break;
    }
  }

  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  DateTime.setTimeZone("<-03>3");
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
  
  setupServerRoutes();
}

void loop() {
  server.handleClient();

  unsigned int ldrValue = analogRead(LDR);

  if(ldrValue > maxBrightness * (4096.0/100)){
    ldrValue = maxBrightness * (4096.0/100);
  }

  if(autoOn) {
    if(ldrValue < minBrightness * (4096.0/100)){
      ldrValue = 0;
    }
  
    if (digitalRead(SENSOR) == HIGH) {  
      lastOnIn = millis();
    }else{
      if(millis() - lastOnIn > maxOnTime){
          ldrValue = 0;
      }
    }

    if(offHoursStart != -1 && offMinutesStart != -1 && offHoursEnd != -1 && offMinutesEnd != -1){
      nowMinutes = DateTime.getParts().getHours() * 60 + DateTime.getParts().getMinutes();
      offTimeStart = offHoursStart * 60 + offMinutesStart;
      offTimeEnd = offHoursEnd * 60 + offMinutesEnd;
     
      if(nowMinutes > offTimeStart && nowMinutes < offTimeEnd){
        ldrValue = 0;
      }
    }
  }else{
    if(!lightOn){
      ldrValue = 0;
    } 
  }
  
  ledcWrite(PWM1_Ch, ldrValue);
}
