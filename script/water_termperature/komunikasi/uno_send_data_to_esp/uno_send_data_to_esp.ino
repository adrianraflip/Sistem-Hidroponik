#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial UART(2, 3); // RX, TX
#include "RTClib.h"
RTC_DS3231 rtc;
DateTime now_time ;

const size_t mo_capacity = JSON_OBJECT_SIZE(7) + 200;
DynamicJsonDocument  mo_doc(mo_capacity);

const size_t ei_capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(15) + 290;
DynamicJsonDocument  ei_doc(mo_capacity);


unsigned long TIME_SCALE = 1000;
unsigned long ROUTINE_TIMER = 1;
unsigned long routineTimepoint = 0;

char time_stamp[30];

void setup () {
  Serial.begin(115200);
  UART.begin (4800);
  while (!Serial) {
    ;
  }
  readrtc3231 ();
}

void loop () {
  if (millis() - routineTimepoint > (ROUTINE_TIMER * TIME_SCALE)) {
    getTime ();
    sendSensorData ();
    routineTimepoint = millis();
  }
}

void readrtc3231 () {
#ifndef ESP8266
  while (!Serial) continue;
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void getTime () {
  DateTime now = rtc.now();
  now_time = rtc.now();
  sprintf(time_stamp, "%02d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  //  Serial.println (time_stamp);
}


void sendSensorData () {

  mo_doc["request_time"] = (time_stamp);
  mo_doc["water_temperature"] = random(20, 25);
  mo_doc["air_temperature"] = random(20, 25);
  mo_doc["humidity"] = random(50, 80);
  mo_doc["tds"] = random(600, 1000);
  mo_doc["water_tank_status"] = random(0, 1);

  serializeJson (mo_doc, UART);
  serializeJson (mo_doc, Serial);
  Serial.println ("");

}
