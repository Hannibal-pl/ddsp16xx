#include <string.h>

#include "ddsp.h"

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
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintbuf(buf_F1);
	oprintf("*r%i = a1%s\n", instr.yReg, instr.x ? "l" : "");
	oprintbuf(buf_Y);
}

void instr_b00110(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

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
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintbuf(buf_F1);
	oprintf("a%c%s = *r%i\n", '1' - instr.d, instr.x ? "l" : "", instr.yReg);
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

	oprintf("*r%i = %s\n", instr.yReg, field_R(instr.r));
	field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);
	oprintbuf(buf_Y);
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

	oprintf("%s = *r%i\n", field_R(instr.r), instr.yReg);
	field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);
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
	oprintf("c1++");
	oprintf("if %s {\n", field_CON(instr.con));
	context.indent++;

	field_F2(buf_F2, sizeof(buf_F2), instr.f2, instr.d, instr.s);
	oprintf("%s\n", buf_F2);
	oprintf("c2 = c1");

	context.indent--;
	oprintf("}\n");

}

void instr_b10011(uint16_t word) {
	union INSTR_F3 instr;
	char buf_F2[64];

	instr.i = word;
	oprintf("test()\n");
	oprintf("if %s {\n", field_CON(instr.con));
	context.indent++;

	field_F2(buf_F2, sizeof(buf_F2), instr.f2, instr.d, instr.s);
	oprintf("%s\n", buf_F2);

	context.indent--;
	oprintf("}\n");

}

void instr_b10100(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintbuf(buf_F1);
	oprintf("*r%i = y%s\n", instr.yReg, instr.x ? "l" : "");
	oprintbuf(buf_Y);
}

void instr_b10110(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintbuf(buf_F1);
	oprintf("x = *r%i\n", instr.yReg);
	oprintbuf(buf_Y);
}

void instr_b10111(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintbuf(buf_F1);
	oprintf("y%s = *r%i\n", instr.x ? "l" : "", instr.yReg);
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

void instr_b11010(uint16_t word) {
	union INSTR_F6 instr;
	union INSTR control_instr;

	instr.i = word;

	if (instr.si) {
		oprintf("icall\n");
	}

	oprintf("test()\n");
	oprintf("if %s {\n", field_CON(instr.con));
	context.indent++;

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
			printf("UNKNOWN CONTROL 0b%05b\n", control_instr.t);
			break;
	}

	context.indent--;
	oprintf("}\n");
}

void instr_b11100(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintbuf(buf_F1);
	oprintf("*r%i = a0%s\n", instr.yReg, instr.x ? "l" : "");
	oprintbuf(buf_Y);
}

void instr_b11111(uint16_t word) {
	union INSTR_F1 instr;
	char buf_F1[64];
	char buf_Y[16];
	unsigned length = 0;

	instr.i = word;

	length += field_F1(buf_F1, sizeof(buf_F1), instr.f1, instr.d, instr.s);
	length += field_Y(buf_Y, sizeof(buf_Y), instr.yReg, instr.yOp);

	oprintbuf(buf_F1);
	oprintf("y = *r%i\n", instr.yReg);
	oprintbuf(buf_Y);
	oprintf("x = *pt\n");
	oprintf("pt++%s\n", instr.x ? "i" : "");
}
