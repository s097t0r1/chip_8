#include <ctime>
#include <random>
#include "chip8.h"
#include <SDL.h>

SDL_Window* gwindow;
SDL_Renderer* grenderer;

bool init();
void free();

int main(int argc, char* argv[])
{
	init();
	Chip8 chip8;
	srand(time(NULL));
	chip8.initialize();
	if (!chip8.load_ROM("test_rom.c8"))
	{
		return 0;
	}

	for(;;)
	{
		chip8.emulate_cycle();
		
		if (chip8.drawflag)
		{

		}
			chip8.drawn();
	}

	system("pause");
}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("", SDL_GetError());
		return false;
	}
		
	else
	{
		gwindow = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
		if (gwindow = NULL)
		{
			printf("Error", SDL_GetError());
			return false;
		}
		else
		{
			grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
		}
	}
}

void free()
{
	SDL_DestroyWindow(gwindow);
	gwindow = NULL;
	SDL_DestroyRenderer(grenderer);
	grenderer = NULL;
}
