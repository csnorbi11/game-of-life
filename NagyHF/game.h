#pragma once
#include "SDL.h"
#include "main.h"
#include <stdio.h>

#define GAME_H

typedef struct Game {
	SDL_Window* window;
	SDL_Renderer* renderer;
} Game;
typedef struct WindowSize {
	int WIDTH;
	int HEIGHT;
} WindowSize;

extern Game game;
extern WindowSize windowsize;
	

void window_init();
void game_main();
void game_init(Cell*** grid, int*** actual, int*** next, int*** update, int size);
void game_loop(Cell*** grid, int*** actual, int*** next, int*** update, int *size, FILE* fp);
void get_windowsize();
void on_mouseclick(Cell*** grid, int*** actual, int size, int mouseX, int mouseY);