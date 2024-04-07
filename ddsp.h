#ifndef __DDSP_H__
#define __DDSP_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define VERSION			"0.1.0"

struct CONTEXT {
	FILE *file;
	unsigned size;
	uint16_t org_start;
	uint16_t org_cur;
	bool is_bin;
	bool is_crc;
	unsigned indent;
	bool is_org;
	bool is_org_cmdline;
	bool check_crc;
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
		unsigned yzOp:2;
		unsigned yzReg:2;
		unsigned x:1;
		unsigned f1:4;
		unsigned s:1;
		unsigned d:1;
		unsigned t:5;
	};
};

union INSTR_F3 {
	uint16_t i;
	struct {
		unsigned con:5;
		unsigned f2:4;
		unsigned s:1;
		unsigned d:1;
		unsigned t:5;
	};
};

union INSTR_F3a {
	uint16_t i;
	struct {
		unsigned bit0:1;
		unsigned zero:1;
		unsigned aT:1;
		unsigned src2:2;
		unsigned f3:4;
		unsigned s:1;
		unsigned d:1;
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

union INSTR_F5 {
	uint16_t i;
	struct {
		unsigned reserved:8;
		unsigned b:3;
		unsigned t:5;
	};
};

union INSTR_F6 {
	uint16_t i;
	struct {
		unsigned con:5;
		unsigned reserved:5;
		unsigned si:1;
		unsigned t:5;
	};
};

union INSTR_F7 {
	uint16_t i;
	struct {
		unsigned yzOp:2;
		unsigned yzReg:2;
		unsigned r:6;
		unsigned aT:1;
		unsigned t:5;
	};
};

union INSTR_F7a {
	uint16_t i;
	struct {
		unsigned x:1;
		unsigned reserved:3;
		unsigned r:6;
		unsigned aT:1;
		unsigned t:5;
	};
};

union INSTR_F7b {
	uint16_t i;
	struct {
		unsigned reserved:4;
		unsigned r:6;
		unsigned x:1;
		unsigned t1:1;
		unsigned s:1;
		unsigned t2:3;
	};
};

union INSTR_F8 {
	uint16_t i;
	struct {
		unsigned reserved:4;
		unsigned r:6;
		unsigned d:1;
		unsigned t:5;
	};
};

union INSTR_F9 {
	uint16_t i;
	struct {
		unsigned im9:9;
		unsigned sr:3;
		unsigned t4:4;
	};
};

union INSTR_F10 {
	uint16_t i;
	struct {
		unsigned k:7;
		unsigned n:4;
		unsigned t:5;
	};
};


//Global variables
extern struct CONTEXT context;

//Functions
//tools:
extern uint16_t next_word(void);
extern uint16_t get_page(void);
extern void oprintf(char* format, ...);
extern void oprintbuf(char *buf);
extern void check_crc(void);
//fields:
extern unsigned field_B(char *buf, unsigned bufsize, unsigned B);
extern char * field_CON(unsigned con);
extern unsigned field_F1(char *buf, unsigned bufsize, unsigned F1, unsigned D, unsigned S);
extern unsigned field_F2(char *buf, unsigned bufsize, unsigned F2, unsigned D, unsigned S);
extern unsigned field_F3(char *buf, unsigned bufsize, unsigned F3, unsigned D, unsigned S, char *hl);
extern char * field_R(unsigned r);
extern char * field_SR(unsigned sr);
extern unsigned field_Y(char *buf, unsigned bufsize, unsigned reg, unsigned op);
extern unsigned field_Z1(char *buf, unsigned bufsize, unsigned reg, unsigned op);
extern unsigned field_Z2(char *buf, unsigned bufsize, unsigned reg, unsigned op);
//instructions:
extern void instr_b00000(uint16_t word);
extern void instr_b00010(uint16_t word);
extern void instr_b00100(uint16_t word);
extern void instr_b00101(uint16_t word);
extern void instr_b00110(uint16_t word);
extern void instr_b00111(uint16_t word);
extern void instr_b01000(uint16_t word);
extern void instr_b01001(uint16_t word);
extern void instr_b01010(uint16_t word);
extern void instr_b01100(uint16_t word);
extern void instr_b01101(uint16_t word);
extern void instr_b01110(uint16_t word);
extern void instr_b01111(uint16_t word);
extern void instr_b10000(uint16_t word);
extern void instr_b10010(uint16_t word);
extern void instr_b10011(uint16_t word);
extern void instr_b10100(uint16_t word);
extern void instr_b10101(uint16_t word);
extern void instr_b10110(uint16_t word);
extern void instr_b10111(uint16_t word);
extern void instr_b11000(uint16_t word);
extern void instr_b11010(uint16_t word);
extern void instr_b11100(uint16_t word);
extern void instr_b11101(uint16_t word);
extern void instr_b11111(uint16_t word);
//cmdline:
extern void parseparams(int argc, char *argv[]);

#endif
