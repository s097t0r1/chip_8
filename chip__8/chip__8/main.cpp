#define _CRT_SECURE_NO_WARNINGS
#include "chip-8.h"
#include <random>
#include <ctime>
#include <fstream>


void emulate_cycle();
void initialize();
bool loadApplication(const char*);
void drawn();

int main(int argc, char* argv[])
{
	srand(time(NULL));
	initialize();
	memory[0x200] = 0x71;
	memory[0x201] = 0x0F;
	memory[0x202] = 0xF1;
	memory[0x203] = 0x29;
	memory[0x204] = 0xD0;
	memory[0x205] = 0x05;

	for (;;)
	{
		emulate_cycle();
		
		if (drawflag)
			drawn();
	}
}

void initialize()
{
	pc = 0x200;
	instr = 0;
	I = 0;
	stack_pointer = 0;
	drawflag = 0;

	//clear display
	for (int i = 0; i < 64 * 32; i++)
		disp[i] = 0;

	//clear memory
	for (uint16_t i = 0x200; i < 0xFFF; i++)
		memory[i] = 0;

	//load fonset
	for (int i = 0; i < 80; i++)
		memory[i] = chip8_fontset[i];

	//clear registers
	for (int i = 0; i < V_COUNT; i++)
		registers[i] = 0;

	//clear stack
	for (int i = 0; i < 16; i++)
		stack[i] = 0;
}

void emulate_cycle()
{
	instr = (memory[pc] << 8) | memory[pc + 1];
	uint16_t nnn = instr & 0x0FFF;
	uint8_t n    =  instr & 0x000F;
	uint8_t x    = (instr >> 8) & 0xF;
	uint8_t y    = (instr >> 4) & 0xF;
	uint8_t kk   = instr & 0x00FF;

	switch (instr & 0xF000)
	{
	case 0x0000:
		switch (instr)
		{
		case 0x00E0:
			for (int i = 0; i < 64 * 32; i++)
				disp[i] = 0;
			break;
		case 0x00EE:
			pc = stack[stack_pointer];
			pc -= 2;
			stack_pointer--;
			break;

		default:
			break;
		}
		break;
	case 0x1000:
		pc = nnn;
		break;
	case 0x2000:
		stack_pointer++;
		stack[stack_pointer] = pc;
		pc = nnn;
		break;
	case 0x3000:
		if (registers[x] == kk)
			pc += 2;
		break;
	case 0x4000:
		if (registers[x] != kk)
			pc += 2;
		break;
	case 0x5000:
		if (registers[x] == registers[y])
			pc += 2;
		break;
	case 0x6000:
		registers[x] = kk;
		break;
	case 0x7000:
		registers[x] += kk;
		break;
	case 0x8000:
		switch (instr & 0x000F)
		{
		case 0x0001:
			registers[x] |= registers[y];
			break;
		case 0x0002:
			registers[x] &= registers[y];
			break;
		case 0x0003:
			registers[x] ^= registers[y];
			break;
		case 0x0004:
			if (registers[x] > (0xFF - registers[y]))
				registers[VF] = 1;
			else
				registers[VF] = 0;
			registers[x] += registers[y];
			break;
		case 0x0005:
			if (registers[x] > registers[y])
			{
				registers[VF] &= 0;
				registers[VF] = 1;
			}
			else
			{
				registers[VF] &= 0;
				registers[x] -= registers[y];
			}
			break;
		case 0x0006:
			if (registers[x] & 0x1)
			{
				registers[VF] &= 0;
				registers[VF] = 1;
			}
			else
			{
				registers[VF] &= 0;
				registers[x] = registers[x] >> 1; // Divide by 2
			}
			break;
		case 0x0007:
			if (registers[y] > registers[x])
			{
				registers[VF] &= 0;
				registers[VF] = 1;
			}
			else
			{
				registers[VF] &= 0;
				registers[x] = registers[y] - registers[x];
			}
			break;
		case 0x000E:
			if ((registers[x] >> 7) & 0x1)
			{
				registers[VF] &= 0;
				registers[VF] = 1;
			}
			else
			{
				registers[VF] &= 0;
				registers[x] = registers[x] << 1; // Multiple by 2
			}
			break;
		default:
			break;
		}
		break;

	case 0x9000:
		if (registers[x] != registers[y])
			pc += 2;
		break;

	case 0xA000:
		I = nnn;
		break;

	case 0xB000:
		pc = registers[V0] + nnn;
		break;

	case 0xC000:
	{
		uint8_t source = rand() % 256;
		registers[x] = source & kk;
		break;
	}
		
		
	case 0xD000:
	{
		uint8_t x_coord = registers[x], y_coord = registers[y];
		for (uint8_t yline = 0; yline < n; yline++)
		{
			uint8_t mask = memory[I + yline];
			for (uint8_t xline = 0; xline < 8; xline++)
			{
				if ((mask & (0x80 >> xline)) > 0)
				{
					if (disp[x + xline + ((y + yline) * 64)] == 1)
						registers[VF] = 1;
					disp[x + xline + ((y + yline) * 64)] ^= 1;
				}
				else
					disp[x + xline + ((y + yline) * 64)] ^= 0;
			}
		}

		drawflag = true;
		break;

	}
	
	case 0xE000:
	{

			switch (instr & 0x00FF)
			{
			case 0x009E:
				if (keys[registers[x]] == 1)
					pc += 2;
				break;

			case 0x00A1:
				if (keys[registers[x]] == 0)
					pc += 2;

			}
			break;
	}
	case 0xF000:
	{
		switch (instr & 0x00FF)
		{
		case 0x0007:
			registers[x] = delay_timer;
			break;
		case 0x000A:
			// Wait for a key press, store the value of the key in Vx.
			break;
		case 0x0015:
			delay_timer = registers[x];
			break;
		case 0x0018:
			sound_timer = registers[x];
			break;
		case 0x001E:
			I += registers[x];
			break;
		case 0x0029:
			I = registers[x] * 5;
			break;
		case 0x0033:
			memory[I] = registers[x] / 100;
			memory[I + 1] = (registers[x] / 10) % 10;
			memory[I + 2] = registers[x] % 10;
			break;
		case 0x0055:
			for (int i = 0; i < x; i++)
				memory[I + i] = registers[x];
			break;
		case 0x0065:
			for (int i = 0; i < x; i++)
				registers[i] = memory[I + i];
			break;

		}

		break;
	}
	}

	pc += 2;
}

void drawn()
{
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (disp[(i * 64) + j])
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
}


bool loadApplication(const char * filename)
{
	initialize();
	printf("Loading: %s\n", filename);

	// Open file
	FILE * pFile;
	fopen_s(&pFile,filename, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		return false;
	}

	// Check file size
	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);

	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		return false;
	}

	// Copy buffer to Chip8 memory
	if ((4096 - 512) > lSize)
	{
		for (int i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for memory");

	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
}