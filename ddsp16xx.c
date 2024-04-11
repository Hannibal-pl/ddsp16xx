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

#include <string.h>
#include <libgen.h>

#include "ddsp16xx.h"

struct CONTEXT context = {
	.file = NULL,
	.size = 0,
	.org_start = 0,
	.org_cur = 0,
	.start = 0,
	.is_bin = true,
	.is_crc = false,
	.indent = 0,
	.is_org = true,
	.is_org_cmdline = false,
	.is_start_cmdline = false,
	.check_crc = false,
	.loop_n = 0,
	.is_hidden = true
};

void disassemble(void) {
	union INSTR word;

	printf("Program listing:\n\n");

	word.i = next_word();
	while(!feof(context.file)) {
//		oprintf("INSTR 0b%05b\n", word.t);

		if (context.is_start_cmdline && (context.start > context.org_cur)) {
			instr_data(word.i);
			goto next;
		}

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
			case 0b00101:
				instr_b00101(word.i);
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
			case 0b01101:
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
			case 0b10101:
				instr_b10101(word.i);
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
			case 0b11001:
				instr_b11001(word.i);
				break;
			case 0b11010:
				instr_b11010(word.i);
				break;
			case 0b11011:
				instr_b11011(word.i);
				break;
			case 0b11100:
				instr_b11100(word.i);
				break;
			case 0b11101:
				instr_b11101(word.i);
				break;
			case 0b11110:
				instr_b11110(word.i);
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

		next: word.i = next_word();
	}

	printf("\nProgram end:\n");
}

int main(int argc, char *argv[]) {
	parseparams(argc, argv);

	fseek(context.file, 0, SEEK_END);
	context.size = ftell(context.file) >> 1;
	context.size -= context.is_bin ? 3 : 0;

	if (context.is_bin && !context.is_org_cmdline) {
		fseek(context.file, 0, SEEK_SET);
		fread(&context.org_start, sizeof(context.org_start), 1, context.file);
		context.org_cur = context.org_start - 1;
	}

	if (context.start < context.org_start) {
		context.start = context.org_start;
	}

	fseek(context.file, context.is_bin ? 6 : 0, SEEK_SET);

	if (context.check_crc) {
		check_crc();
	}
	printf("Program size:   %u words\n", context.size);
	printf("Orgin adress:   0x%04X%s\n", context.org_start, context.is_org_cmdline ? " (manual)" : "");
	printf("Code adress:    0x%04X%s\n\n", context.start, context.is_start_cmdline ? " (manual)" : "");
	disassemble();

	fclose(context.file);
	return 0;
}
