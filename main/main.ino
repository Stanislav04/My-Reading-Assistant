#include <LiquidCrystal.h> 

//#define SENSOR A0
//#define LED 9

#define RS 11
#define EN 12
#define D4 2
#define D5 3
#define D6 4
#define D7 5


LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  analogWrite(9, 600);
  Serial.begin(9600);

  lcd.begin(16, 2);

  byte customChar[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000
};

  lcd.createChar(0, customChar);

  lcd.setCursor(0, 0);
  lcd.print("Hey hey!");

  

  lcd.setCursor(5, 0);
  lcd.write((byte)0);
}

void loop() {
  /*int sensor_value = analogRead(SENSOR);
  Serial.println(sensor_value);*/
  
  /*if (sensor_value > 500) {
    analogWrite(LED, sensor_value);
  }
  else {
    analogWrite(LED, 0);
  }*/

}
