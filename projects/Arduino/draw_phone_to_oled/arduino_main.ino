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

String buffer = "";

void setup() {

  Serial.begin(115200);

  if(!display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C))
  {
    while(true);
  }

  display.clearDisplay();
  display.display();
}

void loop() {

  while (Serial.available()) {

    char c = Serial.read();

    if (c == '\n') {

      processCommand(buffer);

      buffer = "";
    }
    else {
      buffer += c;
    }
  }
}

void processCommand(String cmd) {

  if (cmd == "C") {

    display.clearDisplay();
    display.display();
    return;
  }

  if (cmd.startsWith("L,")) {

    int values[4];
    int idx = 0;

    char temp[50];
    cmd.toCharArray(temp, 50);

    char *token = strtok(temp, ",");

    token = strtok(NULL, ",");

    while (token != NULL && idx < 4) {

      values[idx++] = atoi(token);

      token = strtok(NULL, ",");
    }

    if (idx == 4) {

      display.drawLine(
        values[0],
        values[1],
        values[2],
        values[3],
        SSD1306_WHITE
      );

      display.display();
    }
  }
}