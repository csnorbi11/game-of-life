#include "cell.h"
#include "game.h"
#include "grid.h"
#include "main.h"

Cell init_cell(int size)
{
	Cell cellData;
	cellData.alive = false;
	cellData.size = gamesize / size;
	cellData.szin.r = 50;
	cellData.szin.g = 50;
	cellData.szin.b = 50;
	cellData.szin.a = SDL_ALPHA_OPAQUE;
	cellData.livingcount = 0;

	return cellData;
}
void update_cell(Cell* cell)
{
	if (cell->alive)
	{
		cell->szin.r = 0;
		cell->szin.g = 200;
		cell->szin.b = 200;
		cell->szin.a = 1;
	}
	else if (!cell->alive)
	{
		cell->szin.r = 50;
		cell->szin.g = 50;
		cell->szin.b = 50;
		cell->szin.a = 1;
	}
}
int cal_nextstat(Cell cell)
{
	int next = 0;
	if (cell.alive)
	{
		if (cell.livingcount == 2 || cell.livingcount == 3)
			next = 1;
		else
			next = 0;
	}
	else if (!cell.alive)
	{
		if (cell.livingcount == 3)
			next = 1;
	}
	return next;
}
int neighbor(Cell** grid, int i, int j, int size)
{
	int neighborcount = 0;
	if (i != 0 && j != 0 && grid[i - 1][j - 1].alive)
		neighborcount++;
	if (i != 0 && grid[i - 1][j].alive)
		neighborcount++;
	if (i != 0 && j != size - 1 && grid[i - 1][j + 1].alive)
		neighborcount++;
	if (j != 0 && grid[i][j - 1].alive)
		neighborcount++;
	if (j != size - 1 && grid[i][j + 1].alive)
		neighborcount++;
	if (i != size - 1 && j != 0 && grid[i + 1][j - 1].alive)
		neighborcount++;
	if (i != size - 1 && grid[i + 1][j].alive)
		neighborcount++;
	if (i != size - 1 && j != size - 1 && grid[i + 1][j + 1].alive)
		neighborcount++;
	return neighborcount;

}
void render_cell(Cell cell, int i, int j)
{
	SDL_Rect rect = { cell.size * i,cell.size * j,cell.size+3,cell.size+3  };
	SDL_SetRenderDrawColor(game.renderer, cell.szin.r, cell.szin.g, cell.szin.b, cell.szin.a);
	SDL_RenderFillRect(game.renderer, &rect);
	SDL_SetRenderDrawColor(game.renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(game.renderer, &rect);
}