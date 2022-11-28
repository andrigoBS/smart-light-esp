#include <Arduino.h>
#include <ESPDateTime.h>
#include "wifi/WifiLight.cpp"
#include "web/WebLight.cpp"

#define LDR  35 // sensor de luminosidade
#define MOV  13 // sensor de movimento
#define LED1 12 // led um
#define BT1  27 // botão um
#define LED2 32 // led dois
#define BT2  26 // botão dois
#define POW  33 // potenciometro
#define PWM1_Ch 0
#define PWM1_Res 12
#define PWM1_Freq 1024
#define PWM2_Ch 1
#define PWM2_Res 12
#define PWM2_Freq 1024

WifiLight wifiLight;
LightList* lightList = new LightList();
unsigned short int nowMinutes = 0;
unsigned short int timeStart = 0;
unsigned short int timeEnd = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  ledcAttachPin(LED1, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);

  pinMode(LED2, OUTPUT);
  ledcAttachPin(LED2, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);
  
  pinMode(MOV, INPUT);

  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);

  wifiLight.setup();

  DateTime.setTimeZone("<-03>3");
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
  
  WebLightSetup(lightList);
}

void loop() {
  WebLightListen();

  unsigned int ldrValue = analogRead(LDR);

  if(ldrValue > lightList->getMaxBrightness() * (4096.0/100)){
    ldrValue = lightList->getMaxBrightness() * (4096.0/100);
  }

  for(short int i = 0; i < lightList->getNumOfLamps(); i++) {
    if(lightList->getLight(i)->getAutoOn()){
      if(ldrValue < lightList->getMinBrightness() * (4096.0/100)){
        ldrValue = 0;
      }

      if (digitalRead(MOV) == HIGH) {  
        lightList->getLight(i)->setLastOnIn(millis());
      }else{
        if(millis() - lightList->getLight(i)->getLastOnIn() > lightList->getMaxOnTime()){
            ldrValue = 0;
        }
      }

      nowMinutes = DateTime.getParts().getHours() * 60 + DateTime.getParts().getMinutes();
      for(short int j = 0; j < lightList->getLight(i)->getOffHours()->getSize(); j++) {
        timeStart = lightList->getLight(i)->getOffHours()->getHour(j)->getHoursStart() * 60 + 
                    lightList->getLight(i)->getOffHours()->getHour(j)->getMinutesStart()
        ;
        timeEnd = lightList->getLight(i)->getOffHours()->getHour(j)->getHoursEnd() * 60 + 
                  lightList->getLight(i)->getOffHours()->getHour(j)->getMinutesEnd()
        ;
        if(nowMinutes > timeStart && nowMinutes < timeEnd){
          ldrValue = 0;
        }
      }

      for(short int j = 0; j < lightList->getLight(i)->getOnHours()->getSize(); j++) {
        timeStart = lightList->getLight(i)->getOnHours()->getHour(j)->getHoursStart() * 60 + 
                    lightList->getLight(i)->getOnHours()->getHour(j)->getMinutesStart()
        ;
        timeEnd = lightList->getLight(i)->getOnHours()->getHour(j)->getHoursEnd() * 60 + 
                  lightList->getLight(i)->getOnHours()->getHour(j)->getMinutesEnd()
        ;
        if(nowMinutes > timeStart && nowMinutes < timeEnd){
          ldrValue = analogRead(LDR);

          if(ldrValue > lightList->getMaxBrightness() * (4096.0/100)){
            ldrValue = lightList->getMaxBrightness() * (4096.0/100);
          }
        }
      }
    }else{
      if(!lightList->getLight(i)->getLightOn() || (i == 0 && !digitalRead(BT1)) || (i == 1 && !digitalRead(BT2))){
        ldrValue = 0;
      } 
    }
    
    if(i == 0) {
      ledcWrite(PWM1_Ch, ldrValue);
    }else {
      ledcWrite(PWM2_Ch, ldrValue);
    }
  }
}
