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

// Functions
void initGame();
bool collides(const Piece& p, int ox = 0, int oy = 0, const int8_t shp[][4] = nullptr);
void tryRotate();
void lockAndSpawn();
void updateGame();