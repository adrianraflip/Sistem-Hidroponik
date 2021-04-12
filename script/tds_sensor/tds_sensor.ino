/*
 *  1. CTRL+SHIFT+M
 *  2. Calibration TDS SENSOR:
 *  enter -> enter the calibration mode - ditulis enter
 *  cal:tds value -> calibrate with the known tds value(25^c). e.g. "cal:707" contoh
 *  exit -> save the parameters and exit the calibration mode - ditulis exit
 */

#include      <EEPROM.h>
#include      "GravityTDS.h";
#define       pinTDS A0
GravityTDS    TDS;

unsigned long lastRoutineTime = 0;
unsigned long MAIN_ROUTINE_TIMER = 1;
unsigned long TIME_SCALE         = 1000;
unsigned long MINUTE_TO_SECOND   = 60;

float waterTempVal = 25 ;
int     TDSVal;

void setup() {
  Serial.begin(115200);
  TDS.begin();
  TDS.setPin(pinTDS);
  TDS.setAref(5.0);
  TDS.setAdcRange(1024);
}

void loop() {
  if ((millis() - lastRoutineTime) > (MAIN_ROUTINE_TIMER * TIME_SCALE  /* * MINUTE_TO_SECOND */ )) {
    updateTDSVal();
    lastRoutineTime = millis();
  }
}

void updateTDSVal() {
  TDS.setTemperature(waterTempVal);
  TDS.update();
  TDSVal = TDS.getTdsValue();

  Serial.print    (" TDS = ");
  Serial.println  (TDSVal);
}
