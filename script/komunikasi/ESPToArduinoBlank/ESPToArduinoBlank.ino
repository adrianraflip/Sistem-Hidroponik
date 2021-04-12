#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial esp(4, 5); // RX, TX

//Variabel Parsing
char inStringAr;
String dataInAr;
String dataAr[4]; // Size Array tergantung banyak kategori data
boolean parsing = false;

//Variabel data to Arduino
String dataSend;

int dataWrite1 = 0;
int dataWrite2 = 0;
int dataWrite3 = 0;

void setup() {
  Serial.begin(9600);
  esp.begin(9600);
  dataInAr = "";
  dataSend = "";
}

void loop() {
  dataSend = String("123") + "," + String("456") + "!";
  if(parsing == true){ // mengolah data dari Arduino
    parsingData();
    parsing = false;
    Serial.println(dataWrite1);
    Serial.println(dataWrite2);
    Serial.println(dataWrite3);
    dataInAr="";
    esp.print(dataSend); // mengirim data ke Arduino
    dataSend = "";   
  }  
  if(esp.available()>0){ // Ada data dari ESP
    inStringAr = (char)esp.read();
    dataInAr += inStringAr;
    if(inStringAr == '#'){ // Data pengakhir dari ESP
      parsing = true;
    }
  }
}

void parsingData(){
  int j = 0;
  for(int z = 0 ; z < 4 ; z++ ){ // Loop disesuaiin size array
    dataAr[z]="";
  }
  for(int i = 0 ; i < dataInAr.length() ; i++){
    if ((dataInAr[i] == '#') || (dataInAr[i] == ',')){
      j++;
      dataAr[j]="";
    }
    else{
        dataAr[j] = dataAr[j] + dataInAr[i];
    }
  }
  dataWrite1 = dataAr[0].toInt();
  dataWrite2 = dataAr[1].toInt();
  dataWrite3 = dataAr[2].toInt();
}
