#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUZZER_PIN 8

bool playBirthdaySong = false;

int melody[] = {
  264,264,297,264,352,330,
  264,264,297,264,396,352,
  264,264,528,440,352,330,297,
  466,466,440,352,396,352
};

int durations[] = {
  250,250,500,500,500,1000,
  250,250,500,500,500,1000,
  250,250,500,500,500,500,1000,
  250,250,500,500,500,1200
};

const int TOTAL_NOTES =
  sizeof(melody) / sizeof(melody[0]);

int currentNote = 0;
unsigned long noteStartTime = 0;


// --------------------
// Balloons
// --------------------
struct Balloon {
  float x;
  float y;
  float dx;
  float dy;
};

#define NUM_BALLOONS 0
Balloon balloons[NUM_BALLOONS];

// --------------------
// Balloon Functions
// --------------------
void drawBalloon(int x, int y) {

  display.drawCircle(x, y, 3, SSD1306_WHITE);
  display.fillCircle(x, y, 2, SSD1306_WHITE);

  display.drawLine(x, y + 3, x, y + 8, SSD1306_WHITE);
}

void updateBalloons() {

  for (int i = 0; i < NUM_BALLOONS; i++) {

    balloons[i].x += balloons[i].dx;
    balloons[i].y += balloons[i].dy;

    // Tiny random drift
    if (random(100) < 3) {
      balloons[i].dx += random(-10, 11) / 50.0;
      balloons[i].dy += random(-10, 11) / 50.0;

      balloons[i].dx = constrain(balloons[i].dx, -1.2, 1.2);
      balloons[i].dy = constrain(balloons[i].dy, -1.2, 1.2);
    }

    // Screen borders
    if (balloons[i].x < 5) {
      balloons[i].x = 5;
      balloons[i].dx *= -1;
    }

    if (balloons[i].x > SCREEN_WIDTH - 5) {
      balloons[i].x = SCREEN_WIDTH - 5;
      balloons[i].dx *= -1;
    }

    if (balloons[i].y < 5) {
      balloons[i].y = 5;
      balloons[i].dy *= -1;
    }

    if (balloons[i].y > SCREEN_HEIGHT - 5) {
      balloons[i].y = SCREEN_HEIGHT - 5;
      balloons[i].dy *= -1;
    }

    // Protected text zone
    if (balloons[i].x > 15 &&
        balloons[i].x < 113 &&
        balloons[i].y > 5 &&
        balloons[i].y < 58) {

      balloons[i].dx *= -1;
      balloons[i].dy *= -1;

      balloons[i].x += balloons[i].dx * 4;
      balloons[i].y += balloons[i].dy * 4;
    }

    drawBalloon(
      (int)balloons[i].x,
      (int)balloons[i].y
    );
  }
}

void updateBirthdaySong() {

  if (!playBirthdaySong)
    return;

  if (currentNote >= TOTAL_NOTES) {

    noTone(BUZZER_PIN);
    playBirthdaySong = false;
    return;
  }

  unsigned long now = millis();

  if (noteStartTime == 0 ||
      now - noteStartTime >
      durations[currentNote] * 1.3) {

    tone(
      BUZZER_PIN,
      melody[currentNote]
    );

    noteStartTime = now;

    currentNote++;
  }
}

// --------------------
// Countdown
// --------------------
void countdown() {

  unsigned long startTime = millis();
  const unsigned long duration = 3000;
  static unsigned long lastTick = 0;

  if (millis() - lastTick > 200) {
    tone(BUZZER_PIN, 1800, 40);
    lastTick = millis();
  }

  while (millis() - startTime < duration) {

    unsigned long elapsed = millis() - startTime;
    unsigned long remaining = duration - elapsed;

    int seconds = remaining / 1000;
    int milliseconds = remaining % 1000;

    int microseconds =
      9999 - ((micros() / 10) % 10000);

    display.clearDisplay();

    updateBalloons();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);

    char buf[20];

    sprintf(
      buf,
      "%02d:%03d:%02d",
      seconds,
      milliseconds,
      microseconds/100
    );

    display.setCursor(10, 28);
    display.print(buf);

    display.display();

    delay(20);
 
  }
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(35, 18);
    display.print("00");
    display.display();

    delay(250);
}

