#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <avr/wdt.h>

#define RS 11
#define EN 12
#define D4 2
#define D5 3
#define D6 4
#define D7 5
#define LASER A5
#define LAMP 9
#define SENSOR A0

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
time_t timer;
char time_str[8];
int pages;
int laser_base;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  byte smiley_face[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b10001,
    0b01110,
    0b00000
  };

  byte hourglass[8] = {
    0b11111,
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    0b11111,
    0b00000
  };

  byte eye[8] = {
    0b00000,
    0b00110,
    0b01001,
    0b01001,
    0b01111,
    0b01101,
    0b00110,
    0b00000
  };

  lcd.createChar(0, smiley_face);
  lcd.createChar(1, hourglass);
  lcd.createChar(2, eye);

  lcd.setCursor(2, 0);
  lcd.print("My   Reading");
  lcd.setCursor(5, 0);
  lcd.write((byte)0);
  lcd.setCursor(4, 1);
  lcd.print("Assistant");

  delay(5000);
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.write((byte)2);
  lcd.setCursor(2, 0);
  lcd.write((byte)2);

  lcd.setCursor(15, 0);
  lcd.write((byte)1);

  lcd.setCursor(0, 1);
  lcd.print("Bookmark:");

  lcd.setCursor(15, 1);
  lcd.write((byte)0);

  laser_base = analogRead(LASER);
  pages = EEPROM.get(4, pages);

  setTime(timer);
}

void loop() {
  timer = now();
  sprintf(time_str, "%.1d:%.2d:%.2d", hour(timer), minute(timer), second(timer));
  lcd.setCursor(7, 0);
  lcd.print(time_str);

  lcd.setCursor(10, 1);
  lcd.print(pages);

  int laser_value = analogRead(LASER);

  if (laser_value > laser_base + 80) {
    laser_base = laser_value;
  } else if (laser_value > laser_base + 30 || laser_value < laser_base - 30) {
    pages += 2;
    delay(500);
  }

  int sensor_value = analogRead(SENSOR);
  if (sensor_value > 950) {
    analogWrite(LAMP, map(sensor_value, 950, 1023, 50, 255));
  }
  else {
    analogWrite(LAMP, 0);
  }
  Serial.println(laser_value);
}

void exit_screen() {
  if (pages) {
    lcd.clear();
    timer = now();
    timer /= pages;
    if (hour(now())) {
      sprintf(time_str, "%.1dh%.2dm%.2ds", hour(timer), minute(timer), second(timer));
    }
    else if (minute(now())) {
      sprintf(time_str, "%.2dmin %.2dsec", minute(timer), second(timer));
    }
    else {
      sprintf(time_str, "%.2dsec", minute(timer), second(timer));
    }
    lcd.setCursor(7, 0);
    lcd.print(time_str);
    delay(2000);
  }
  lcd.clear();
  while (analogRead(LASER) < 980);
  wdt_enable(WDTO_1S);
}
