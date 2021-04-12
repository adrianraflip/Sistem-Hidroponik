/*
ARDUINO UNO 
------------------------
MOTOR A MIX = 8  => LED 
MOTOR B MIX = 9  => LED
WATER FLOAT = 6
DHT 21 = 5
DS18b20 = 4 
TX = 3
TX = 2
AHV = 10
BHV = 11
A0 = TDS SENSOR
A1 = PH SENSOR
A4 = SDA
A5 = SCL 

-----------------------------
ESP8266 
-------------------------------
LED WIFI = D7 GPIO13
BLV = D6
ALV = D5
RXLV = D2 / 4
TXLV = D1 /5
---------------------------------------
led power (1  MERAH)
led wifi (1 hijau)
led ab mix ( 2 kuning )
----------------------------------
D8/gpio15
D4/gpio2
D3/ gpio0
D0/ gpio16
d5 / 14
d6/12
d7/13

---------------------------------------------
ALTERNATIF  PROGRAM KOMUNIKASI BERHASIL
--------------------------------------------
UNO | ESP
2       |  RX
3       | TX

*/


#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
BlynkTimer timer;
SoftwareSerial remot(4, 5); // RX, TX (GPIO1 , GPIO3)

char auth[] = "xG15115-fjT-1rpi9-wMLvLyCOsrmbO5"; // PASTE YOUR AUTHENTICATION CODE HERE
char ssid[] = "www.tunasfarm.id"; //WIFI NAME
char pass[] = "Tunas@123987-"; //PASSWORD
int i ;
String dataIn;
String dt[50];
boolean parsing = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  remot.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, parsingData);
}


void loop() {
  Blynk.run();
  timer.run();
  if (remot.available() > 0)
  {
    char inChar = (char)remot.read();
    dataIn += inChar;
    if (inChar == '\n') {
      parsing = true;
    }

  }
  if (parsing)
  {
    parsingData();
    parsing = false;
    dataIn = "";
  }
}

void parsingData()
{
  int j = 0;
  //kirim data yang telah diterima sebelumnya

  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //Serial.print(dataIn);
  //proses parsing data
  for (i = 1; i < dataIn.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '&') || (dataIn[i] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[i];
    }

  }
  //Serial.println (dt[0]) ;
  //Serial.println (dt[1]) ;
  //Serial.print (dt[2]) ;
  //delay(30000);
  //Serial.println("************");
  String data1 = "&" + dt[0] + "," + dt[1] + "," + dt[2] + dt[3] + "," + dt[4] + "," + dt[5];
  Serial.print("KIRIM DATA: ");
  //client.print(dt[0]); // kirim data
  //client.print(dt[1]);
  //client.print(dt[2]);
  Serial.print(data1);
  Blynk.virtualWrite(V1, dt[0]);
  Blynk.virtualWrite(V2, dt[1]);
  Blynk.virtualWrite(V3, dt[2]);
  Blynk.virtualWrite(V4, dt[3]);

}
