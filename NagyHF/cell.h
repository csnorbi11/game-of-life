#pragma once
#include "main.h"

#define CELL_H

Cell init_cell(int size);
void update_cell(Cell* cell);
int cal_nextstat(Cell cell);
int neighbor(Cell** grid, int i, int j, int size);
void render_cell(Cell cell, int i, int j);