#pragma once
#include "SDL.h"
#include <stdbool.h>

#define MAIN_H

typedef struct Cell {
	SDL_Color szin;
	double size;
	bool alive;
	int livingcount;
} Cell;