#ifndef __DDSP_H__
#define __DDSP_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

struct CONTEXT {
	FILE *file;
	unsigned size;
	uint16_t org_start;
	uint16_t org_cur;
	bool is_bin;
	bool is_indent;
	bool is_org;
	unsigned loop_n;
};

union INSTR {
	uint16_t i;
	struct {
		unsigned p:11;
		unsigned t:5;
	};
};

union INSTR_F1 {
	uint16_t i;
	struct {
		unsigned yOp:2;
		unsigned yReg:2;
		unsigned x:1;
		unsigned f1:4;
		unsigned s:1;
		union {
			unsigned d:1;
			unsigned aT:1;
		};
		unsigned t:5;
	};
};

union INSTR_F4 {
	uint16_t i;
	struct {
		unsigned ja:11;
		unsigned t:5;
	};
};

//Global variables
extern struct CONTEXT context;

//Functions
//tools:
extern uint16_t get_page(void);
extern void oprintf(char* format, ...);
extern void oprintbuf(char *buf);
//fields:
extern unsigned field_F1(char *buf, unsigned bufsize, unsigned F1, unsigned D, unsigned S);
extern unsigned field_Y(char *buf, unsigned bufsize, unsigned reg, unsigned op);
//instructions:
extern void instr_b00000(uint16_t word);
extern void instr_b00100(uint16_t word);
extern void instr_b00110(uint16_t word);
extern void instr_b10000(uint16_t word);
extern void instr_b11100(uint16_t word);

#endif
