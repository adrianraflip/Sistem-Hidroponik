#include <DallasTemperature.h>
#include <OneWire.h>
#define            pinWaterTemperature 4
OneWire            ONEWIRE(pinWaterTemperature);
DallasTemperature  WTEMP(&ONEWIRE);

unsigned long lastRoutineTime = 0;
unsigned long MAIN_ROUTINE_TIMER = 1;
unsigned long TIME_SCALE         = 1000;
unsigned long MINUTE_TO_SECOND   = 60;
float   waterTempVal;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  WTEMP.begin();

}

void loop() {
  if ((millis() - lastRoutineTime) > (MAIN_ROUTINE_TIMER * TIME_SCALE  /* * MINUTE_TO_SECOND */ )) {
    updateWaterTempVal();
    lastRoutineTime = millis();
  }
}

void updateWaterTempVal() {
  WTEMP.requestTemperatures();
  waterTempVal = WTEMP.getTempCByIndex(0);
  Serial.print ("Water Temperature = ");
  Serial.print (waterTempVal, 1);
  Serial.println (" *C");
}
