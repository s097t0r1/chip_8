#include <ctime>
#include <random>
#include "chip8.h"


int main(int argc, char* argv[])
{
	Chip8 chip8;
	srand(time(NULL));
	chip8.initialize();
	if (!chip8.load_ROM("test_rom.c8"))
	{
		return 0;
	}

	for (;;)
	{
		chip8.emulate_cycle();
		
		if (chip8.drawflag)
			chip8.drawn();
	}

	system("pause");
}


