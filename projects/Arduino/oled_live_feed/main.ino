#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

uint8_t frameBuffer[1024];
void startupAnimation()
{
  // Boot Screen
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(18, 5);
  display.println("BOOT");

  display.setTextSize(1);
  display.setCursor(15, 32);
  display.println("The Engineers Zone");

  display.display();

  delay(1500);

  // Loading Bar
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(25, 15);
  display.println("Loading...");

  display.drawRect(10, 35, 108, 10, WHITE);

  for (int i = 0; i <= 104; i += 2)
  {
    display.fillRect(12, 37, i, 6, WHITE);
    display.display();
    delay(35);
  }

  delay(500);

  // Connecting Camera
  display.clearDisplay();

  display.setCursor(10, 18);
  display.println("Connecting Camera");

  for (int k = 0; k < 3; k++)
  {
    for (int i = 0; i < 4; i++)
    {
      display.fillCircle(40 + i * 15, 40, 2, WHITE);
      display.display();
      delay(250);
    }

    delay(100);

    display.clearDisplay();
    display.setCursor(10, 18);
    display.println("Connecting Camera");
  }

  // Countdown
  for (int n = 3; n > 0; n--)
  {
    display.clearDisplay();

    display.setTextSize(5);
    display.setCursor(50, 12);
    display.print(n);

    display.display();

    delay(700);
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("CAMERA READY");

  display.display();

  delay(1000);

  display.clearDisplay();
  display.display();
}


void setup()
{
  Serial.begin(921600);

  if (!display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C))
  
  {
    while (1);
  }
  startupAnimation();
}

void loop()
{
  int received = 0;

  while (received < 1024)
  {
    if (Serial.available())
    {
      frameBuffer[received++] = Serial.read();
    }
  }

  display.clearDisplay();

  for (int page = 0; page < 8; page++)
  {
    for (int x = 0; x < 128; x++)
    {
      uint8_t b = frameBuffer[page * 128 + x];

      for (int bit = 0; bit < 8; bit++)
      {
        if (b & (1 << bit))
        {
          display.drawPixel(
            x,
            page * 8 + bit,
            WHITE
          );
        }
      }
    }
  }

  display.display();
}