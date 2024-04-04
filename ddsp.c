#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>

bool is_bin = false;
FILE *file = NULL;
unsigned size = 0;
uint16_t org_start = 0;
uint16_t org_cur = 0;

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
	printf("0x%04X:    ", org_cur);
	vprintf(format, ap);
	va_end(ap);
}

void op_gotoB(uint16_t word) {
	unsigned b  = (word & (0x03F0)) >> 7;
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
