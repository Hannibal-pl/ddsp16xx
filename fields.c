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

char *format_B[8] = {	"return",
			"ireturn",
			"goto pt",
			"call pt",
			"unknown gotoB operation 0b1$%03b",
			"unknown gotoB operation 0b1$%03b",
			"unknown gotoB operation 0b1$%03b",
			"unknown gotoB operation 0b1$%03b"};

unsigned field_B(char *buf, unsigned bufsize, unsigned B) {
	snprintf(buf, bufsize, format_B[B & 0x7], B);
	return strlen(buf);
}




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
				"unk30",
				"unk31"};

char * field_CON(unsigned con) {
	return string_CON[con & 0x1F];
}

bool is_CON_true(unsigned con) {
	return (context.is_hidden && (con == 0b01110));
}

bool is_CON_false(unsigned con) {
	return (context.is_hidden && (con == 0b01111));
}

void fix_CON(unsigned cpu) {
	switch (cpu) {
		case CPU_DSP_1611:
		case CPU_DSP_1617:
			string_CON[0b11100] = "unk28";
			string_CON[0b11101] = "unk29";
			break;
		case CPU_DSP_1618:
			string_CON[0b11100] = "ebusy";
			string_CON[0b11101] = "unk29";
		case CPU_DSP_1627:
		case CPU_DSP_1629:
			string_CON[0b11101] = "unk29";
			break;
	}
}




char *string_DR[16] = {		"r0",
				"r1",
				"r2",
				"r3",
				"a0",
				"a0l",
				"a1",
				"a1l",
				"y",
				"yl",
				"p",
				"pl",
				"x",
				"pt",
				"pr",
				"psw"};

char * field_DR(unsigned dr) {
	return string_DR[dr & 0xF];
}




char *format_F1 [16] = {	"p = x * y#a%1$i = p",
				"p = x * y#a%1$i = a%2$i + p",
				"p = x * y",
				"p = x * y#a%1$i = a%2$i - p",
				"a%1$i = p",
				"a%1$i = a%2$i + p",
				"",
				"a%1$i = a%2$i - p",
				"a%1$i = a%2$i | y",
				"a%1$i = a%2$i ^ y",
				"a%2$i & y",
				"a%2$i - y",
				"a%1$i = y",
				"a%1$i = a%2$i + y",
				"a%1$i = a%2$i & y",
				"a%1$i = a%2$i - y"};

unsigned field_F1(char *buf, unsigned bufsize, unsigned F1, unsigned D, unsigned S) {
	snprintf(buf, bufsize, format_F1[F1 & 0xF], D, S);
	return strlen(buf);
}




char *format_F2[16] = {		"a%1$i = a%2$i >> 1",
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

unsigned field_F2(char *buf, unsigned bufsize, unsigned F2, unsigned D, unsigned S) {
	snprintf(buf, bufsize, format_F2[F2 & 0xF], D, S);
	return strlen(buf);
}




char *format_F3 [16] = {	"unknown F3 operation 0x0",
				"unknown F3 operation 0x1",
				"unknown F3 operation 0x2",
				"unknown F3 operation 0x3",
				"unknown F3 operation 0x4",
				"unknown F3 oparation 0x5",
				"unknown F3 operation 0x6",
				"unknown F3 operation 0x7",
				"a%1$i = a%2$i%3$s | ",
				"a%1$i = a%2$i%3$s ^ ",
				"a%2$i%3$s & ",
				"a%2$i%3$s - ",
				"unknown operation F3 0xC",
				"a%1$i = a%2$i%3$s + ",
				"a%1$i = a%2$i%3$s & ",
				"a%1$i = a%2$i%3$s - "};

unsigned field_F3(char *buf, unsigned bufsize, unsigned F3, unsigned D, unsigned S, char *hl) {
	snprintf(buf, bufsize, format_F3[F3 & 0xF], D, S, hl);
	return strlen(buf);
}


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
				"unk15",
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
				"unk30",
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
				"unk44",
				"unk45",
				"unk46",
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

char * field_R(unsigned r) {
	return string_R[r & 0x1E];
}

void fix_R(unsigned cpu) {
	switch (cpu) {
		case CPU_DSP_1611:
			string_R[0b001110] = "unk14";
			string_R[0b101111] = "unk47";
			break;
		case CPU_DSP_1617:
			string_R[0b001110] = "pdx2";
			string_R[0b001111] = "pdx3";
			string_R[0b011100] = "pioc";
			string_R[0b011110] = "pdx1";
			string_R[0b101100] = "pdx4";
			string_R[0b101101] = "pdx5";
			string_R[0b101110] = "pdx6";
			string_R[0b101111] = "pdx7";
			string_R[0b111001] = "unk57";
			string_R[0b111110] = "unk62";
			break;
		case CPU_DSP_1618:
			string_R[0b001110] = "unk14";
			break;
		case CPU_DSP_1627:
		case CPU_DSP_1629:
			string_R[0b101111] = "unk47";
			string_R[0b111001] = "unk57";
			string_R[0b111110] = "unk62";
			break;
	}
}





char *string_SR[8] = {	"j",
			"k",
			"rb",
			"re",
			"r0",
			"r1",
			"r2",
			"r3"};

char * field_SR(unsigned sr) {
	return string_SR[sr & 0x7];
}




char *format_Y[4] = {	"",
			"r%1$i++",
			"r%1$i--",
			"r%1$i++j"};

unsigned field_Y(char *buf, unsigned bufsize, unsigned reg, unsigned op) {
	snprintf(buf, bufsize, format_Y[op & 0x3], reg);
	return strlen(buf);
}





char *format_Z1[4] = {	"",
			"r%1$i++",
			"r%1$i--",
			"r%1$i++j"};

char *format_Z2[4] = {	"r%1$i++",
			"",
			"r%1$i++2",
			"r%1$i++k"};

unsigned field_Z1(char *buf, unsigned bufsize, unsigned reg, unsigned op) {
	snprintf(buf, bufsize, format_Z1[op & 0x3], reg);
	return strlen(buf);
}

unsigned field_Z2(char *buf, unsigned bufsize, unsigned reg, unsigned op) {
	snprintf(buf, bufsize, format_Z2[op & 0x3], reg);
	return strlen(buf);
}

