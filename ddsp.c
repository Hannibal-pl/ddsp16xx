#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>

bool is_bin = false;
bool is_indent = false;
FILE *file = NULL;
unsigned size = 0;
uint16_t org_start = 0;
uint16_t org_cur = 0;

char *string_CON[32] = {	"mi",
				"pl",
				"eq",
				"ne",
				"lvs",
				"lvc",
				"mvs",
				"mvc",
				"heads",
				"tails",
				"c0ge",
				"c0lt",
				"c1ge",
				"c1lt",
				"true",
				"false",
				"gt",
				"le",
				"allt",
				"allf",
				"somet",
				"somef",
				"oddp",
				"evenp",
				"mns1",
				"nmns1",
				"npint",
				"njint",
				"lock",
				"ebusy",
				"UNK",
				"UNK"};

char *string_F1 [16] = {	"p = x * y; a%1$i = p",
				"p = x * y; a%1$i = a%2$i + p",
				"p = x * y",
				"p = x * y; a%1$i = a%2$i - p",
				"a%1$i = p",
				"a%1$i = a%2$i + p",
				"nop",
				"a%1$i = a%2$i - p",
				"a%1$i = a%2$i | y",
				"a%1$i = a%2$i ^ y",
				"a%2$i & y",
				"a%2$i - y",
				"a%1$i = y",
				"a%1$i = a%2$i + y",
				"a%1$i = a%2$i & y",
				"a%1$i = a%2$i - y"};

char *string_F2[16] = {		"a%1$i = a%2$i >> 1",
				"a%1$i = a%2$i << 1",
				"a%1$i = a%2$i >> 4",
				"a%1$i = a%2$i >> 4",
				"a%1$i = a%2$i >> 8",
				"a%1$i = a%2$i >> 8",
				"a%1$i = a%2$i >> 16",
				"a%1$i = a%2$i >> 16",
				"a%1$i = p",
				"a%1$ih = a%2$ih + 1",
				"a%1$i = ~a%2$i",
				"a%1$ih = rnd(a%2$i)",
				"a%1$i = y",
				"a%1$i = a%2$i + 1",
				"a%1$i = a%2$i",
				"a%1$i = -a%2$i"};

char *string_R [64] = {		"r0",
				"r1",
				"r2",
				"r3",
				"j",
				"k",
				"rb",
				"re",
				"pt",
				"pr",
				"p1",
				"i",
				"p",
				"pl",
				"pllc",
				"UNK",
				"x",
				"y",
				"yl",
				"auc",
				"psw",
				"c0",
				"c1",
				"c2",
				"sioc",
				"srta",
				"sdx",
				"tdms",
				"phifc",
				"pdx0",
				"UNK",
				"ybase",
				"inc",
				"ins",
				"sdx2",
				"saddx",
				"cloop",
				"mwait",
				"saddx2",
				"sioc2",
				"cbit",
				"sbit",
				"ioc",
				"jtag",
				"UNK",
				"UNK",
				"UNK",
				"eir",
				"a0",
				"a0l",
				"a1",
				"a1l",
				"timerc",
				"timer0",
				"tdms2",
				"srta2",
				"powerc",
				"edr",
				"ar0",
				"ar1",
				"ar2",
				"ar3",
				"ear",
				"alf"};

char *string_SR[8] = {		"j",
				"k",
				"rb",
				"re",
				"r0",
				"r1",
				"r2",
				"r3"};

void usage(char *binary) {
	printf("Usage: %s [bin|raw] filename\n", basename(binary));
}

uint16_t next_word(void) {
	uint16_t word;
	fread(&word, sizeof(word), 1, file);
	org_cur ++;

	return word;
}

unsigned get_page(void) {
	return (org_cur & 0xF000);
}

void oprintf(char* format, ...) {
	va_list ap;
	va_start(ap, format);
	if (is_indent) {
		printf("\t");
	}
	printf("0x%04X:    ", org_cur);
	vprintf(format, ap);
	va_end(ap);
}

void decode_F1(unsigned F1, bool D, bool S, bool is_semi) {
	if (is_semi){
		printf("; ");
	}

	printf(string_F1[F1 & 0xF], D ? 1 : 0, S ? 1 : 0);
}

void decode_F2(unsigned F2, bool D, bool S, bool is_semi) {
	if (is_semi){
		printf("; ");
	}

	printf(string_F2[F2 & 0xF], D ? 1 : 0, S ? 1 : 0);
}

