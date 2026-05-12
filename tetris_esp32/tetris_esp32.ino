#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "config.h"
#include "display.h"
#include "game.h"
#include "input.h"

bool lastResetBtn = HIGH;

void setup() {
  Serial.begin(115200);
  delay(1000);  
  Serial.println("=== BOOT ===");
  initInput();
  Serial.println("input OK");
  initDisplay();
  Serial.println("display OK");
  initGame();
  Serial.println("game OK");
}

void loop() {
  // Reset button
  bool resetBtn = digitalRead(PIN_RESET);
  if (resetBtn == LOW && lastResetBtn == HIGH) {
    initGame();
  }
  lastResetBtn = resetBtn;
  processInput();
  updateGame();

  if (gameOver) {
    drawGameOver(score);
    return;
  }

  display.clearDisplay();
  drawLayout();
  drawBoard(board, ROWS);
  drawCurrentPiece(cur.shape, cur.x, cur.y);
  drawSidebar(nxt.shape, nxt.size, score, level);
  display.display();

  delay(10);
}