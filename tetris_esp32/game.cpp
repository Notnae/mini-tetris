#include "game.h"

// --- เพิ่มตัวแปรระดับโกลบอลด้านบน ---
unsigned long gameStartTime = 0;
unsigned long gameDurationSec = 0;

// Tetrominoes
const int8_t SHAPES[7][4][4] = {
  {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I
  {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // O
  {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
  {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
  {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z
  {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J
  {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // L
};
const int8_t SIZES[7] = {4, 2, 3, 3, 3, 3, 3};

// State definitions
uint8_t      board[ROWS][COLS];
Piece        cur, nxt;
int          score    = 0;
int          lines    = 0;
int          level    = 1;
bool         gameOver = false;
unsigned long dropMs  = BASE_DROP_MS;

unsigned long lastGravity = 0;

// Internal
static void makeRandomPiece(Piece& p) {
  int t = random(7);
  memcpy(p.shape, SHAPES[t], sizeof(p.shape));
  p.size = SIZES[t];
  p.x    = (COLS - p.size) / 2;  // แก้จาก (COLS - 4) / 2
  p.y    = 0;
}

// --- แก้ไขใน initGame() ---
void initGame() {
  memset(board, 0, sizeof(board));
  score         = 0;
  lines         = 0;
  level         = 1;
  gameOver      = false;
  dropMs        = BASE_DROP_MS;
  randomSeed(esp_random());
  makeRandomPiece(nxt);
  makeRandomPiece(cur);

  // เริ่มจับเวลาใหม่เมื่อเริ่มเกม
  gameStartTime = millis();
  gameDurationSec = 0;
}

bool collides(const Piece& p, int ox, int oy, const int8_t shp[][4]) {
  for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++) {
      int8_t v = shp ? shp[r][c] : p.shape[r][c];
      if (!v) continue;
      int nx = p.x + c + ox;
      int ny = p.y + r + oy;
      if (nx < 0 || nx >= COLS || ny >= ROWS) return true;
      if (ny >= 0 && board[ny][nx])           return true;
    }
  return false;
}

void tryRotate() {
  int8_t rotated[4][4];
  memset(rotated, 0, sizeof(rotated));
  for (int r = 0; r < cur.size; r++)
    for (int c = 0; c < cur.size; c++)
      rotated[c][cur.size - 1 - r] = cur.shape[r][c];

  int kicks[] = {0, 1, -1, 2, -2};
  for (int k : kicks) {
    if (!collides(cur, k, 0, rotated)) {
      cur.x += k;
      memcpy(cur.shape, rotated, sizeof(cur.shape));
      return;
    }
  }
}

void lockAndSpawn() {
  // lock
  for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++)
      if (cur.shape[r][c] && cur.y + r >= 0)
        board[cur.y + r][cur.x + c] = 1;

  // clear lines
  int cleared = 0;
  for (int r = ROWS - 1; r >= 0; r--) {
    bool full = true;
    for (int c = 0; c < COLS; c++) if (!board[r][c]) { full = false; break; }
    if (full) {
      memmove(board[1], board[0], r * sizeof(board[0]));
      memset(board[0], 0, sizeof(board[0]));
      cleared++; r++;
    }
  } // สิ้นสุดลูปตรวจแถวตรงนี้ (เอาบล็อก spawn ในลูปเดิมออก)

  // score
  const int pts[] = {0, 100, 300, 500, 800};
  score  += pts[cleared] * level;
  lines  += cleared;
  level   = lines / LEVEL_STEP + 1;
  dropMs  = max((long)MIN_DROP_MS, (long)BASE_DROP_MS - (level - 1) * 50L);

  // spawn (ให้ทำงานจุดนี้เพียงจุดเดียว)
  cur = nxt;
  makeRandomPiece(nxt);
  if (collides(cur)) {
    gameOver = true;
    gameDurationSec = (millis() - gameStartTime) / 1000;
  }
}

void updateGame() {
  if (gameOver) return;

  unsigned long now = millis();
  if (now - lastGravity >= dropMs) {
    lastGravity = now;
    if (!collides(cur, 0, 1)) cur.y++;
    else lockAndSpawn();
  }
}