void decode_AT(bool D, bool X, unsigned Y, bool is_semi) {
	char reg = '0' + ((Y & 0xC) >> 2);

	if (is_semi){
		printf("; ");
	}

	printf("a%s%s = *r%c",D ? "0" : "1" ,X ? "l" : "", reg);
}

void decode_Y(unsigned Y, bool is_semi) {
	char reg = '0' + ((Y & 0xC) >> 2);

	if (is_semi){
		printf("; ");
	}

	switch (Y & 0x3) {
		case 0b00:
			printf("nop");
			break;
		case 0b01:
			printf("*r%c++", reg);
			break;
		case 0b10:
			printf("*r%c--", reg);
			break;
		case 0b11:
			printf("*r%c++j", reg);
			break;
	}
}

void op_gotoB(uint16_t word) {
	unsigned b  = (word & (0x0700)) >> 8;
	switch (b) {
		case 0b000:
			oprintf("return\n");
			break;
		case 0b001:
			oprintf("ireturn\n");
			break;
		case 0b010:
			oprintf("goto pt\n");
			break;
		case 0b011:
			oprintf("call pt\n");
			break;
		default:
			oprintf("UNKNOWN gotoB 0b%03b\n", b);
			break;
	}
}

void op_F1_Y(uint16_t word) {
	oprintf("");
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	decode_Y(word & 0x000F, true);
	printf("\n");
}

void op_F1_AT_Y(uint16_t word) {
	oprintf("");
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	decode_AT(word & 0x0400, word & 0x0010 ,word & 0x000F, true);
	decode_Y(word & 0x000F, true);
	printf("\n");
}

void op_R_Y(uint16_t word) {
	oprintf("%s = *r[%c]", string_R[(word & 0x02F0) >> 4], '0' + ((word & 0x000C) >> 2));
	decode_Y(word & 0x000F, true);
	printf("\n");
}

void op_SR_IMM9(uint16_t word) {
	oprintf("%s = 0x%04X\n", string_SR[(word & 0x0E00) >> 9], word & 0x01FF);
}

void op_ifc_CON_F2(uint16_t word) {
	oprintf("test; c1++; if %s {", string_CON[word & 0x001F]);
	decode_F2((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	printf("; c2 = c1 }\n");
}

void disassemble(void) {
	uint16_t word;
	unsigned opcode;

	printf("Program listing:\n\n");

	word = next_word();
	while(!feof(file)) {
		opcode = (word & 0xF800) >> 11;

		switch (opcode) {
			case 0b00000:
			case 0b00001:
				oprintf("goto 0x%04x\n", get_page() | (word & 0x0FFF));
				break;
			case 0b00010:
			case 0b00011:
				op_SR_IMM9(word);
				break;
			case 0b00110:
				op_F1_Y(word);
				break;
			case 0b00111:
				op_F1_AT_Y(word);
				break;
			case 0b01111:
				op_R_Y(word);
				break;
			case 0b10010:
				op_ifc_CON_F2(word);
				break;
			case 0b11000:
				op_gotoB(word);
				break;
			default:
				oprintf("unknown opcode 0b%05b param 0x%04x\n", opcode, (word & 0x0FFF));
				break;
		}
		word = next_word();
	}

	printf("\nProgram end:\n");
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		usage(argv[0]);
		return 1;
	}
	if (!strncmp(argv[1], "bin", 3)) {
		is_bin = true;
	} else if (!strncmp(argv[1], "raw", 3)) {
		is_bin = false;
	} else {
		printf("Unknown file format: %s\n\n", argv[1]);
		usage(argv[0]);
		return 1;
	}

	file = fopen(argv[2], "r");
	if (file == NULL) {
		printf("Input file not found: %s\n\n", argv[2]);
		usage(argv[0]);
		return 1;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file) >> 1;
	size -= is_bin ? 3 : 0;

	if (is_bin) {
		fseek(file, 0, SEEK_SET);
		fread(&org_start, sizeof(org_start), 1, file);
		org_cur = org_start;
	}

	fseek(file, is_bin ? 6 : 0, SEEK_SET);

	printf("Program size is %u words long orgins is 0x%04X\n\n", size, org_start);
	disassemble();

	fclose(file);
	return 0;
}
