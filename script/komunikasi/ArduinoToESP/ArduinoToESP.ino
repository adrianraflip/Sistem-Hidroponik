#include <SoftwareSerial.h>

SoftwareSerial arduino(2, 3); // RX, TX

//Variabel Millis
unsigned long TIME_SCALE = 1000;
unsigned long ROUTINE_TIMER = 1;
unsigned long lastSendDataArduino = 0;

//Variabel Parsing
char inStringEsp;
String dataInEsp;
String dataEsp[4]; // Size Array tergantung banyak kategori data
boolean parsing = false;
int minEsp = 0;
int maxEsp = 0;
int calEsp = 0;

//Variabel data to ESP
String dataSend;
int waterTemp;
int airTemp;
int airHum;
int TDS;
int waterState;
String rtc;

void setup() {
  Serial.begin(9600);
  arduino.begin(9600);
  dataInEsp = "";
  dataSend = "";
}

void loop() {
  waterTemp = random(10,20);
  airTemp = random(100,200);
  airHum = random(1000,2000);
  TDS = random(30,40);
  waterState = 0;
  rtc = "01:1:10";

  dataSend = String(waterTemp) + "," + String(airTemp) + "," + 
             String(airHum) + "," + String(TDS) + "," + 
             String(waterState) + "," + String(rtc) + "#";

  Serial.print("min ESP\t");
  Serial.print("max ESP\t");
  Serial.print("cal ESP\t");
  Serial.print(minEsp);
  Serial.print("\t");
  Serial.print(maxEsp);
  Serial.print("\t");
  Serial.println(calEsp);
  
  if(parsing == true){ // mengolah data dari ESP
    parsingData();
    parsing = false;
    dataInEsp="";   
  }
  else{ // mengirim data ke ESP
    if (millis() - lastSendDataArduino > (ROUTINE_TIMER * TIME_SCALE)) {
      arduino.print(dataSend);
      dataSend = "";
      lastSendDataArduino = millis();
    }    
  }
  
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
  for(int z = 0 ; z < 4 ; z++ ){ // Loop disesuaiin size array
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
}
