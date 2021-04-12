#define PUMP_A 8
#define PUMP_B 9

void setup() {
  Serial.begin (9600);
  pinMode (PUMP_A, OUTPUT);
  pinMode (PUMP_B, OUTPUT);
}

void loop() {
  digitalWrite (PUMP_A, HIGH);
  digitalWrite (PUMP_B, HIGH);
  Serial.println ("HIGH");
  delay (1000);

  digitalWrite (PUMP_A, LOW);
  digitalWrite (PUMP_B, LOW);
  Serial.println ("LOW");
  delay (2000);



}
