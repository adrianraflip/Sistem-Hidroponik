#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#define BLYNK_PRINT Serial
BlynkTimer timer;

char auth[] = "xG15115-fjT-1rpi9-wMLvLyCOsrmbO5"; // PASTE YOUR AUTHENTICATION CODE HERE
// Your WiFi credentials.
char ssid[] = "www.tunasfarm.id"; //WIFI NAME
char pass[] = "Tunas@123987-"; //PASSWORD


char time_stamp[30];
float water_temperature;
int humidity;
int tds;
int water_tank_status;
float air_temperature;


unsigned long TIME_SCALE             = 1000;
unsigned long MINUTE_TO_SECOND       = 60;
unsigned long GET_IDEAL_VALUES_TIMER = 1;
unsigned long lastIdealValuesReqTime = 0;

const size_t mi_capacity = JSON_OBJECT_SIZE(7) + 200;
DynamicJsonDocument mi_doc(mi_capacity);


void setup() {
  Serial.begin(4800);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
  if ((millis() - lastIdealValuesReqTime) > GET_IDEAL_VALUES_TIMER * TIME_SCALE * MINUTE_TO_SECOND ) {
    checkMegaDataRoutine();
    lastIdealValuesReqTime = millis();

  }
}

void checkMegaDataRoutine() {
  if (Serial.available())
  {
    DeserializationError err = deserializeJson(mi_doc, Serial);
    if (err == DeserializationError::Ok)
    {
      Serial.println(mi_doc.as<String>());

      //      sendSensorDataToServer();
    }
    else
    {
      //      Serial.print("deserializeJson() returned ");
      //      Serial.println(err.c_str());

      while (Serial.available() > 0)
        Serial.read();
    }
  }
}




void sendSensor() {
  Serial.println(mi_doc.as<String>());

  mi_doc["request_time"] = (time_stamp);
  mi_doc["water_temperature"] = (water_temperature);
  mi_doc["air_temperature"] = (air_temperature);
  mi_doc["humidity"] = (humidity);
  mi_doc["tds"] = (tds);
  mi_doc["water_tank_status"] = (water_tank_status);


  //  Blynk.virtualWrite(V0, request_time);
  Blynk.virtualWrite(V1, water_temperature);
  Blynk.virtualWrite(V2, air_temperature);
  Blynk.virtualWrite(V3, humidity);
  Blynk.virtualWrite(V4, tds);
}
