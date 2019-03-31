#include <ctime>
#include <random>
#include <SDL.h>
#include "chip8.h"
#include <string.h>
#include <SDL_mixer.h>

SDL_Window* gwindow;
SDL_Renderer* grenderer;
SDL_Event e;




bool init();
void free();
char* concat(const char*, const char*);

int main(int argc, char* argv[])
{
	char* path = concat("../chip__8/roms/", argv[1]);

	if (argc == 2)
	{
		bool quit = 0;
		init();
		Chip8 chip8;
		srand(time(NULL));
		chip8.initialize();
		if (!chip8.load_ROM(path))
		{
			return 0;
		}
		while (!quit)
		{
			if (SDL_PollEvent(&e) > 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = 1;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{

					case SDLK_1:
						chip8.keys[0] = 1;
						break;

					case SDLK_2:
						chip8.keys[1] = 1;
						break;

					case SDLK_3:
						chip8.keys[2] = 1;
						break;

					case SDLK_4:
						chip8.keys[3] = 1;
						break;

					case SDLK_q:
						chip8.keys[4] = 1;
						break;

					case SDLK_w:
						chip8.keys[5] = 1;
						break;

					case SDLK_e:
						chip8.keys[6] = 1;
						break;

					case SDLK_r:
						chip8.keys[7] = 1;
						break;

					case SDLK_a:
						chip8.keys[8] = 1;
						break;

					case SDLK_s:
						chip8.keys[9] = 1;
						break;

					case SDLK_d:
						chip8.keys[10] = 1;
						break;

					case SDLK_f:
						chip8.keys[11] = 1;
						break;

					case SDLK_z:
						chip8.keys[12] = 1;
						break;

					case SDLK_x:
						chip8.keys[13] = 1;
						break;

					case SDLK_c:
						chip8.keys[14] = 1;
						break;

					case SDLK_v:
						chip8.keys[15] = 1;
						break;

					default:
						break;
					}
				}
				else if (e.type == SDL_KEYUP)
				{
					switch (e.key.keysym.sym)
					{

					case SDLK_1:
						chip8.keys[0] = 0;
						break;

					case SDLK_2:
						chip8.keys[1] = 0;
						break;

					case SDLK_3:
						chip8.keys[2] = 0;
						break;

					case SDLK_4:
						chip8.keys[3] = 0;
						break;

					case SDLK_q:
						chip8.keys[4] = 0;
						break;

					case SDLK_w:
						chip8.keys[5] = 0;
						break;

					case SDLK_e:
						chip8.keys[6] = 0;
						break;

					case SDLK_r:
						chip8.keys[7] = 0;
						break;

					case SDLK_a:
						chip8.keys[8] = 0;
						break;

					case SDLK_s:
						chip8.keys[9] = 0;
						break;

					case SDLK_d:
						chip8.keys[10] = 0;
						break;

					case SDLK_f:
						chip8.keys[11] = 0;
						break;

					case SDLK_z:
						chip8.keys[12] = 0;
						break;

					case SDLK_x:
						chip8.keys[13] = 0;
						break;

					case SDLK_c:
						chip8.keys[14] = 0;
						break;

					case SDLK_v:
						chip8.keys[15] = 0;
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
	else if (argc > 2 && !strcmp(argv[2], "-disasm"))
	{
		Chip8 chip8;
		chip8.initialize();

		if (!chip8.load_ROM(path))
		{
			fputs("Error of loading rom", stderr);
			return 0;
		}
		
		while (!chip8.disasm());

	}
	;
}

char* concat(const char* str1, const char* str2)
{
	char* str3 = (char*) malloc(strlen(str1) + strlen(str2) + 1);
	strcpy_s(str3, strlen(str1) + strlen(str2) + 1, str1);
	strcat_s(str3, strlen(str1) + strlen(str2) + 1, str2);
	return str3;
}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
	{
		printf("Error", SDL_GetError());
		return false;
	}
		
	else
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("Error mix", Mix_GetError());
			return false;
		}
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
