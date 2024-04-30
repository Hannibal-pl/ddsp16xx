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

#include "ddsp16xx.h"

void instr_data(uint16_t word) {
	if (context.data_bin) {
		oprintf("WORD 0b%016b\n", word);
	} else {
		oprintf("WORD 0x%04X\n", word);
	}
}

void instr_b00000(uint16_t word) {
	union INSTR_F4 instr;

	instr.i = word;
	oprintf("goto 0x%04X\n", get_page() | instr.ja);
}

void instr_b00010(uint16_t word) {
	union INSTR_F9 instr;

	instr.i = word;
	oprintf("%s = 0x%04X\n", field_SR(instr.sr), instr.im9);
}

void instr_b00100(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	oprintbuf(buf_F1);
	oprintf("*r%i = a1%s\n", instr.yzReg, instr.x ? "l" : "");
	oprintbuf(buf_Y);
}

void instr_b00101(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Z[16];

	instr.i = word;

	field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	oprintbuf(buf_F1);

	oprintf("tmp = a%c%s\n", '1' - instr.d, instr.x ? "l" : "");
	oprintf("a%c%s = *r%i\n", '1' - instr.d, instr.x ? "l" : "", instr.yzReg);
	field_Z1(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
	oprintf("*r%i = tmp\n", instr.yzReg);
	field_Z2(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
}

void instr_b00110(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	if (length == 0) { // both nop
		oprintf("nop\n");
		return;
	}

	oprintbuf(buf_F1);
	oprintbuf(buf_Y);
}

void instr_b00111(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];

	instr.i = word;

	field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	oprintbuf(buf_F1);
	oprintf("a%c%s = *r%i\n", '1' - instr.d, instr.x ? "l" : "", instr.yzReg);
	oprintbuf(buf_Y);
}

void instr_b01000(uint16_t word) {
	union INSTR_F7a instr;

	instr.i = word;
	oprintf("a%c%s = %s\n", '1' - instr.aT, instr.x ? "l" : "", field_R(instr.r));
}

void instr_b01001(uint16_t word) {
	union INSTR_F7b instr;

	instr.i = word;
	oprintf("%s = a%c%s\n", field_R(instr.r), '0' + instr.s, instr.x ? "l" : "");
}

void instr_b01010(uint16_t word) {
	union INSTR_F8 instr;

	instr.i = word;
	oprintf("%s = ", field_R(instr.r));
	printf("0x%04X\n", next_word());
}

void instr_b01100(uint16_t word) {
	union INSTR_F7 instr;
	char buf_Y[16];

	instr.i = word;

	oprintf("*r%i = %s\n", instr.yzReg, field_R(instr.r));
	field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Y);
}

