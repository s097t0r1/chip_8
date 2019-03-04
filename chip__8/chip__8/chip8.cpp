#include "chip8.h"
#include <random>
#include <cstdint>
#include <SDL.h>



uint8_t chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};




void Chip8::initialize()
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
	
	//determine pixels
	for (int h = 0; h < 32; h++)
	{
		for (int w = 0; w < 64; w++)
		{
			pixels[h * 64 + w] = { w * 8, h * 8, 8, 8 };
		}
	}

}

void Chip8::emulate_cycle()
{
	instr = (memory[pc] << 8) | (memory[pc + 1]);
	uint16_t nnn = instr & 0x0FFF;
	uint8_t n = instr & 0x000F;
	uint8_t x = (instr >> 8) & 0xF;
	uint8_t y = (instr >> 4) & 0xF;
	uint8_t kk = instr & 0x00FF;

	switch (instr & 0xF000)
	{
	case 0x0000:
		switch (instr)
		{
		case 0x00E0: // Clear the display
			for (int i = 0; i < 64 * 32; i++)
				disp[i] = 0;
			break;
		case 0x00EE: // Return from subroutine
			stack_pointer--;
			pc = stack[stack_pointer];
			break;

		default:
			break;
		}
		break;
	case 0x1000: // Jump to location nnn
		pc = nnn;
		pc -= 2;
		break;
	case 0x2000: // Call subroutine to nnn
		stack[stack_pointer] = pc;
		stack_pointer++;
		pc = nnn;
		pc -= 2;
		break;
	case 0x3000: // Skip next instruction if Vx == kk
		if (registers[x] == kk)
			pc += 2;
		break;
	case 0x4000: // Skip next instruction if Vx != kk
		if (registers[x] != kk)
			pc += 2;
		break;
	case 0x5000: //Skip next instruction if Vx == Vy
		if (registers[x] == registers[y])
			pc += 2;
		break;
	case 0x6000: // Set Vx = kk
		registers[x] = kk;
		break;
	case 0x7000: // Set Vx = Vx + kk
		registers[x] += kk;
		break;
	case 0x8000: 
		switch (instr & 0x000F)
		{
		case 0x0000: // Set Vx = Vy
			registers[x] = registers[y];
			break;

		case 0x0001: // Set Vx = Vx OR Vy
			registers[x] |= registers[y];
			break;
		case 0x0002: // Set Vx = Vx AND Vy
			registers[x] &= registers[y];
			break;
		case 0x0003: // Set Vx = Vx XOR Vy
			registers[x] ^= registers[y];
			break;
		case 0x0004: // Set Vx = Vx + Vy, set VF = carry
			if (registers[x] > (0xFF - registers[y]))
				registers[VF] = 1;
			else
				registers[VF] = 0;
			registers[x] += registers[y];
			break;
		case 0x0005: // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
			if (registers[x] > registers[y])
			{
				registers[VF] = 1;
			}
			else
			{
				registers[VF] = 0;
				registers[x] -= registers[y];
			}
			break;
		case 0x0006: //If the least - significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
			if (registers[x] & 0x1)
				registers[VF] = 1;
			else
			{
				registers[VF] &= 0;
				registers[x] = registers[x] >> 1; // Divide by 2
			}
			break;
		case 0x0007: // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
			if (registers[y] > registers[x])
			{
				registers[VF] = 1;
			}
			else
			{
				registers[VF] = 0;
				registers[x] = registers[y] - registers[x];
			}
			break;
		case 0x000E: // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
			if ((registers[x] >> 7) & 0x1)
			{
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

	case 0x9000: // Skip next instruction if Vx != Vy
		if (registers[x] != registers[y])
			pc += 2;
		break;

	case 0xA000: // Set I = nnn
		I = nnn;
		break;

	case 0xB000: // Jump to location nnn + V0
		pc = registers[V0] + nnn;
		pc -= 2;
		break;

	case 0xC000: // Set Vx = random byte AND kk
	{
		uint8_t source = rand() % 255;
		registers[x] = source & kk;
		break;
	}


	case 0xD000: // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
	{
		uint8_t x_coord = registers[x], y_coord = registers[y];
		uint8_t mask;
		for (uint8_t yline = 0; yline < n; yline++)
		{
			mask = memory[I + yline];
			for (uint8_t xline = 0; xline < 8; xline++)
			{
				if ((mask & (0x80 >> xline)) != 0)
				{
					if (disp[x + xline + ((y_coord + yline) * 64)] == 1)
						registers[VF] = 1;
					disp[x_coord + xline + ((y_coord + yline) * 64)] ^= 1;
				}
				//else
					//disp[x_coord + xline + ((y_coord + yline) * 64)] ^= 0;
			}
		}

		drawflag = true;
		break;

	}

	case 0xE000: //
	{

		switch (instr & 0x00FF)
		{
		case 0x009E: // Skip next instruction if key with the value of Vx is pressed
			if (keys[registers[x]] == 1)
				pc += 2;
			break;

		case 0x00A1: // Skip next instruction if key with the value of Vx is not pressed 
			if (keys[registers[x]] == 0)
				pc += 2;

		}
		break;
	}
	case 0xF000:
	{
		switch (instr & 0x00FF)
		{
		case 0x0007: // Set Vx = delay timer value
			registers[x] = delay_timer;
			break;
		case 0x000A:
			// Wait for a key press, store the value of the key in Vx.
			break;
		case 0x0015: // Set delay timer = Vx
			delay_timer = registers[x];
			break;
		case 0x0018: // Set sound timer = Vx
			sound_timer = registers[x];
			break;
		case 0x001E: // Set I = I + Vx
			I += registers[x];
			break;
		case 0x0029: // Set I = location of sprite for digit Vx
			I = registers[x] * 5;
			break;
		case 0x0033: // Store BCD representation of Vx in memory locations I, I+1, and I+2.
			memory[I] = registers[x] / 100;
			memory[I + 1] = (registers[x] / 10) % 10;
			memory[I + 2] = (registers[x] % 100) % 10;
			break;
		case 0x0055: // Store registers V0 through Vx in memory starting at location I

			for (int i = 0; i <= x; i++)
				memory[I + i] = registers[i];
			break;
		case 0x0065: // Read registers V0 through Vx from memory starting at location I
			for (int i = 0; i <= x; i++)
				registers[i] = memory[I + i];
			break;

		}

		break;
	}
	}
	pc += 2;

	if (delay_timer > 0)
		delay_timer--;
	if (sound_timer > 0)
		sound_timer > 0;
}


void Chip8::drawn(SDL_Renderer* grenderer)
{
	 for (int h = 0; h < 32; h++)
		{
			for (int w = 0; w < 64; w++)
			{
				if (disp[h * 64 + w])
				{
					SDL_RenderFillRect(grenderer, &pixels[h * 64 + w]);
				}
			}
		}

	//SDL_Rect rect = { 0,0, 64, 32 };
	//SDL_RenderDrawRect(grenderer, pixels);
}



bool Chip8::load_ROM(const char* rom_title)
{
	FILE* rom;

	fopen_s(&rom, rom_title, "rb");

	if (rom == NULL)
	{
		fputs("Error of open ROM", stderr);
		return false;
	}

	//Take size of rom
	fseek(rom, 0, SEEK_END);
	long size = ftell(rom);

	fseek(rom, 0, SEEK_SET);

	uint8_t* buffer = (uint8_t*)malloc(size);

	if (buffer == NULL)
	{
		fputs("Error of allocate of memory", stderr);
		return false;
	}

	long success_byte = fread(buffer, 1, size, rom);

	if (success_byte != size)
	{
		fputs("Error of copy file to buffer", stderr);
		return false;
	}

	if ((4096 - 512) > size)
	{
		for (int i = 0; i < size; i++)
			memory[i + 512] = buffer[i];
	}

	return true;
}

void Chip8::drawn_debug()
{
	for (int h = 0; h < 32; h++)
	{
		for (int w = 0; w < 64; w++)
		{
			if (disp[h * 64 + w])
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}