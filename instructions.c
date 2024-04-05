#include <string.h>

#include "ddsp.h"

void instr_b00000(uint16_t word) {
	union INSTR_F4 instr;

	instr.i = word;
	oprintf("goto 0x%04X\n", get_page() | instr.ja);
}

void instr_b00100(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintf("*r%i = a1%s\n", instr.yReg, instr.x ? "l" : "");

	oprintbuf(buf_F1);
	oprintbuf(buf_Y);
}

void instr_b00110(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	if (length == 0) { // both nop
		oprintf("nop\n");
		return;
	}

	oprintbuf(buf_F1);
	oprintbuf(buf_Y);
}

void instr_b10000(uint16_t word) {
	union INSTR_F4 instr;

	instr.i = word;
	oprintf("call 0x%04X\n", get_page() | instr.ja);
}


void instr_b11100(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintf("*r%i = a1%s\n", instr.yReg, instr.x ? "l" : "");

	oprintbuf(buf_F1);
	oprintbuf(buf_Y);
}

