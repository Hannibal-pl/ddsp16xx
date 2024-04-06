#include <string.h>
#include <libgen.h>

#include "ddsp.h"

struct CONTEXT context = {
	.file = NULL,
	.size = 0,
	.org_start = 0,
	.org_cur = 0,
	.is_bin = false,
	.indent = 0,
	.is_org = true,
	.loop_n = 0,
};

char *string_CONa[32] = {	"mi",
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
				"UNK1",
				"UNK2"};

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

char *string_Ra [64] = {		"r0",
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

void usage(char *binary) {
	printf("Usage: %s [bin|raw] filename\n", basename(binary));
}

void disassemble(void) {
	union INSTR word;

	printf("Program listing:\n\n");

	word.i = next_word();
	while(!feof(context.file)) {
//		oprintf("INSTR 0b%05b\n", word.t);
		switch (word.t) {
			case 0b00000:
			case 0b00001:
				instr_b00000(word.i);
				break;
			case 0b00010:
			case 0b00011:
				instr_b00010(word.i);
				break;
			case 0b00100:
				instr_b00100(word.i);
				break;
			case 0b00110:
				instr_b00110(word.i);
				break;
			case 0b00111:
				instr_b00111(word.i);
				break;
			case 0b01000:
				instr_b01000(word.i);
				break;
			case 0b01001:
				instr_b01001(word.i);
				break;
			case 0b01010:
				instr_b01010(word.i);
				break;
			case 0b01011:
				instr_b01001(word.i);
				break;
			case 0b01100:
				instr_b01100(word.i);
				break;
			case 0b01110:
				instr_b01110(word.i);
				break;
			case 0b01111:
				instr_b01111(word.i);
				break;
			case 0b10000:
			case 0b10001:
				instr_b10000(word.i);
				break;
			case 0b10010:
				instr_b10010(word.i);
				break;
			case 0b10011:
				instr_b10011(word.i);
				break;
			case 0b10100:
				instr_b10100(word.i);
				break;
			case 0b10110:
				instr_b10110(word.i);
				break;
			case 0b10111:
				instr_b10111(word.i);
				break;
			case 0b11000:
				instr_b11000(word.i);
				break;
			case 0b11010:
				instr_b11010(word.i);
				break;
			case 0b11100:
				instr_b11100(word.i);
				break;
			case 0b11111:
				instr_b11111(word.i);
				break;
			default:
				oprintf("unknown opcode 0b%05b param 0x%04x\n", word.t, word.p);
				break;
		}

		if (context.loop_n > 0) {
			if ((--context.loop_n) == 0) {
				context.indent--;
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
