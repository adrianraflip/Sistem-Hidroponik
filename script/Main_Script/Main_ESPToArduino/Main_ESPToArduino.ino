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
WidgetTerminal terminal3(V13);
WidgetLCD lcd(V100);
//Variabel Parsing
char inStringAr;
String dataInAr;
String dataAr[8]; // Size Array tergantung banyak kategori data
boolean parsing = false;

//Variabel data to Arduino
String dataSend;

//Variabel Blynk
int minTerminal = 0;
int maxTerminal = 0;
int calTerminal = 0;
int timerTerminal = 0;
int minSend = 0;
int maxSend = 0;
int timerSend = 0;
int calSend = 0;
int calMode = 0;
int lock = 0;
int lock1 = 0;
int lock2 = 0;

float waterTemp = 0; //V0
float airTemp = 0; //V1
float airHum = 0; //V2
int TDS = 0; //V3
int waterState = 0; //V4
String rtc1; //V5
String rtc2; //V5
int maxTds = 0;

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
    terminal2.flush();      
  }
  else{
    terminal2.println("INPUT FAIL");
    terminal2.flush();      
  }
}

BLYNK_WRITE(V13){ //Terminal Timer
  String TerminalString3 = param.asStr();
  char check[10];
  bool digit;
  TerminalString3.toCharArray(check, 10);
  for(int i = 0; i < TerminalString3.length(); i++){
    if(isDigit(check[i])){
      digit = true;
    }
    else{
      digit = false;
      i = TerminalString3.length() + 1;
    }
  }

  if(digit){
    timerTerminal = TerminalString3.toInt();
    terminal3.println("INPUT SUCCESS");
    terminal3.flush();      
  }
  else{
    terminal3.println("INPUT FAIL");
    terminal3.flush();      
  }
}

BLYNK_WRITE(V20){
  int button = param.asInt();
  if(button == 1 && lock == 0){
    if(minTerminal > maxTerminal || maxTerminal < minTerminal || minTerminal < 0 || maxTerminal < 0 || minTerminal == maxTerminal){
      terminal.println("INPUT FAIL");
      terminal.flush();        
      terminal1.println("INPUT FAIL");
      terminal1.flush();        
    }
    else{
      minSend = minTerminal;
      maxSend = maxTerminal; 
    }
    lock = 1;
  }
  if(button == 0){
    lock = 0;
  }
}

BLYNK_WRITE(V21){
  int button1 = param.asInt();
  if(button1 == 1 && lock1 == 0){
    if(calTerminal < 0){
      terminal2.println("INPUT FAIL");
      terminal2.flush();
    }
    else{
      calSend = calTerminal;
      calMode = 1;
    }
    lock1 = 1;
  }
  if(button1 == 0){
    lock1 = 0;
  }
}

BLYNK_WRITE(V22){
  int button2 = param.asInt();
  if(button2 == 1 && lock2 == 0){
    if(timerTerminal < 0){
      terminal3.println("INPUT FAIL");
      terminal3.flush();
    }
    else{
      timerSend = timerTerminal;
    }
    lock2 = 1;
  }
  if(button2 == 0){
    lock2 = 0;
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
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
  dataSend = String(minSend) + "," + String(maxSend) + "," + String(calSend)+ "," + String(calMode) + "," + String(timerSend) + "!";
  if(parsing == true){ // mengolah data dari Arduino
    parsingData();
    Blynk.virtualWrite(V0, waterTemp);
    Blynk.virtualWrite(V1, airTemp);
    Blynk.virtualWrite(V2, airHum);
    Blynk.virtualWrite(V3, TDS);
    if(maxSend == 0){
      maxTds = 1000;
    }
    else{
      maxTds = maxSend;
    }    
    if(TDS > maxTds){
      Blynk.notify("TDS MELEBIHI BATAS");
    }
    if(waterState == 0){
      Blynk.virtualWrite(V4, "AIR KURANG");
    }
    else{
      Blynk.virtualWrite(V4, "AIR TERCUKUPI");
    }
    lcd.print(3, 0, rtc1);
    lcd.print(4, 1, rtc2);
    parsing = false;
    dataInAr="";
    esp.print(dataSend); // mengirim data ke Arduino
    dataSend = "";
    if(calMode == 1){
      calMode = 0;  
    }
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
  for(int z = 0 ; z < 8 ; z++ ){ // Loop disesuaiin size array
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
  waterTemp = dataAr[0].toFloat();
  airTemp = dataAr[1].toFloat();
  airHum = dataAr[2].toFloat();
  TDS = dataAr[3].toInt();
  waterState = dataAr[4].toInt();
  rtc1 = dataAr[5];
  rtc2 = dataAr[6];
}
