#include <Wire.h>
#include <Mouse.h>

#define MPU_ADDR 0x68

// ===== SETTINGS =====
const int DEADZONE = 1200;       // Increase if cursor jitters
const float FILTER = 0.20;       // 0.1 = smoother, 0.5 = faster
const float SENSITIVITY = 0.0025;
const int MAX_SPEED = 25;

// Calibration offsets
long offsetX = 0;
long offsetY = 0;

// Filter variables
float filtX = 0;
float filtY = 0;

void setup() {

  Serial.begin(115200);

  Wire.begin();

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  delay(1000);

  Serial.println("Keep the board flat...");

  calibrate();

  Mouse.begin();

  Serial.println("Ready!");
}

void loop() {

  int16_t ax, ay, az;

  readAccel(ax, ay, az);

  ax -= offsetX;
  ay -= offsetY;

  filtX = FILTER * ax + (1 - FILTER) * filtX;
  filtY = FILTER * ay + (1 - FILTER) * filtY;

  int moveX = 0;
  int moveY = 0;

  if (abs(filtY) > DEADZONE) {

    moveX = filtY * SENSITIVITY;

    if (moveX > MAX_SPEED) moveX = MAX_SPEED;
    if (moveX < -MAX_SPEED) moveX = -MAX_SPEED;
  }

  if (abs(filtX) > DEADZONE) {

    moveY = -filtX * SENSITIVITY;

    if (moveY > MAX_SPEED) moveY = MAX_SPEED;
    if (moveY < -MAX_SPEED) moveY = -MAX_SPEED;
  }

  if (moveX != 0 || moveY != 0)
    Mouse.move(moveX, moveY);

  Serial.print("AX:");
  Serial.print(ax);
  Serial.print(" AY:");
  Serial.print(ay);
  Serial.print(" MX:");
  Serial.print(moveX);
  Serial.print(" MY:");
  Serial.println(moveY);

  delay(8);
}

void calibrate() {

  long sumX = 0;
  long sumY = 0;

  const int samples = 300;

  for (int i = 0; i < samples; i++) {

    int16_t ax, ay, az;

    readAccel(ax, ay, az);

    sumX += ax;
    sumY += ay;

    delay(5);
  }

  offsetX = sumX / samples;
  offsetY = sumY / samples;

  Serial.print("Offset X = ");
  Serial.println(offsetX);

  Serial.print("Offset Y = ");
  Serial.println(offsetY);
}

void readAccel(int16_t &ax, int16_t &ay, int16_t &az) {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
}