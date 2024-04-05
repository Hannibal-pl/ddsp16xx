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
		unsigned y:4;
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
//instructions:
extern void instr_b00000(uint16_t word);

#endif
