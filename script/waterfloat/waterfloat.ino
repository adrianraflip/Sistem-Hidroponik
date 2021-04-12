#define pinWaterFloat 6

void setup() {
  Serial.begin(9600);
  Serial.println("Start reading..");
  pinMode(pinWaterFloat, INPUT);
}

void loop() {
  if (digitalRead(pinWaterFloat) == HIGH) {
    Serial.println("AIR KURANG");
  } else {
    Serial.println("AIR TERCUKUPI");
  }
  delay (1000);
}
