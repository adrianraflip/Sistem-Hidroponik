#include <ArduinoJson.h>

unsigned long TIME_SCALE             = 1000;
unsigned long MINUTE_TO_SECOND       = 60;
unsigned long GET_IDEAL_VALUES_TIMER = 1;
unsigned long lastIdealValuesReqTime = 0;

const size_t mi_capacity = JSON_OBJECT_SIZE(7) + 200;
DynamicJsonDocument mi_doc(mi_capacity);


void setup() {
  Serial.begin(9600);
}

void loop() {

  if ((millis() - lastIdealValuesReqTime) > GET_IDEAL_VALUES_TIMER * TIME_SCALE * MINUTE_TO_SECOND ) {
    checkMegaDataRoutine();
    Serial.println ("");
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
