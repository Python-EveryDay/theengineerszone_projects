#define LEFT_PIN   A3
#define RIGHT_PIN  A4
#define UP_PIN     A5
#define DOWN_PIN   A6

int prevLeft = HIGH;
int prevRight = HIGH;
int prevUp = HIGH;
int prevDown = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
}

void loop() {

  int leftState = digitalRead(LEFT_PIN);
  int rightState = digitalRead(RIGHT_PIN);
  int upState = digitalRead(UP_PIN);
  int downState = digitalRead(DOWN_PIN);

  // LEFT
  if (leftState != prevLeft) {
    if (leftState == LOW)
      Serial.println("LEFT_DOWN");
    else
      Serial.println("LEFT_UP");

    prevLeft = leftState;
  }

  // RIGHT
  if (rightState != prevRight) {
    if (rightState == LOW)
      Serial.println("RIGHT_DOWN");
    else
      Serial.println("RIGHT_UP");

    prevRight = rightState;
  }

  // UP
  if (upState != prevUp) {
    if (upState == LOW)
      Serial.println("UP_DOWN");
    else
      Serial.println("UP_UP");

    prevUp = upState;
  }

  // DOWN
  if (downState != prevDown) {
    if (downState == LOW)
      Serial.println("DOWN_DOWN");
    else
      Serial.println("DOWN_UP");

    prevDown = downState;
  }

  delay(10);
}