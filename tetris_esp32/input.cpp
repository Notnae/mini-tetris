#include "input.h"

// บันทึกค่ากึ่งกลางของจอยสติ๊ก
static int center_x = 2048;
static int center_y = 2048;

static unsigned long lastMoveX = 0;
static unsigned long lastMoveY = 0;
static bool          lastRotBtn = HIGH;

void initInput() {
  pinMode(PIN_ROT, INPUT_PULLUP);
  pinMode(PIN_RESET, INPUT_PULLUP);  
  analogReadResolution(12);

  // Calibrate หาค่ากึ่งกลางจริงตอนเริ่มระบบ
  long sx = 0, sy = 0;
  for (int i = 0; i < 10; i++) {
    sx += analogRead(PIN_JOY_X);
    sy += analogRead(PIN_JOY_Y);
    delay(10);
  }
  center_x = sx / 10;
  center_y = sy / 10;
}

void processInput() {
  if (gameOver) return;

  unsigned long now = millis();

  // ลบด้วยค่า center_x และ center_y ที่ Calibrate ได้
  int raw_x = analogRead(PIN_JOY_X) - center_x;
  int raw_y = analogRead(PIN_JOY_Y) - center_y;

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
  if (btn == LOW && lastRotBtn == HIGH) tryRotate();
  lastRotBtn = btn;
}
