#define RIGHT_PIN 3
#define JUMP_PIN 4

int prevRight = HIGH;
int prevJump = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(JUMP_PIN, INPUT_PULLUP);
}

void loop() {

  int rightState = digitalRead(RIGHT_PIN);
  int jumpState = digitalRead(JUMP_PIN);

  if (rightState != prevRight) {
    if (rightState == LOW)
      Serial.println("RIGHT_DOWN");
    else
      Serial.println("RIGHT_UP");

    prevRight = rightState;
  }

  if (jumpState != prevJump) {
    if (jumpState == LOW)
      Serial.println("JUMP_DOWN");
    else
      Serial.println("JUMP_UP");

    prevJump = jumpState;
  }

  delay(10);
}