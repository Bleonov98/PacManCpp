#pragma once
#include "stdfix.h"

#define ROWS 55
#define COLS 120

typedef struct wd {
	int size_rows;
	int size_cols;
	char16_t vBuf[ROWS][COLS]{0};
} WD;