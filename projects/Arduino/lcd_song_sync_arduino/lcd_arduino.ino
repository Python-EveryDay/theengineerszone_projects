#include <LiquidCrystal.h>

// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(1, 2, 3, 4, 5, 6);

// Custom Heart Character
byte heart[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);

  // Create heart character at slot 0
  lcd.createChar(0, heart);

  // Seed random generator
  randomSeed(analogRead(A0));

  lcd.clear();
  lcd.print("Ready...");
}

void loop() {

  if (Serial.available()) {

    String msg = Serial.readStringUntil('\n');

    int separator = msg.indexOf('|');

    String word;

    if (separator >= 0) {
      word = msg.substring(0, separator);
    } else {
      word = msg;
    }

    word.trim();

    if (word.length() == 0)
      return;

    lcd.clear();

    // ---------- Random Style ----------
    int style = random(0, 8);

    String displayWord;

    switch (style) {

      case 0:
        displayWord = "<" + word + ">";
        break;

      case 1:
        displayWord = "[" + word + "]";
        break;

      case 2:
        displayWord = "~" + word + "~";
        break;

      case 3:
        displayWord = "*" + word + "*";
        break;

      case 4:
        displayWord = "=" + word + "=";
        break;

      case 5:
        displayWord = "+" + word + "+";
        break;

      default:
        displayWord = word;
        break;
    }

    // ---------- Random Position ----------
    int row = random(0, 2);

    int maxCol = 16 - displayWord.length();

    if (maxCol < 0)
      maxCol = 0;

    int col = random(0, maxCol + 1);

    // ---------- Heart Mode ----------
    bool showHeart = (random(0, 100) < 25); // 25% chance

    if (showHeart) {

      int heartWidth = displayWord.length() + 2;

      maxCol = 16 - heartWidth;

      if (maxCol < 0)
        maxCol = 0;

      col = random(0, maxCol + 1);

      lcd.setCursor(col, row);
      lcd.write(byte(0));

      lcd.print(displayWord);

      lcd.write(byte(0));
    }
    else {

      lcd.setCursor(col, row);
      lcd.print(displayWord);
    }

    // Small flash effect
    delay(20);
  }
}