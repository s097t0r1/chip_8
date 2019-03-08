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
	if (!chip8.load_ROM("blitz"))
	{  
		return 0;
	}
	while(!quit)
	{
		if(SDL_PollEvent(&e) > 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = 1;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{

					case SDLK_0:
						chip8.keys[0] = 0;
						break;

					case SDLK_1:
						chip8.keys[1] = 0;
						break;

					case SDLK_2:
						chip8.keys[2] = 0;
						break;

					case SDLK_3:
						chip8.keys[3] = 0;
						break;

					case SDLK_4:
						chip8.keys[4] = 0;
						break;

					case SDLK_5:
						chip8.keys[5] = 0;
						break;

					case SDLK_6:
						chip8.keys[6] = 0;
						break;

					case SDLK_7:
						chip8.keys[7] = 0;
						break;

					case SDLK_8:
						chip8.keys[8] = 0;
						break;

					case SDLK_9:
						chip8.keys[9] = 0;
						break;

					case SDLK_a:
						chip8.keys[10] = 0;
						break;

					case SDLK_b:
						chip8.keys[11] = 0;
						break;

					case SDLK_c:
						chip8.keys[12] = 0;
						break;

					case SDLK_d:
						chip8.keys[13] = 0;
						break;

					case SDLK_e:
						chip8.keys[14] = 0;
						break;

					case SDLK_f:
						chip8.keys[15] = 0;
						break;

					default:
						break;
				}
			}
			else if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{

				case SDLK_0:
					chip8.keys[0] = 1;
					break;

				case SDLK_1:
					chip8.keys[1] = 1;
					break;

				case SDLK_2:
					chip8.keys[2] = 1;
					break;

				case SDLK_3:
					chip8.keys[3] = 1;
					break;

				case SDLK_4:
					chip8.keys[4] = 1;
					break;

				case SDLK_5:
					chip8.keys[5] = 1;
					break;

				case SDLK_6:
					chip8.keys[6] = 1;
					break;

				case SDLK_7:
					chip8.keys[7] = 1;
					break;

				case SDLK_8:
					chip8.keys[8] = 1;
					break;

				case SDLK_9:
					chip8.keys[9] = 1;
					break;

				case SDLK_a:
					chip8.keys[10] = 1;
					break;

				case SDLK_b:
					chip8.keys[11] = 1;
					break;

				case SDLK_c:
					chip8.keys[12] = 1;
					break;

				case SDLK_d:
					chip8.keys[13] = 1;
					break;

				case SDLK_e:
					chip8.keys[14] = 1;
					break;

				case SDLK_f:
					chip8.keys[15] = 1;
					break;

				default:
					break;
				}
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
