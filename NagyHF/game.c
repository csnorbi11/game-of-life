#include "game.h"
#include "stdio.h"
#include "main.h"
#include "grid.h"
#include "cell.h"
#include "filehandling.h"
#include <stdio.h>

Game game;
const char TITLE[5] = "Game";
WindowSize windowsize;

int quit = 0;

int playdelay = 100;


void window_init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("Nem sikerült inicializálni a SDL-t: %s", SDL_GetError());
		exit(1);
	}
	game.window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowsize.WIDTH, windowsize.HEIGHT, 0);
	if (game.window == NULL)
	{
		SDL_Log("Nem sikerült létrehozni az ablakot: %s", SDL_GetError());
		exit(1);
	}
	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
	if (game.renderer == NULL)
	{
		SDL_Log("Nem sikerült létrehozni a kirajzolót: %s", SDL_GetError());
		exit(1);
	}
	SDL_Log("Minden inicializálva lett ami kell");
}
void game_main()
{
	FILE* fp=NULL;
	Cell** grid=NULL;
	int size = 10;
	int** actualstat=NULL;
	int** nextstat=NULL;
	int** toupdate=NULL;

	game_init(&grid,&actualstat,&nextstat,&toupdate,size);

	game_loop(&grid,&actualstat,&nextstat,&toupdate,&size, &fp);

	
	SDL_Quit();
	free_grid(&grid, &actualstat, &nextstat, &toupdate, size);
	SDL_DestroyWindow(game.window);
}
void game_init(Cell*** grid, int*** actual, int*** next, int*** update, int size)
{
	get_windowsize();
	window_init();
	SDL_RenderClear(game.renderer);
	init_grid(grid,actual,next,update,size);
}
void game_loop(Cell*** grid, int*** actual, int*** next, int*** update, int *size, FILE* fp)
{
	char fname[51] = { 0 };
	while (quit != 1)
	{
		SDL_RenderClear(game.renderer);

		SDL_Event ev;
		SDL_PollEvent(&ev);
		switch (ev.type)
		{
		case SDL_KEYDOWN:
			switch (ev.key.keysym.sym)
			{
			case SDLK_SPACE:
				if (play)
					play = false;
				else
					play = true;
				break;
			case SDLK_UP:
				if (playdelay >= 50)
					playdelay -= 20;
				break;
			case SDLK_DOWN:
				if (playdelay < 200)
					playdelay += 20;
				break;
			case SDLK_r:
				randomize_grid(grid, actual, next, update,*size);
				break;
			case SDLK_g:
				resize_grid(grid, actual, next, update,size);
				break;
			case SDLK_w:
				//scanf_s("%s", fname);
				write_file(&fp, fname, *actual, *size);
				break;
			case SDLK_q:
				//scanf_s("%s", fname);
				read_file(&fp, fname, *actual, *size);
				update_grid(*grid, *actual, *next, *update, *size);
				draw_grid(*grid, *size);
				SDL_RenderPresent(game.renderer);
				break;
			case SDLK_ESCAPE:
				quit = 1;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (ev.key.keysym.sym)
			{
			case SDLK_LCTRL:
				step = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
		{
			int mouseX = ev.button.x;
			int mouseY = ev.button.y;
			on_mouseclick(grid, actual, *size, mouseX, mouseY);
		}
		break;
		case SDL_QUIT:
			quit = 1;
			break;
		}

		if (play)
		{
			update_grid(*grid, *actual, *next, *update, *size);
			draw_grid(*grid, *size);
			SDL_RenderPresent(game.renderer);
		}
		else if (step)
		{
			update_grid(*grid, *actual, *next, *update, *size);
			draw_grid(*grid, *size);
			SDL_RenderPresent(game.renderer);
			step = false;
		}


		SDL_Delay(playdelay);
	}
}
void get_windowsize()
{
	printf("---------------------\n");
	printf("CONWAY'S GAME OF LIFE\n");
	printf("---------------------\n\n");
	printf("SPACE - Pause game\nUP ARROW - Speed up\nDOWN ARROW - Speed down\nLEFT CTRL - Step\n");
	printf("G - Resize the grid\nR - Randomize the grid\nQ - Read file\nR - Write file\n");
	printf("------------------\n");
	printf("Select window size:\n");
	printf("1. 800x600\n2. 1280x720\n3. Custom Resolution\n0. Quit\n");
	int sel;
	scanf_s("%d", &sel);
	switch (sel)
	{
	case 1:
		windowsize.WIDTH = 800;
		windowsize.HEIGHT = 600;
		break;
	case 2:
		windowsize.WIDTH = 1820;
		windowsize.HEIGHT = 720;
		break;
	case 3:
		printf("Give the width of the window: ");
		scanf_s("%d", &windowsize.WIDTH);
		printf("Give the height of the window: ");
		scanf_s("%d", &windowsize.HEIGHT);
		break;
	case 0:
		quit = 1;
		break;
	}
}
void on_mouseclick(Cell*** grid, int*** actual, int size, int mouseX, int mouseY)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int cellsize = (*grid)[i][j].size;
			int cellstartx = i * cellsize;
			int cellstarty = j * cellsize;
			int cellendx = i * cellsize + cellsize;
			int cellendy = j * cellsize + cellsize;
			if (mouseX > cellstartx && mouseX<cellendx && mouseY>cellstarty && mouseY < cellendy)
			{
				if ((*actual)[i][j] == 1)
				{
					(*actual)[i][j] = 0;
					(*grid)[i][j].alive = false;
				}
				else
				{
					(*actual)[i][j] = 1;
					(*grid)[i][j].alive = true;
				}
				update_cell((*grid)[i] + j);
			}
		}


	}
	draw_grid(*grid, size);
	SDL_RenderPresent(game.renderer);
}