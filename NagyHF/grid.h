#pragma once
#include "main.h"

#define GRID_H

extern int gamesize;

extern bool play;
extern bool step;

void init_grid(Cell*** grid, int*** actual, int*** next, int*** update, int size);
void update_grid(Cell** grid, int** actual, int** next, int** update, int size);
void draw_grid(Cell** grid, int size);
void random_alive(int** actual, int size);
void randomize_grid(Cell** grid, int** actual, int** next, int** update, int size);
void free_grid(Cell*** grid, int*** actual, int*** next, int*** update, int size);
void resize_grid(Cell*** grid, int*** actual, int*** next, int*** update, int *size);