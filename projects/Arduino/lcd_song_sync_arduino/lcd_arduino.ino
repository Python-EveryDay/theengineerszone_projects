#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String receivedText = "";

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.print("Waiting...");
}

void loop() {

  if (Serial.available()) {

  String msg = Serial.readStringUntil('\n');

  int separator = msg.indexOf('|');

  String line1;
  String line2;

  if(separator >= 0) {

    line1 = msg.substring(0, separator);
    line2 = msg.substring(separator + 1);

  } else {

    line1 = msg;
    line2 = "";
  }

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(line1);

  lcd.setCursor(0,1);
  lcd.print(line2);
  }
}