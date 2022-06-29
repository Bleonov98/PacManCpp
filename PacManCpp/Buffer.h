#pragma once
#include "stdfix.h"

#define ROWS 55
#define COLS 120

typedef struct wd {
	int size_rows;
	int size_cols;
	char16_t vBuf[ROWS][COLS]{0};
	int grid[ROWS][COLS]{0};
} WD;

enum WallType {
	MAIN,
	BIG,
	LIL,
	GIANT,
	THIN,
	REGULAR
};

enum CoinType {
	MINI,
	IMMORTAL
};

enum BonusType {
	LOW,
	MID,
	HIGH,
	INSANE
};

enum Colors {
	White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	Red = FOREGROUND_RED,
	Green = FOREGROUND_GREEN,
	Blue = FOREGROUND_BLUE,
	Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	Purple = FOREGROUND_RED | FOREGROUND_BLUE,
	Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
	PacMan = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};