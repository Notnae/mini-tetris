#pragma once

// Pin
#define PIN_JOY_X   1
#define PIN_JOY_Y   0
#define PIN_ROT     4
#define PIN_RESET   7   

// Display
#define I2C_ADDRESS   0x3C
#define OLED_RESET    -1      
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

// Board
#define COLS    10
#define ROWS    13
#define CW      6
#define CH      4

// Layout
#define BX      5
#define BY      5
#define SX      68
#define BOXX    97
#define BOXW    27

// Timing
#define JOY_DEADZONE    400
#define JOY_REPEAT_MS   150
#define DROP_REPEAT_MS  80
#define BASE_DROP_MS    600
#define MIN_DROP_MS     80
#define LEVEL_STEP      10