void fireworkBurst() {

  int cx = SCREEN_WIDTH / 2;
  int cy = SCREEN_HEIGHT / 2;

  // Expanding explosion
  for (int r = 2; r <= 34; r += 2) {

    display.clearDisplay();

    display.drawLine(cx, cy, cx + r, cy, SSD1306_WHITE);
    display.drawLine(cx, cy, cx - r, cy, SSD1306_WHITE);

    display.drawLine(cx, cy, cx, cy + r, SSD1306_WHITE);
    display.drawLine(cx, cy, cx, cy - r, SSD1306_WHITE);

    display.drawLine(cx, cy, cx + r, cy + r, SSD1306_WHITE);
    display.drawLine(cx, cy, cx - r, cy - r, SSD1306_WHITE);

    display.drawLine(cx, cy, cx + r, cy - r, SSD1306_WHITE);
    display.drawLine(cx, cy, cx - r, cy + r, SSD1306_WHITE);

    display.display();

    delay(25);
  }

  // Sparkle phase
  for (int frame = 0; frame < 12; frame++) {

    display.clearDisplay();

    for (int i = 0; i < 40; i++) {

      display.drawPixel(
        random(0, SCREEN_WIDTH),
        random(0, SCREEN_HEIGHT),
        SSD1306_WHITE
      );
    }

    display.display();

    delay(40);
  }

  display.clearDisplay();
  display.display();
}
// --------------------
// Intro Animation
// --------------------
void birthdayIntro() {

  int happyX = -40;
  int birthdayX = SCREEN_WIDTH;

  while (happyX < 40 || birthdayX > 20) {

    display.clearDisplay();

    updateBalloons();

    if (happyX < 40)
      happyX += 2;

    if (birthdayX > 20)
      birthdayX -= 2;

    display.setTextSize(2);

    display.setCursor(happyX, 8);
    display.print("HAPPY");

    display.setCursor(birthdayX, 22);
    display.print("BIRTHDAY");

    display.display();

    delay(20);
  }

  // TO YOU drop animation

  for (int y = -10; y < 42; y += 2) {

    display.clearDisplay();

    updateBalloons();

    display.setTextSize(2);

    display.setCursor(40, 8);
    display.print("HAPPY");

    display.setCursor(20, 24);
    display.print("BIRTHDAY");

    display.setCursor(35, y);
    display.print("TO YOU!");

    display.display();

    delay(25);
  }
}

// --------------------
// Final Screen
// --------------------
void showBirthdayScreen() {

  display.clearDisplay();

  updateBalloons();

  display.setTextSize(2);

  display.setCursor(40, 8);
  display.print("HAPPY");

  display.setCursor(20, 24);
  display.print("BIRTHDAY");

  static int frame = 0;

  int bounce = sin(frame * 0.08) * 3;

  display.setCursor(35, 42 + bounce);
  display.print("TO YOU!");

  frame++;

  display.display();
}

// --------------------
// Setup
// --------------------
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  if (!display.begin(
          SSD1306_SWITCHCAPVCC,
          0x3C)) {
    while (true);
  }

  randomSeed(analogRead(A0));

  for (int i = 0; i < NUM_BALLOONS; i++) {

  // Spawn mostly near edges

  if (i % 2 == 0) {
    balloons[i].x = random(5, 20);
  } else {
    balloons[i].x = random(108, 123);
  }

  balloons[i].y = random(5, 59);

  balloons[i].dx = random(-10, 11) / 10.0;
  balloons[i].dy = random(-10, 11) / 10.0;

  if (abs(balloons[i].dx) < 0.3)
    balloons[i].dx = 0.5;

  if (abs(balloons[i].dy) < 0.3)
    balloons[i].dy = 0.5;
}

countdown();
fireworkBurst();
birthdayIntro();

playBirthdaySong = true;
currentNote = 0;
noteStartTime = 0;

}

// --------------------
// Loop
// --------------------
void loop() {

  showBirthdayScreen();
  updateBirthdaySong();

  delay(30);
}