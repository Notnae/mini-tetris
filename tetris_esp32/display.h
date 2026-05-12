#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

extern Adafruit_SH1106G display;

void initDisplay();
void drawLayout();
void drawBoard(uint8_t board[][10], int rows);
void drawCurrentPiece(int8_t shape[][4], int x, int y);
void drawSidebar(int8_t nextShape[][4], int nextSize, int score, int level);
void drawGameOver(int score);