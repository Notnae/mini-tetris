#include "input.h"

// Input state
static unsigned long lastMoveX = 0;
static unsigned long lastMoveY = 0;
static bool          lastRotBtn = HIGH;
void initInput() {
  pinMode(PIN_ROT, INPUT_PULLUP);
  pinMode(PIN_RESET, INPUT_PULLUP);  
  analogReadResolution(12);  // ESP32: 0–4095
}

void processInput() {
  if (gameOver) return;

  unsigned long now = millis();

  int raw_x = analogRead(PIN_JOY_X) - 2048;
  int raw_y = analogRead(PIN_JOY_Y) - 2048;

  // หมุน 90 องศาขวา
  int cx = -raw_y;   
  int cy =  raw_x;  

  // ซ้าย / ขวา
  int dx = 0;
  if      (cx < -JOY_DEADZONE) dx = -1;
  else if (cx >  JOY_DEADZONE) dx =  1;

  if (dx != 0) {
    if (lastMoveX == 0 || now - lastMoveX >= JOY_REPEAT_MS) {
      if (!collides(cur, dx, 0)) cur.x += dx;
      lastMoveX = now;
    }
  } else {
    lastMoveX = 0;
  }

  // Soft drop
  if (cy > JOY_DEADZONE) {
    if (lastMoveY == 0 || now - lastMoveY >= DROP_REPEAT_MS) {
      if (!collides(cur, 0, 1)) { cur.y++; score++; }
      lastMoveY = now;
    }
  } else {
    lastMoveY = 0;
  }

  // Rotate
  bool btn = digitalRead(PIN_ROT);
  Serial.println(btn);
  if (btn == LOW && lastRotBtn == HIGH) tryRotate();
  lastRotBtn = btn;
}