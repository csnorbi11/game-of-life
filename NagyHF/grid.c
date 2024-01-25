#include "main.h"
#include "grid.h"
#include "debugmalloc.h"
#include "game.h"
#include "cell.h"
#include <omp.h>

int gamesize;

bool play=false;
bool step = false;



void init_grid(Cell*** grid, int*** actual, int*** next, int*** update, int size)
{
	SDL_Log("Starting grid to init.\n");
	gamesize = windowsize.HEIGHT;

	*grid = (Cell**)malloc(size * sizeof(Cell*));
	*actual = (int**)malloc(size * sizeof(int*));
	*next = (int**)malloc(size * sizeof(int*));
	*update = (int**)malloc(size * sizeof(int*));
	SDL_RenderClear(game.renderer);
	for (int i = 0; i < size; i++)
	{
		(*grid)[i] = (Cell*)malloc(size * sizeof(Cell));
		(*actual)[i] = (int*)malloc(size * sizeof(int));
		(*next)[i] = (int*)malloc(size * sizeof(int));
		(*update)[i] = (int*)malloc(size * sizeof(int));
		for (int j = 0; j < size; j++)
		{
			(*grid)[i][j] = init_cell(size);
			render_cell(***grid, i, j);
			(*actual)[i][j] = 0;
			(*next)[i][j] = 0;
			(*update)[i][j] = 0;
		}
	}

	SDL_RenderPresent(game.renderer);
	SDL_Log("Grid init done.\n");
}
void update_grid(Cell** grid, int** actual, int** next, int** update, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (actual[i][j] == 1)
				grid[i][j].alive = true;
			else
				grid[i][j].alive = false;
			if (update[i][j] == 1)
			{
				update_cell(grid[i]+j);
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			grid[i][j].livingcount = neighbor(grid, i, j,size);
			next[i][j] = cal_nextstat(grid[i][j]);
			if (next[i][j] != actual[i][j])
				update[i][j] = 1;
			else
				update[i][j] = 0;
			actual[i][j] = next[i][j];
		}
	}
}
void draw_grid(Cell** grid, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			render_cell(grid[i][j], i, j);
		}
	}
}
void random_alive(int** actual, int size)
{
	time_t t;
	srand(time(&t));
	int randomcellamount = rand() % (size * size + size/10);
	for (int i = 0; i < randomcellamount; i++)
	{
		int f = rand() % size;
		int s = rand() % size;
		actual[f][s] = 1;
	}
}
void randomize_grid(Cell** grid, int** actual, int** next, int** update, int size)
{
	random_alive(*actual, size);
	update_grid(*grid, *actual, *next, *update,size);
	draw_grid(*grid,size);
	SDL_RenderPresent(game.renderer);
}
void free_grid(Cell*** grid, int*** actual, int*** next, int*** update, int size)
{
	for (int i = 0; i < size; i++)
	{
		free((*grid)[i]);
		free((*actual)[i]);
		free((*next)[i]);
		free((*update)[i]);
	}
	free(*grid);
	free(*actual);
	free(*next);
	free(*update);
}
void resize_grid(Cell*** grid, int*** actual, int*** next, int*** update, int* size)
{
	SDL_RenderClear(game.renderer);
	printf("Give me the new size of the grid\n");
	int newsize = 0;
	scanf_s("%d", &newsize);
	if (*size == newsize)
	{
		SDL_Log("Grid resizing havent happened\n");
		return;
	}
		
	SDL_Log("Starting grid to resize.\n");
	int s = newsize > *size ? *size : newsize;
	
	Cell** newgrid = (Cell**)malloc(s * sizeof(Cell*));
	int** newactual = (int**)malloc(s * sizeof(int*));
	int** newnext = (int**)malloc(s * sizeof(int*));
	int** newtoupdate = (int**)malloc(s * sizeof(int*));
	

	for (int i = 0; i < s; i++)
	{
		newgrid[i] = (Cell*)malloc(s * sizeof(Cell));
		newactual[i] = (int*)malloc(s * sizeof(int));
		newnext[i] = (int*)malloc(s * sizeof(int));
		newtoupdate[i] = (int*)malloc(s * sizeof(int));
		int j;
		for (j = 0; j < s; j++)
		{
			newgrid[i][j] = (*grid)[i][j];
			newactual[i][j] = (*actual)[i][j];
			newnext[i][j] = (*next)[i][j];
			newtoupdate[i][j] = (*update)[i][j];
		}
	}
	free_grid(grid,actual,next,update,*size);
	*size = newsize;

	*grid = (Cell**)malloc(newsize * sizeof(Cell*));
	*actual = (int**)malloc(newsize * sizeof(int*));
	*next = (int**)malloc(newsize * sizeof(int*));
	*update = (int**)malloc(newsize * sizeof(int*));
	for (int i = 0; i < newsize; i++)
	{
		(*grid)[i] = (Cell*)malloc(newsize * sizeof(Cell));
		(*actual)[i] = (int*)malloc(newsize * sizeof(int));
		(*next)[i] = (int*)malloc(newsize * sizeof(int));
		(*update)[i] = (int*)malloc(newsize * sizeof(int));
		for (int j = 0; j < newsize; j++)
		{
			(*grid)[i][j] = init_cell(*size);
			(*actual)[i][j] = 0;
			(*next)[i][j] = 0;
			(*update)[i][j] = 0;
		}
	}

	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			render_cell(***grid, i, j);
			(*actual)[i][j] = newactual[i][j];
			(*next)[i][j] = newnext[i][j];
			(*update)[i][j] = newtoupdate[i][j];
		}
	}

	for (int i = 0; i < s; i++)
	{
		free(newgrid[i]);
		free(newactual[i]);
		free(newnext[i]);
		free(newtoupdate[i]);
	}
	free(newgrid);
	free(newactual);
	free(newnext);
	free(newtoupdate);
	update_grid(*grid,*actual,*next,*update,*size);
	draw_grid(*grid,*size);
	SDL_RenderPresent(game.renderer);
	SDL_Log("Grid resizing done.\n");
}