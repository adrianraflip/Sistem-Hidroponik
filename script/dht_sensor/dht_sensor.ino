#include           <dhtnew.h>
#define            pinDHT 5
DHTNEW             DHT21(pinDHT);

unsigned long lastRoutineTime = 0;
unsigned long MAIN_ROUTINE_TIMER = 1;
unsigned long TIME_SCALE         = 1000;
unsigned long MINUTE_TO_SECOND   = 60;

float   humidityVal;
float   airTempVal;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

}

void loop() {
  if ((millis() - lastRoutineTime) > (MAIN_ROUTINE_TIMER * TIME_SCALE  /* * MINUTE_TO_SECOND */ )) {
    updateDHTVal();
    lastRoutineTime = millis();
  }
}

void updateDHTVal() {
  DHT21.read();
  humidityVal = DHT21.getHumidity();
  airTempVal = DHT21.getTemperature();

  Serial.print ("Relatif Humidity (RH) = ");
  Serial.print  (humidityVal);
  Serial.print  (" %");
  Serial.print  ("\t");
  
  Serial.print (", Air Temperature = ");
  Serial.print  (airTempVal);
  Serial.println  (" *C");
}
