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

#include <stdarg.h>
#include <string.h>

#include "ddsp16xx.h"

uint16_t next_word(void) {
	uint16_t word;

	if (context.is_single) {
		//if in single instruction mode treat hi word as optional second instruction word
		return (context.single >> 16);
	}

	fread(&word, sizeof(word), 1, context.file);
	context.org_cur++;

	return word;
}

uint16_t lookahead_word(void) {
	uint16_t word;
	fread(&word, sizeof(word), 1, context.file);
	fseek(context.file, -sizeof(word), SEEK_CUR);

	return word;
}

uint16_t get_page(void) {
	return (context.org_cur & 0xF000);
}

void oprintf(char* format, ...) {
	va_list ap;
	va_start(ap, format);
	if (context.is_org) {
		printf("0x%04X:    ", context.org_cur);
	}
	for (int i = 0; i < context.indent; i++) {
		printf("    ");
	}
	vprintf(format, ap);
	va_end(ap);
}

void oprintbuf(char *buf) {
	char *cur = buf;
	char *next;

	if (strlen(buf) == 0) {
		return;
	}
	while((next = strchr(cur, '#')) != NULL) {
		next[0] = 0;
		oprintf("%s\n", cur);
		cur = next + 1;
	}
	oprintf("%s\n", cur);
}

void check_crc(void) {
	uint32_t fpos;
	uint16_t size;
	uint16_t crc;
	uint16_t val;

	if (!context.is_bin) {
		return;
	}

	fpos = ftell(context.file);

	fseek(context.file, 2, SEEK_SET);
	fread(&size, sizeof(size), 1, context.file);
	fread(&crc, sizeof(crc), 1, context.file);
	printf("Control sum 0x%04X - ", crc);
	for (int i = 0; i < size; i ++) {
		fread(&val, sizeof(val), 1, context.file);
		crc ^= val;
	}

	if (crc == 0) {
		printf("OK\n");
	} else {
		printf("FAILED\n");
	}

	fseek(context.file, fpos, SEEK_SET);
}

void debug_instruction(uint16_t word) {
	if (!context.debug_instruction) {
		return;
	}

	oprintf("// %04b ", (word >> 12) & 0x000F);
	printf("%04b ", (word >> 8) & 0x000F);
	printf("%04b ", (word >> 4) & 0x000F);
	printf("%04b", word & 0x000F);

	//second word
	if (((word & 0xF800) == 0x5000) || ((word & 0xF809) == 0xC001) || ((word & 0xF9A0) == 0xF180)) {
		uint16_t second = lookahead_word();
		printf("        %04b ", (second >> 12) & 0x000F);
		printf("%04b ", (second >> 8) & 0x000F);
		printf("%04b ", (second >> 4) & 0x000F);
		printf("%04b", second & 0x000F);
	}

	printf("\n");

}
