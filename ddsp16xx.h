/*
Copyright (c) 2024, Konrad Rzepecki <hannibal@astral.lodz.pl>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OFMERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef __DDSP_H__
#define __DDSP_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define VERSION			"0.9.0"

#define CPU_NAME_LENGTH		16
#define CPU_DESC_LENGTH		96

#define CPU_DSP_GENERIC		0
#define CPU_DSP_1604		1
#define CPU_DSP_1606		2
#define CPU_DSP_1610		3
#define CPU_DSP_1611		4
#define CPU_DSP_1617		5
#define CPU_DSP_1618		6
#define CPU_DSP_1627		7
#define CPU_DSP_1628		8
#define CPU_DSP_1629		9
#define CPU_DSP_164X		10
#define CPU_DSP_1650		11
#define CPU_DSP_MAX		11
#define CPU_DSP_COUNT		(CPU_DSP_MAX + 1)


struct CONTEXT {
	FILE *file;
	char *filename;
	unsigned size;
	int32_t org_start;
	int32_t org_cur;
	int32_t start;
	bool is_bin;
	bool is_crc;
	unsigned indent;
	bool is_org;
	bool is_org_cmdline;
	bool is_start_cmdline;
	bool check_crc;
	unsigned loop_n;
	bool is_hidden;
	unsigned cpu;
	bool is_single;
	uint32_t single;
};

struct CPU {
	unsigned id;
	char name[CPU_NAME_LENGTH];
	char description[CPU_DESC_LENGTH];
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

union INSTR_F3b {
	uint16_t i;
	struct {
		unsigned m:2;
		unsigned bmu:7;
		unsigned s:1;
		unsigned d:1;
		unsigned t:5;
	};
};

union INSTR_F4 {
	uint16_t i;
	struct {
		unsigned ja:12;
		unsigned t4:4;
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

union INSTR_F9a {
	uint16_t i;
	struct {
		unsigned offset:5;
		unsigned bit5:1;
		unsigned dr:4;
		unsigned rw:1;
		unsigned t:5;
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
extern bool is_CON_true(unsigned con);
extern bool is_CON_false(unsigned con);
extern void fix_CON(unsigned cpu);
extern char * field_DR(unsigned dr);
extern unsigned field_F1(char *buf, unsigned bufsize, unsigned F1, unsigned D, unsigned S);
extern unsigned field_F2(char *buf, unsigned bufsize, unsigned F2, unsigned D, unsigned S);
extern unsigned field_F3(char *buf, unsigned bufsize, unsigned F3, unsigned D, unsigned S, char *hl);
extern char * field_R(unsigned r);
extern void fix_R(unsigned cpu);
extern char * field_SR(unsigned sr);
extern unsigned field_Y(char *buf, unsigned bufsize, unsigned reg, unsigned op);
extern unsigned field_Z1(char *buf, unsigned bufsize, unsigned reg, unsigned op);
extern unsigned field_Z2(char *buf, unsigned bufsize, unsigned reg, unsigned op);
//instructions:
extern void instr_data(uint16_t word);
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
extern void instr_b11001(uint16_t word);
extern void instr_b11010(uint16_t word);
extern void instr_b11011(uint16_t word);
extern void instr_b11100(uint16_t word);
extern void instr_b11101(uint16_t word);
extern void instr_b11110(uint16_t word);
extern void instr_b11111(uint16_t word);
//cmdline:
extern void parseparams(int argc, char *argv[]);
//cpu
extern void list_cpu(void);
extern void fix_cpu(unsigned cpu);
extern struct CPU * get_cpu(unsigned cpu);
extern bool select_cpu(char *name);

#endif
