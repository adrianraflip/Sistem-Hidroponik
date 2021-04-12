#include <SoftwareSerial.h>

SoftwareSerial UART(2, 3); // RX, TX
#include "RTClib.h"
RTC_DS3231 rtc;
DateTime now_time ;

String water_temperature, air_temperature, humidity, tds, water_tank_status;

unsigned long TIME_SCALE = 1000;
unsigned long ROUTINE_TIMER = 1;
unsigned long routineTimepoint = 0;

char time_stamp[30];


void setup() {
  Serial.begin (115200) ;
  kl.begin (9600) ;
  while (!Serial) {
    ;
  }
  readrtc3231 ();
}

void loop() {

  if (millis() - routineTimepoint > (ROUTINE_TIMER * TIME_SCALE)) {
    getTime ();
    sendSensorData ();
    routineTimepoint = millis();
  }


  water_temperature = random(20, 25); ;
  air_temperature = random(20, 25); ;
  humidity = randomrandom(50, 80);
  tds= random(600, 1000);
  //  Serial.print ("sensor 1 =") ;
  //  Serial.println (a) ;
  //  Serial.print ("sensor 2 =") ;
  //  Serial.println (b) ;
  //  Serial.print ("sensor 3 =") ;
  //  Serial.println (c) ;
  String datakirim = "&" + String(a) + "," + String(b) + "," + String(c) + "&" ;
  Serial.println (datakirim) ;
  kl.print(",");
  kl.print(a);
  kl.print(",");
  kl.print(b);
  kl.print(",");
  kl.println(c);
  delay (10000) ;

}

void getTime () {
  DateTime now = rtc.now();
  now_time = rtc.now();
  sprintf(time_stamp, "%02d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  //  Serial.println (time_stamp);
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
