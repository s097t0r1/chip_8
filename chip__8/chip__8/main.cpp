#include <ctime>
#include <random>
#include <SDL.h>
#include "chip8.h"


SDL_Window* gwindow;
SDL_Renderer* grenderer;
SDL_Event e;

bool init();
void free();

int main(int argc, char* argv[])
{
	bool quit = 0;
	init();
	Chip8 chip8;
	srand(time(NULL));
	chip8.initialize();
	if (!chip8.load_ROM("computer.ch8"))
	{  
		return 0;
	}

	while(!quit)
	{
		while (SDL_PollEvent(&e) > 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = 1;
			}
				
		}

		chip8.emulate_cycle();

		

		if (chip8.drawflag)
		{
			SDL_SetRenderDrawColor(grenderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(grenderer);
			SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			//chip8.drawn_debug();
			chip8.drawn(grenderer);
			chip8.drawflag = 0;
			
		} 

		SDL_RenderPresent(grenderer);
		
	}

}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error", SDL_GetError());
		return false;
	}
		
	else
	{
		gwindow = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
		if (gwindow == NULL)
		{
			printf("Error", SDL_GetError());
			return false;
		}
		else
		{
			grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
			if (grenderer == NULL)
			{
				printf("Error", SDL_GetError());
			}
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