void instr_b01101(uint16_t word) {
	union INSTR_F7 instr;
	char buf_Z[16];

	instr.i = word;

	oprintf("tmp = %s\n", field_R(instr.r));
	oprintf("%s = *r%i\n", field_R(instr.r), instr.yzReg);
	field_Z1(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
	oprintf("*r%i = tmp\n", instr.yzReg);
	field_Z2(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
}

void instr_b01110(uint16_t word) {
	union INSTR_F10 instr;
	char buf_k[16];

	instr.i = word;

	if (instr.k == 0) {
		strncpy(buf_k, "cloop", sizeof(buf_k));
	} else {
		snprintf(buf_k, sizeof(buf_k), "%i", instr.k);
	}

	if (instr.n == 0) {
		oprintf("redo %s\n", buf_k);
		return;
	}

	oprintf("do %s {\n", buf_k);
	context.loop_n = instr.n + 1;
	context.indent++;
}

void instr_b01111(uint16_t word) {
	union INSTR_F7 instr;
	char buf_Y[16];

	instr.i = word;

	oprintf("%s = *r%i\n", field_R(instr.r), instr.yzReg);
	field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Y);
}

void instr_b10000(uint16_t word) {
	union INSTR_F4 instr;

	instr.i = word;
	oprintf("call 0x%04X\n", get_page() | instr.ja);
}

void instr_b10010(uint16_t word) {
	union INSTR_F3 instr;
	char buf_F2[64];

	instr.i = word;

	oprintf("test()\n");
	oprintf("c1++\n");

	if (is_CON_true(instr.con)) {
		field_F2(buf_F2, sizeof(buf_F2), instr.f2, instr.d, instr.s);
		oprintf("%s\n", buf_F2);
		oprintf("c2 = c1\n");
	} else if (!is_CON_false(instr.con)) {
		oprintf("if %s {\n", field_CON(instr.con));
		context.indent++;

		field_F2(buf_F2, sizeof(buf_F2), instr.f2, instr.d, instr.s);
		oprintf("%s\n", buf_F2);
		oprintf("c2 = c1\n");

		context.indent--;
		oprintf("}\n");
	}
}

void instr_b10011(uint16_t word) {
	union INSTR_F3 instr;
	char buf_F2[64];

	instr.i = word;

	oprintf("test()\n");

	if (is_CON_true(instr.con)) {
		field_F2(buf_F2, sizeof(buf_F2), instr.f2, instr.d, instr.s);
		oprintf("%s\n", buf_F2);
	} else if (!is_CON_false(instr.con)) {
		oprintf("if %s {\n", field_CON(instr.con));
		context.indent++;
		field_F2(buf_F2, sizeof(buf_F2), instr.f2, instr.d, instr.s);
		oprintf("%s\n", buf_F2);

		context.indent--;
		oprintf("}\n");
	}
}

void instr_b10100(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	oprintbuf(buf_F1);
	oprintf("*r%i = y%s\n", instr.yzReg, instr.x ? "l" : "");
	oprintbuf(buf_Y);
}

void instr_b10101(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Z[16];

	instr.i = word;

	field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	oprintbuf(buf_F1);

	oprintf("tmp = y%s\n", instr.x ? "l" : "");
	oprintf("y%s = *r%i\n", instr.x ? "l" : "", instr.yzReg);
	field_Z1(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
	oprintf("*r%i = tmp\n", instr.yzReg);
	field_Z2(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
}

void instr_b10110(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	oprintbuf(buf_F1);
	oprintf("x = *r%i\n", instr.yzReg);
	oprintbuf(buf_Y);
}

void instr_b10111(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	oprintbuf(buf_F1);
	oprintf("y%s = *r%i\n", instr.x ? "l" : "", instr.yzReg);
	oprintbuf(buf_Y);
}

void instr_b11000(uint16_t word) {
	union INSTR_F3a instr3;
	union INSTR_F5 instr5;
	char buf[64];
	char buf_src2[16];
	char *hl = "";

	instr3.i = word;
	instr5.i = word;

	if (instr3.bit0) {
		oprintf("");
		switch (instr3.src2) {
			case 0b00:
				hl = "l";
				snprintf(buf_src2, sizeof(buf_src2), "0x%04X", next_word());
				break;
			case 0b01:
				snprintf(buf_src2, sizeof(buf_src2), "a%c", '1' - instr3.aT);
				break;
			case 0b10:
				hl = "h";
				snprintf(buf_src2, sizeof(buf_src2), "0x%04X", next_word());
				break;
			case 0b11:
				strncpy(buf_src2, "p", sizeof(buf_src2));
				break;
		}
		field_F3(buf, sizeof(buf), instr3.f3, instr3.d, instr3.s, hl);
		printf("%s%s\n", buf, buf_src2);
	} else {
		field_B(buf, sizeof(buf), instr5.b);
		oprintf("%s\n", buf);
	}
}

void instr_b11001(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];

	instr.i = word;

	field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	oprintbuf(buf_F1);

	oprintf("tmp = y\n");
	oprintf("y = a0\n");
	oprintf("x = *pt\n");
	oprintf("pt++%s\n", instr.x ? "i" : "");
}

void instr_b11010(uint16_t word) {
	union INSTR_F6 instr;
	union INSTR control_instr;

	instr.i = word;

	if (instr.si) {
		oprintf("icall\n");
	}

	oprintf("test()\n");

	if (is_CON_false(instr.con)) {
		next_word(); //fetch control instruction
		return;
	}

	if (!is_CON_true(instr.con)) {
		oprintf("if %s {\n", field_CON(instr.con));
		context.indent++;
	}

	control_instr.i = next_word();
	switch (control_instr.t) {
		case 0b00000:
		case 0b00001:
			instr_b00000(control_instr.i);
			break;
		case 0b10000:
		case 0b10001:
			instr_b10000(control_instr.i);
			break;
		case 0b11000:
			instr_b11000(control_instr.i);
			break;
		default:
			oprintf("Unknown CONTROL operation 0b%05b\n", control_instr.t);
			break;
	}

	if (!is_CON_true(instr.con)) {
		context.indent--;
		oprintf("}\n");
	}
}

void instr_b11011(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];

	instr.i = word;

	field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	oprintbuf(buf_F1);

	oprintf("tmp = y\n");
	oprintf("y = a1\n");
	oprintf("x = *pt\n");
	oprintf("pt++%s\n", instr.x ? "i" : "");
}

void instr_b11100(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	oprintbuf(buf_F1);
	oprintf("*r%i = a0%s\n", instr.yzReg, instr.x ? "l" : "");
	oprintbuf(buf_Y);
}

void instr_b11101(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Z[16];

	instr.i = word;

	field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	oprintbuf(buf_F1);

	oprintf("tmp = y\n");
	oprintf("y = *r%i\n", instr.yzReg);
	field_Z1(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
	oprintf("*r%i = tmp\n", instr.yzReg);
	field_Z2(buf_Z, sizeof(buf_Z), instr.yzReg, instr.yzOp);
	oprintbuf(buf_Z);
	oprintf("x = *pt\n");
	oprintf("pt++%s\n", instr.x ? "i" : "");
}

void instr_b11110(uint16_t word) {
	union INSTR_F9a instr9;
	union INSTR_F3b instr3;

	instr9.i = word;
	instr3.i = word;

	if (instr9.bit5) {
		if (instr9.rw) {
			oprintf("%s = *(ybase + 0x%02X)\n", field_DR(instr9.dr), instr9.offset);
		} else {
			oprintf("*(ybase + 0x%02X) = %s\n", instr9.offset, field_DR(instr9.dr));
		}
	} else {
		switch(instr3.bmu) {
			case 0b0000000:
				oprintf("a%1$c = a%2$c >> ar%3$i\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b0000100:
				oprintf("a%1$c = a%2$c << ar%3$i\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b0000010:
				oprintf("a%1$c = a%2$c >>> ar%3$i\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b0000110:
				oprintf("a%1$c = a%2$c <<< ar%3$i\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b1000000:
				oprintf("a%1$c = a%2$c >> a%3$c\n", '0' + instr3.d, '1' - instr3.s, '0' + instr3.s);
				break;
			case 0b1000100:
				oprintf("a%1$c = a%2$c << a%3$c\n", '0' + instr3.d, '1' - instr3.s, '0' + instr3.s);
				break;
			case 0b1000010:
				oprintf("a%1$c = a%2$c >>> a%3$c\n", '0' + instr3.d, '1' - instr3.s, '0' + instr3.s);
				break;
			case 0b1000110:
				oprintf("a%1$c = a%2$c <<< a%3$c\n", '0' + instr3.d, '1' - instr3.s, '0' + instr3.s);
				break;
			case 0b1100000:
				oprintf("a%1$c = a%2$c >> 0x%3$04X\n", '0' + instr3.d, '0' + instr3.s, next_word());
				break;
			case 0b1101000:
				oprintf("a%1$c = a%2$c << 0x%3$04X\n", '0' + instr3.d, '0' + instr3.s, next_word());
				break;
			case 0b1100010:
				oprintf("a%1$c = a%2$c >> 0x%3$04X\n", '0' + instr3.d, '0' + instr3.s, next_word());
				break;
			case 0b1101010:
				oprintf("a%1$c = a%2$c << 0x%3$04X\n", '0' + instr3.d, '0' + instr3.s, next_word());
				break;
			case 0b0000011:
				oprintf("a%1$c = exp(a%2$c)\n", '0' + instr3.d, '0' + instr3.s);
				break;
			case 0b0000111:
				oprintf("a%1$c = norm(a%2$c, ar%3$i)\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b1110000:
				oprintf("a%1$c = extracts(a%2$c, 0x%3$04X)\n", '0' + instr3.d, '0' + instr3.s, next_word());
				break;
			case 0b1110001:
				oprintf("a%1$c = extractz(a%2$c, 0x%3$04X)\n", '0' + instr3.d, '0' + instr3.s, next_word());
				break;
			case 0b1110010:
				oprintf("a%1$c = insert(a%2$c, 0x%3$04X)\n", '0' + instr3.d, '0' + instr3.s, next_word());
				break;
			case 0b0010000:
				oprintf("a%1$c = extracts(a%2$c, ar%3$i)\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b0010001:
				oprintf("a%1$c = extractz(a%2$c, ar%3$i)\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b0010010:
				oprintf("a%1$c = insert(a%2$c, ar%3$i)\n", '0' + instr3.d, '0' + instr3.s, instr3.m);
				break;
			case 0b0110100:
				if ((instr3.m & 0x2) == 0) {
					oprintf("a%1$c = a%2$c:aa%3$i\n", '0' + instr3.d, '0' + instr3.s, instr3.m & 0x1);
					break;
				}
				//faltrough
			default:
				oprintf("Unknown BMU operation 0b%07b\n", instr3.bmu);
				// It seems that 5th bit mark IM16 parameter. In case if it's set in unknown
				// operation it maybe desirable to fetch it. For now it's missing because
				// this situation in real case is higly unlikely. And in case of dissassemby
				// data this may break following code.
				break;
		}
	}
}

void instr_b11111(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yzReg, instr.yzOp);

	oprintbuf(buf_F1);
	oprintf("y = *r%i\n", instr.yzReg);
	oprintbuf(buf_Y);
	oprintf("x = *pt\n");
	oprintf("pt++%s\n", instr.x ? "i" : "");
}
