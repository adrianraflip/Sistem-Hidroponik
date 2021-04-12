#include "RTClib.h"
RTC_DS3231 rtc;
DateTime now_time ;

unsigned long TIME_SCALE = 1000;
unsigned long ROUTINE_TIMER = 1;
unsigned long routineTimepoint = 0;

char time_stamp[30];

void setup () {
  Serial.begin(9600);
  readrtc3231 ();
}

void loop () {
  if (millis() - routineTimepoint > (ROUTINE_TIMER * TIME_SCALE)) {
    getTime ();
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
  Serial.println (time_stamp);
}
