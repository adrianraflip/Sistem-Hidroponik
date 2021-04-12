#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial esp(4, 5); // RX, TX

//Blynk Variable
char auth[] = "H06HTlhcHM02jXcvu26x14WaWWtvPmdJ";
char ssid[] = "BigPond082B";
char pass[] = "5064935146";
WidgetTerminal terminal(V10);
WidgetTerminal terminal1(V11);
WidgetTerminal terminal2(V12);
WidgetLCD lcd(V100);
//Variabel Parsing
char inStringAr;
String dataInAr;
String dataAr[6]; // Size Array tergantung banyak kategori data
boolean parsing = false;

//Variabel data to Arduino
String dataSend;

//Variabel Blynk
int minTerminal = 0;
int maxTerminal = 0;
int calTerminal = 0;
int minSend = 0;
int maxSend = 0;
int calSend = 0;
int lock = 0;

int waterTemp = 0; //V0
int airTemp = 0; //V1
int airHum = 0; //V2
int TDS = 0; //V3
int waterState = 0; //V4
String rtc; //V5

BLYNK_WRITE(V10){ //Terminal Minimum
  String TerminalString0 = param.asStr();
  char check[10];
  bool digit;
  TerminalString0.toCharArray(check, 10);
  for(int i = 0; i < TerminalString0.length(); i++){
    if(isDigit(check[i])){
      digit = true;
    }
    else{
      digit = false;
      i = TerminalString0.length() + 1;
    }
  }

  if(digit){
    minTerminal = TerminalString0.toInt();
    terminal.println("INPUT SUCCESS");
    terminal.flush();      
  }
  else{
    terminal.println("INPUT FAIL");
    terminal.flush();      
  }
}

BLYNK_WRITE(V11){ //Terminal Maximum
  String TerminalString1 = param.asStr();
  char check[10];
  bool digit;
  TerminalString1.toCharArray(check, 10);
  for(int i = 0; i < TerminalString1.length(); i++){
    if(isDigit(check[i])){
      digit = true;
    }
    else{
      digit = false;
      i = TerminalString1.length() + 1;
    }
  }

  if(digit){
    maxTerminal = TerminalString1.toInt();
    terminal1.println("INPUT SUCCESS");
    terminal1.flush();      
  }
  else{
    terminal1.println("INPUT FAIL");
    terminal1.flush();      
  }
}

BLYNK_WRITE(V12){ //Terminal Kalibrasi
  String TerminalString2 = param.asStr();
  char check[10];
  bool digit;
  TerminalString2.toCharArray(check, 10);
  for(int i = 0; i < TerminalString2.length(); i++){
    if(isDigit(check[i])){
      digit = true;
    }
    else{
      digit = false;
      i = TerminalString2.length() + 1;
    }
  }

  if(digit){
    calTerminal = TerminalString2.toInt();
    terminal2.println("INPUT SUCCESS");
    terminal1.flush();      
  }
  else{
    terminal2.println("INPUT FAIL");
    terminal2.flush();      
  }
}

BLYNK_WRITE(V20){
  int button = param.asInt();
  if(button == 1 && lock == 0){
    if(minTerminal > maxTerminal || maxTerminal < minTerminal || minTerminal < 0 || maxTerminal < 0 || calTerminal < 0){
      terminal.println("INPUT FAIL");
      terminal.flush();        
      terminal1.println("INPUT FAIL");
      terminal1.flush();        
      terminal2.println("INPUT FAIL");
      terminal2.flush();
    }
    else{
      minSend = minTerminal;
      maxSend = maxTerminal;
      calSend = calTerminal; 
    }
    lock = 1;
  }
  if(button == 0){
    lock = 0;
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,0,4), 8080);
  esp.begin(9600);
  dataInAr = "";
  dataSend = "";
  lcd.clear(); 
  lcd.print(4, 0, "Hello");
  lcd.print(4, 1, "World");
  terminal.clear();
  terminal1.clear();
  terminal2.clear();
  delay(3000);
  lcd.clear();
}

void loop() {
  dataSend = String(minSend) + "," + String(maxSend) + "!";
  if(parsing == true){ // mengolah data dari Arduino
    parsingData();
    Blynk.virtualWrite(V0, waterTemp);
    Blynk.virtualWrite(V1, airTemp);
    Blynk.virtualWrite(V2, airHum);
    Blynk.virtualWrite(V3, TDS);
    if(waterState == 0){
      Blynk.virtualWrite(V4, "AIR KURANG");
    }
    else{
      Blynk.virtualWrite(V4, "AIR TERCUKUPI");
    }
    lcd.print(4, 0, rtc);
    parsing = false;
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
  Blynk.run();   
}

void parsingData(){
  int j = 0;
  for(int z = 0 ; z < 6 ; z++ ){ // Loop disesuaiin size array
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
  waterTemp = dataAr[0].toInt();
  airTemp = dataAr[1].toInt();
  airHum = dataAr[2].toInt();
  TDS = dataAr[3].toInt();
  waterState = dataAr[4].toInt();
  rtc = dataAr[5];
}
