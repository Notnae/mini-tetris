#pragma once
#include <Arduino.h>
#include "config.h"

// Structs 
struct Piece {
  int8_t shape[4][4];
  int8_t size;
  int8_t x, y;
};

// Game state
extern uint8_t board[ROWS][COLS];
extern Piece   cur;
extern Piece   nxt;
extern int     score;
extern int     lines;
extern int     level;
extern bool    gameOver;
extern unsigned long dropMs;
// เพิ่มตัวแปรเก็บเวลาสำหรับ extern
extern unsigned long gameStartTime;
extern unsigned long gameDurationSec;

// Functions
void initGame();
bool collides(const Piece& p, int ox = 0, int oy = 0, const int8_t shp[][4] = nullptr);
void tryRotate();
void lockAndSpawn();
void updateGame();
// ปรับ prototype ของ drawGameOver ให้รับค่าเวลา (วินาที) เพิ่ม
void drawGameOver(int score, unsigned long seconds);
