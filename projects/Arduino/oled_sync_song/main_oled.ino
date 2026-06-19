#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ======================================
// Serial Lyrics
// ======================================
String currentWord = "";

// ======================================
// Zoom Animation
// ======================================
int zoomSize = 3;
unsigned long zoomStart = 0;
bool zoomAnimating = false;

// ======================================
// Side Wave Animation
// ======================================
int waveOffset = 0;
int waveDir = 1;

// ======================================
// Equalizer Animation
// ======================================
int eqFrame = 0;

// ======================================
// Center Ripple Animation
// ======================================
bool pulseActive = false;
int pulseRadius = 0;

// ======================================
// Read Serial
// ======================================
void readSerialWord()
{
  while (Serial.available())
  {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();

    int separator = incoming.indexOf('|');

    if (separator >= 0)
    {
      String newWord = incoming.substring(0, separator);

      if (newWord.length() > 0)
      {
        currentWord = newWord;

        // Start ripple
        pulseActive = true;
        pulseRadius = 0;

        // Start zoom animation
        zoomSize = 1;
        zoomAnimating = true;
        zoomStart = millis();

        Serial.println(currentWord);
      }
    }
  }
}

// ======================================
// Radar Waves
// ======================================
void drawRadarWaves()
{
  int cy = 30;

  int r1 = 18 + waveOffset;
  int r2 = 26 + waveOffset;
  int r3 = 34 + waveOffset;

  display.drawCircleHelper(8, cy, r1, 1, WHITE);
  display.drawCircleHelper(8, cy, r2, 1, WHITE);
  display.drawCircleHelper(8, cy, r3, 1, WHITE);

  display.drawCircleHelper(120, cy, r1, 2, WHITE);
  display.drawCircleHelper(120, cy, r2, 2, WHITE);
  display.drawCircleHelper(120, cy, r3, 2, WHITE);

  waveOffset += waveDir;

  if (waveOffset >= 3 || waveOffset <= 0)
    waveDir *= -1;
}

// ======================================
// Center Ripple
// ======================================
void drawPulse()
{
  if (!pulseActive)
    return;

  int cx = SCREEN_WIDTH / 2;
  int cy = SCREEN_HEIGHT / 2 - 2;

  display.drawCircle(cx, cy, pulseRadius, WHITE);

  if (pulseRadius > 8)
    display.drawCircle(cx, cy, pulseRadius - 8, WHITE);

  if (pulseRadius > 16)
    display.drawCircle(cx, cy, pulseRadius - 16, WHITE);

  pulseRadius += 2;

  if (pulseRadius > 40)
  {
    pulseRadius = 0;
    pulseActive = false;
  }
}

// ======================================
// Equalizer
// ======================================
void drawEqualizer()
{
  int startX = 14;
  int baseY = 63;
  int width = 6;
  int gap = 3;

  for (int i = 0; i < 12; i++)
  {
    float val = sin((i * 0.6) + (eqFrame * 0.25));

    int h = 4 + abs(val) * 14;

    display.fillRect(
      startX + i * (width + gap),
      baseY - h,
      width,
      h,
      WHITE
    );
  }

  eqFrame++;
}

// ======================================
// Center Text
// ======================================
void drawCenteredWord(String word, int size)
{
  display.setTextSize(size);
  display.setTextColor(WHITE);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(word, 0, 0, &x1, &y1, &w, &h);

  int x = (SCREEN_WIDTH - w) / 2;
  int y = (SCREEN_HEIGHT - h) / 2 - 4;

  display.setCursor(x, y);
  display.print(word);
}

// ======================================
// Zoom Animation Update
// ======================================
void updateZoomAnimation()
{
  if (!zoomAnimating)
    return;

  unsigned long elapsed = millis() - zoomStart;

  if (elapsed < 60)
    zoomSize = 1;
  else if (elapsed < 120)
    zoomSize = 2;
  else
  {
    zoomSize = 3;
    zoomAnimating = false;
  }
}

// ======================================
// Setup
// ======================================
void setup()
{
  Serial.begin(921600);
  Serial.setTimeout(5);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (true);
  }

  display.clearDisplay();
  display.display();
}

// ======================================
// Main Loop
// ======================================
void loop()
{
  readSerialWord();

  updateZoomAnimation();

  display.clearDisplay();

  drawRadarWaves();
  drawEqualizer();
  drawPulse();

  if (currentWord.length() > 0)
  {
    drawCenteredWord(currentWord, zoomSize);
  }

  display.display();

  delay(35);
}