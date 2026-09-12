#include "display.h"
#include "config.h" 


Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Init 
void initDisplay() {
   Wire.begin(8, 9);  // SDA=GPIO8, SCL=GPIO9
  display.begin(I2C_ADDRESS, true);
  display.setRotation(2);  // (1=90°, 2=180°, 3=270°)
  display.clearDisplay();
  display.display();
}

// Static layout 
void drawLayout() {
  // outer border
  display.drawRect(0, 0, 128, 64, SH110X_WHITE);
  // board border (4px margin + 1px border)
  display.drawRect(4, 4, CW * COLS + 2, CH * ROWS + 2, SH110X_WHITE);
}

// Board cells
void drawBoard(uint8_t board[][10], int rows) {
  for (int r = 0; r < rows; r++)
    for (int c = 0; c < COLS; c++)
      if (board[r][c])
        display.fillRect(
          BX + c * CW,
          BY + r * CH,
          CW - 1, CH - 1,
          SH110X_WHITE
        );
}

// Current piece
void drawCurrentPiece(int8_t shape[][4], int x, int y) {
  for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++)
      if (shape[r][c] && y + r >= 0)
        display.fillRect(
          BX + (x + c) * CW,
          BY + (y + r) * CH,
          CW - 1, CH - 1,
          SH110X_WHITE
        );
}

// Sidebar 
void drawSidebar(int8_t nextShape[][4], int nextSize, int score, int level) {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  // NEXT box: y=4, h=20 
  display.drawRect(BOXX, 4, BOXW, 20, SH110X_WHITE);
  display.setCursor(SX + 1, 9);
  display.print("NEXT");

  // next piece preview (cell 5px)
  const int PC = 5;
  for (int r = 0; r < nextSize; r++)
    for (int c = 0; c < nextSize; c++)
      if (nextShape[r][c])
        display.fillRect(
          BOXX + 3 + c * PC,
          4 + 2 + r * PC,
          PC - 1, PC - 1,
          SH110X_WHITE
        );

  // SCORE box: y=26, h=16 
  display.drawRect(BOXX, 26, BOXW, 16, SH110X_WHITE);
  display.setCursor(SX + 1, 31);
  display.print("SCORE");
  display.setCursor(BOXX + 4, 31);
  display.print(score);

  // LEVEL box: y=44, h=14 
  display.drawRect(BOXX, 44, BOXW, 14, SH110X_WHITE);
  display.setCursor(SX + 1, 48);
  display.print("LEVEL");
  display.setCursor(BOXX + 4, 48);
  display.print(level);
}

void drawGameOver(int score, unsigned long seconds) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  // Outer border
  display.drawRect(0, 0, 128, 64, SH110X_WHITE);

  display.setCursor(34, 6);
  display.print("GAME OVER");

  display.drawLine(8, 17, 119, 17, SH110X_WHITE);

  // แสดงคะแนน
  display.setCursor(16, 23);
  display.print("SCORE: ");
  display.print(score);

  // แสดงเวลาที่เล่นไป (วินาที)
  display.setCursor(16, 35);
  display.print("TIME : ");
  display.print(seconds);
  display.print(" s");

  // ข้อความรีสตาร์ท
  display.setCursor(20, 50);
  display.print("RST to restart");

  display.display();
}
