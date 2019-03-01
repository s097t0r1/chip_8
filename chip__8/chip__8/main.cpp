#include "chip-8.h"
#include <ctime>
#include <cstdio>
#include <random>


void emulate_cycle();
void initialize();
bool load_ROM(const char*);
void drawn();

int main(int argc, char* argv[])
{
	srand(time(NULL));
	initialize();
	if (!load_ROM("test_rom.c8"))
	{
		return 0 ;
	}
	for (int i = 512; i < 4096; i++)
		printf("%x \n", memory[i]);

	for (;;)
	{
		emulate_cycle();
		
		if (drawflag)
			drawn();
	}

	system("pause");
}


