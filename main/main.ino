#define SENSOR A0
#define LED 9

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(SENSOR));
  analogWrite(LED, 1024 - analogRead(SENSOR));
}
