#ifndef HELLO
#define HELLO

#include <cstdint>

enum {
	V0 = 0,
	V1,
	V2,
	V3,
	V4,
	V5,
	V6,
	V7,
	V8,
	V9,
	VA,
	VB,
	VC,
	VD,
	VF,
	V_COUNT
};

class Chip8
{
private:
	uint16_t I;
	uint16_t pc;
	uint16_t instr;
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint8_t stack_pointer;
	uint16_t stack[16];
	uint8_t keys[16];
	uint8_t disp[64 * 32];
	uint8_t registers[16];
	uint8_t memory[4096];

public:
	bool drawflag;


	void initialize();
	bool load_ROM(const char*);
	void drawn();
	void emulate_cycle();
};




#endif