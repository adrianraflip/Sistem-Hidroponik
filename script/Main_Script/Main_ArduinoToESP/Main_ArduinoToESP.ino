#include <SoftwareSerial.h>
#include <dhtnew.h>
#include <EEPROM.h>
#include <GravityTDS.h>
#include <RTClib.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define pinWaterFloat 6
#define pinWaterTemperature 4
#define PUMP_A 8
#define PUMP_B 9
#define pinTDS A0
#define pinDHT 5

RTC_DS3231 rtc;
DateTime now_time;
GravityTDS TDS;
DHTNEW DHT21(pinDHT);
SoftwareSerial arduino(2, 3); // RX, TX
SoftwareSerial cal(10, 11); // RX, TX
OneWire ONEWIRE(pinWaterTemperature);
DallasTemperature WTEMP(&ONEWIRE);
//Variabel Millis
unsigned long TIME_SCALE = 1000;
unsigned long ROUTINE_TIMER = 1;
unsigned long lastSendDataArduino = 0;
unsigned long lastMillisPump = 0;
unsigned long timerMillis = 0;
unsigned long timer = 0;

//Variabel Parsing
char inStringEsp;
String dataInEsp;
String dataEsp[7]; // Size Array tergantung banyak kategori data
boolean parsing = false;
int minEsp = 0;
int maxEsp = 0;
int calEsp = 0;
int calMod = 0;
String calPrint;
int minTds = 0;
//int maxTds = 0;

//Variabel data to ESP
String dataSend;
float waterTemp;
float airTemp;
float airHum;
int TDSVal;
int waterState;
char time_stamp[30];
int lockTDS = 0;

void setup() {
  Serial.begin(9600);
  arduino.begin(9600);
  cal.begin(9600);
  TDS.begin();
  TDS.setPin(pinTDS);
  TDS.setAref(5.0);
  TDS.setAdcRange(1024);
  pinMode (PUMP_A, OUTPUT);
  pinMode (PUMP_B, OUTPUT);
  pinMode(pinWaterFloat, INPUT);
  readrtc3231();
  WTEMP.begin();
  dataInEsp = "";
  dataSend = "";
}

void loop() {
//  Serial.print("min ESP\t");
//  Serial.print("max ESP\t");
//  Serial.print("cal Val\t");
//  Serial.print("cal ESP\t");
//  Serial.print("TDS\t");
//  Serial.print(minEsp);
//  Serial.print("\t");
//  Serial.print(maxEsp);
//  Serial.print("\t");
//  Serial.println(calEsp);
//  Serial.print("\t");
//  Serial.println(TDSVal);
  if(minEsp == 0){
    minTds = 300;
  }
  else{
    minTds = minEsp;
  }

  if(TDSVal < minTds && lockTDS == 0){
    lockTDS = 1;
    if(timer == 0){
      timer = 3;
    }
    timerMillis = timer;
  }
  
  if(lockTDS == 1){
    if (millis() - lastMillisPump > 1000){
      digitalWrite (PUMP_A, HIGH);
      digitalWrite (PUMP_B, HIGH);
      timerMillis--; 
      lastMillisPump = millis();       
    }
    if(timerMillis == 0){
      lockTDS = 0;
    }    
  }
  else{
    digitalWrite (PUMP_A, LOW);
    digitalWrite (PUMP_B, LOW);
    lastMillisPump = millis();  
  }
  
  if(parsing == true){ // mengolah data dari ESP
    parsingData();
    parsing = false;
    dataInEsp="";
    Serial.println(calMod);
    if(calMod == 1){
      cal.listen();
      cal.println("enter");
      Serial.print("===============enter==============");
      calPrint = "cal:" + String(calEsp);
      cal.println(calPrint);
      cal.println("exit");  
      calMod = 0;
    }        
  }
  
  else{ // mengirim data ke ESP
    if (millis() - lastSendDataArduino > (ROUTINE_TIMER * TIME_SCALE)) {    
      updateDHTVal();
      updateTDSVal();    
      getTime();
      updateWaterTempVal();
      updateWaterFloat();
      dataSend = String(waterTemp) + "," + String(airTemp) + "," + 
           String(airHum) + "," + String(TDSVal) + "," + 
           String(waterState) + "," + String(time_stamp) + "#";
           
      arduino.print(dataSend);
      dataSend = "";
      lastSendDataArduino = millis();
    }    
  }
  arduino.listen();
  if(arduino.available()>0){ // Ada data dari ESP
    inStringEsp = (char)arduino.read();
    dataInEsp += inStringEsp;
    if(inStringEsp == '!'){ // Data pengakhir dari ESP
      parsing = true;
    }
  } 
}

void parsingData(){
  int j = 0;
  for(int z = 0 ; z < 7 ; z++ ){ // Loop disesuaiin size array
    dataEsp[z]="";
  }
  for(int i = 0 ; i < dataInEsp.length() ; i++){
    if ((dataInEsp[i] == '!') || (dataInEsp[i] == ',')){
      j++;
      dataEsp[j]="";
    }
    else{
      dataEsp[j] = dataEsp[j] + dataInEsp[i];
    }
  }
  minEsp = dataEsp[0].toInt();
  maxEsp = dataEsp[1].toInt();
  calEsp = dataEsp[2].toInt();
  calMod = dataEsp[3].toInt();
  timer = dataEsp[4].toInt();
}

void updateDHTVal() {
  DHT21.read();
  airHum = DHT21.getHumidity();
  airTemp = DHT21.getTemperature();

//  Serial.print ("Relatif Humidity (RH) = ");
//  Serial.print  (airHum);
//  Serial.print  (" %");
//  Serial.print  ("\t");
//  
//  Serial.print (", Air Temperature = ");
//  Serial.print  (airTemp);
//  Serial.println  (" *C");
}

void updateTDSVal() {
  TDS.setTemperature(waterTemp);
  TDS.update();
  TDSVal = TDS.getTdsValue();
//
//  Serial.print    (" TDS = ");
//  Serial.println  (TDSVal);
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
  sprintf(time_stamp, "%02d-%02d-%02d,%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
//  Serial.println (time_stamp);
}

void updateWaterTempVal() {
  WTEMP.requestTemperatures();
  waterTemp = WTEMP.getTempCByIndex(0);
//  Serial.print ("Water Temperature = ");
//  Serial.print (waterTemp, 1);
//  Serial.println (" *C");
}

void updateWaterFloat(){
  if (digitalRead(pinWaterFloat) == HIGH) {
    waterState = 0; //("AIR KURANG");
  }
  else {
    waterState = 1; //("AIR TERCUKUPI");
  }  
}
