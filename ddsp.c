#include <string.h>
#include <libgen.h>

#include "ddsp.h"

//bool is_bin = false;
//bool is_indent = false;
//bool is_org = true;
//int loop_n = 0;
//FILE *file = NULL;
//unsigned size = 0;
//uint16_t org_start = 0;
//uint16_t org_cur = 0;

struct CONTEXT context = {
	.file = NULL,
	.size = 0,
	.org_start = 0,
	.org_cur = 0,
	.is_bin = false,
	.is_indent = false,
	.is_org = true,
	.loop_n = 0,
};

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
	fread(&word, sizeof(word), 1, context.file);
	context.org_cur++;

	return word;
}

/*unsigned get_page(void) {
	return (org_cur & 0xF000);
}

void oprintf(char* format, ...) {
	va_list ap;
	va_start(ap, format);
	if (is_org) {
		printf("0x%04X:    ", org_cur);
	}
	if (is_indent) {
		printf("    ");
	}
	vprintf(format, ap);
	va_end(ap);
}*/

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

void decode_yY(bool X, unsigned Y, bool is_semi) {
	char reg = '0' + ((Y & 0xC) >> 2);

	if (is_semi){
		printf("; ");
	}

	printf("y%s = *r%c", X ? "l" : "", reg);
}

void decode_Yy(bool X, unsigned Y, bool is_semi) {
	char reg = '0' + ((Y & 0xC) >> 2);

	if (is_semi){
		printf("; ");
	}

	printf("*r%c = y%s", reg, X ? "l" : "");
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

void op_call_JA(uint16_t word) {
	oprintf("call 0x%04X", get_page() | (word & 0x0FFF));
}

void op_gotoB(uint16_t word) {
	unsigned b  = (word & (0x0700)) >> 8;
	switch (b) {
		case 0b000:
			oprintf("return [0x%02X]", (word & 0x00FF));
			break;
		case 0b001:
			oprintf("ireturn [0x%02X]", (word & 0x00FF));
			break;
		case 0b010:
			oprintf("goto pt [0x%02X]", (word & 0x00FF));
			break;
		case 0b011:
			oprintf("call pt [0x%02X]", (word & 0x00FF));
			break;
		default:
			oprintf("UNKNOWN gotoB 0b%03b [0x%02X]", b, (word & 0x00FF));
			break;
	}
}

void op_F1_Y(uint16_t word) {
	oprintf("");
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	decode_Y(word & 0x000F, true);
}

void op_F1_AT_Y(uint16_t word) {
	oprintf("");
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	decode_AT(word & 0x0400, word & 0x0010 ,word & 0x000F, true);
	decode_Y(word & 0x000F, true);
}

void op_R_Y(uint16_t word) {
	oprintf("%s = *r[%c]", string_R[(word & 0x02F0) >> 4], '0' + ((word & 0x000C) >> 2));
	decode_Y(word & 0x000F, true);
}

void op_Y_R(uint16_t word) {
	oprintf("*r[%c] = %s", '0' + ((word & 0x000C) >> 2), string_R[(word & 0x02F0) >> 4]);
	decode_Y(word & 0x000F, true);
}

void op_SR_IMM9(uint16_t word) {
	oprintf("%s = 0x%04X", string_SR[(word & 0x0E00) >> 9], word & 0x01FF);
}

void op_ifc_CON_F2(uint16_t word) {
	oprintf("test; c1++; if %s {", string_CON[word & 0x001F]);
	decode_F2((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	printf("; c2 = c1 }");
}

void op_if_CON_F2(uint16_t word) {
	oprintf("test; if %s {", string_CON[word & 0x001F]);
	decode_F2((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	printf("}");
}

void op_if_branch(uint16_t word) {
	uint16_t wordb;
	unsigned opcodeb;


	if (word & 0x0200){
		oprintf("icall");
		return;
	}
	oprintf("test; if %s { ", string_CON[word & 0x001F]);

	context.is_org = false;

	wordb = next_word();
	opcodeb = (wordb & 0xF800) >> 11;
	switch (opcodeb) {
		case 0b00000:
		case 0b00001:
			instr_b00000(wordb);
			break;
		case 0b11000:
			op_gotoB(wordb);
			break;
		default:
			printf("unknown branch 0x%04X", wordb);
			break;
	}

	context.is_org = true;

	printf(" }");
}

void op_R_IMM16(uint16_t word) {
	oprintf("%s", string_R[(word & 0x02F0) >> 4]);
	printf(" = 0x%04X", next_word());
}

void op_F1_y_Y(uint16_t word) {
	oprintf("");
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	decode_yY(word & 0x0010 ,word & 0x000F, true);
	decode_Y(word & 0x000F, true);
}

void op_F1_y_Y_x(uint16_t word) {
	oprintf("");
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	decode_Y(word & 0x000F, true);
	printf("; x = *pt; pt++%s", (word & 0x0010) ? "i" : "");
}

void op_F1_Y_aX(uint16_t word, char a) {
	char reg = '0' + ((word & 0x000C) >> 2);

	oprintf("*r%c = a%c%s", reg, a, (word & 0x0010) ? "l" : "");
	decode_Y(word & 0x000F, true);
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, true);
}

void op_R_aX(uint16_t word, char a) {
	oprintf("%s = a%c%s", string_R[(word & 0x03F0) >> 4], a, (word & 0x0400) ? "l" : "");
}

void op_a_R(uint16_t word) {
	oprintf("a%c%s = %s", (word & 0x0400) ? '1' : '0', (word & 0x0001) ? "l" : "", string_R[(word & 0x03F0) >> 4]);
}

void op_F1_Y_y(uint16_t word) {
	oprintf("");
	decode_F1((word & 0x01E0) >> 5, word & 0x0400, word & 0x0200, false);
	decode_Yy(word & 0x0010 ,word & 0x000F, true);
	decode_Y(word & 0x000F, true);
}

void op_do_K(uint16_t word) {
	uint16_t n = (word & 0x0780) >> 7;
	uint16_t k = word & 0x007F;
	if (n == 0) {
		oprintf("redo %i", k);
		return;
	}
	oprintf("do %i [%i]{", k, n);
	context.loop_n = n + 1;
	context.is_indent = true;
}

void disassemble(void) {
	union INSTR word;

	printf("Program listing:\n\n");

	word.i = next_word();
	while(!feof(context.file)) {
		switch (word.t) {
			case 0b00000:
			case 0b00001:
				instr_b00000(word.i);
				break;
			case 0b00010:
			case 0b00011:
				op_SR_IMM9(word.i);
				break;
			case 0b00100:
				instr_b00100(word.i);
//				op_F1_Y_aX(word.i, '1');
				break;
			case 0b00110:
//				op_F1_Y(word.i);
				instr_b00110(word.i);
				break;
			case 0b00111:
				op_F1_AT_Y(word.i);
				break;
			case 0b01000:
				op_a_R(word.i);
				break;
			case 0b01001:
				op_R_aX(word.i, '0');
				break;
			case 0b01010:
				op_R_IMM16(word.i);
				break;
			case 0b01011:
				op_R_aX(word.i, '1');
				break;
			case 0b01100:
				op_Y_R(word.i);
				break;
			case 0b01110:
				op_do_K(word.i);
				break;
			case 0b01111:
				op_R_Y(word.i);
				break;
			case 0b10000:
			case 0b10001:
				instr_b10000(word.i);
//				op_call_JA(word.i);
				break;
			case 0b10010:
				op_ifc_CON_F2(word.i);
				break;
			case 0b10011:
				op_if_CON_F2(word.i);
				break;
			case 0b10100:
				op_F1_Y_y(word.i);
				break;
			case 0b10111:
				op_F1_y_Y(word.i);
				break;
			case 0b11000:
				op_gotoB(word.i);
				break;
			case 0b11010:
				op_if_branch(word.i);
				break;
			case 0b11100:
				instr_b11100(word.i);
//				op_F1_Y_aX(word.i, '0');
				break;
			case 0b11111:
				op_F1_y_Y_x(word.i);
				break;
			default:
				oprintf("unknown opcode 0b%05b param 0x%04x", word.t, (word.i & 0x0FFF));
				break;
		}
		printf("\n");

		if (context.loop_n > 0) {
			if ((--context.loop_n) == 0) {
				context.is_indent = false;
				oprintf("}\n");
			}
		}

		word.i = next_word();
	}

	printf("\nProgram end:\n");
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		usage(argv[0]);
		return 1;
	}
	if (!strncmp(argv[1], "bin", 3)) {
		context.is_bin = true;
	} else if (!strncmp(argv[1], "raw", 3)) {
		context.is_bin = false;
	} else {
		printf("Unknown file format: %s\n\n", argv[1]);
		usage(argv[0]);
		return 1;
	}

	context.file = fopen(argv[2], "r");
	if (context.file == NULL) {
		printf("Input file not found: %s\n\n", argv[2]);
		usage(argv[0]);
		return 1;
	}

	fseek(context.file, 0, SEEK_END);
	context.size = ftell(context.file) >> 1;
	context.size -= context.is_bin ? 3 : 0;

	if (context.is_bin) {
		fseek(context.file, 0, SEEK_SET);
		fread(&context.org_start, sizeof(context.org_start), 1, context.file);
		context.org_cur = context.org_start - 1;
	}

	fseek(context.file, context.is_bin ? 6 : 0, SEEK_SET);

	printf("Program size is %u words long orgins is 0x%04X\n\n", context.size, context.org_start);
	disassemble();

	fclose(context.file);
	return 0;
}
