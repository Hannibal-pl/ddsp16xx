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
		case CPU_DSP_1604:
		case CPU_DSP_1606:
			string_CON[0b10010] = "unk18";
			string_CON[0b10011] = "unk19";
			string_CON[0b10100] = "unk20";
			string_CON[0b10101] = "unk21";
			string_CON[0b10110] = "unk22";
			string_CON[0b10111] = "unk23";
			string_CON[0b11000] = "unk24";
			string_CON[0b11001] = "unk25";
			string_CON[0b11100] = "unk28";
			string_CON[0b11101] = "unk29";
			break;
		case CPU_DSP_1610:
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
		case CPU_DSP_1650:
			string_CON[0b10010] = "pllon";
			string_CON[0b10011] = "slowon";
			string_CON[0b10100] = "plloff";
			string_CON[0b10101] = "stopclk";
			string_CON[0b10110] = "unk22";
			string_CON[0b10111] = "unk23";
			string_CON[0b11000] = "unk24";
			string_CON[0b11001] = "unk25";
			string_CON[0b11100] = "unk28";
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




char *format_F1 [16] = {	"a%1$i = p#p = x * y",
				"a%1$i = a%2$i + p#p = x * y",
				"p = x * y",
				"a%1$i = a%2$i - p#p = x * y",
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
				"pi",
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
	return string_R[r & 0x3F];
}

char * field_R_ish(unsigned r) {
	switch (r & 0x3F) {
		case 0b001100: //p
		case 0b010001: //y
		case 0b110000: //a0
		case 0b110010: //a1
			return "h";
			break;
		default:
			break;
	}
	return "";
}

void fix_R(unsigned cpu) {
	switch (cpu) {
		case CPU_DSP_1604:
		case CPU_DSP_1606:
			string_R[0b001110] = "unk14";
			string_R[0b011001] = "sioce";
			string_R[0b011010] = "sdx0";
			string_R[0b011011] = "pgreg";
			string_R[0b011100] = "unk28";
			string_R[0b011101] = "unk29";
			string_R[0b011111] = "unk31";
			string_R[0b100010] = "unk34";
			string_R[0b100010] = "unk35";
			string_R[0b100110] = "drc";
			string_R[0b100111] = "unk39";
			string_R[0b101000] = "cbita";
			string_R[0b101001] = "sbita";
			string_R[0b101111] = "unk47";
			string_R[0b110110] = "cbitb";
			string_R[0b110111] = "sbitb";
			string_R[0b111000] = "cbitc";
			string_R[0b111001] = "sbitc";
			string_R[0b111010] = "timer1";
			string_R[0b111011] = "sdx1";
			string_R[0b111100] = "sdx2";
			string_R[0b111101] = "unk61";
			string_R[0b111110] = "unk62";
			break;
		case CPU_DSP_1610:
			string_R[0b001110] = "pdx2";
			string_R[0b001111] = "pdx3";
			string_R[0b011100] = "pioc";
			string_R[0b011110] = "pdx1";
			string_R[0b100110] = "unk38";
			string_R[0b101100] = "pdx4";
			string_R[0b101101] = "pdx5";
			string_R[0b101110] = "pdx6";
			string_R[0b101111] = "pdx7";
			string_R[0b110110] = "unk54";
			string_R[0b110111] = "unk55";
			string_R[0b111000] = "unk56";
			string_R[0b111001] = "unk57";
			string_R[0b111110] = "unk62";
			break;
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
		case CPU_DSP_164X: //guessed
			//suspected to be 1650 like
			break;
		case CPU_DSP_1650:
			string_R[0b001110] = "unk14";
			string_R[0b001111] = "sbitd";
			string_R[0b011000] = "cdcpg";
			string_R[0b011001] = "cdcamux";
			string_R[0b011010] = "adc1/spkr";
			string_R[0b011011] = "preg";
			string_R[0b011100] = "ssid";
			string_R[0b011101] = "ssic";
			string_R[0b011110] = "chipc";
			string_R[0b100010] = "pllc";
			string_R[0b100011] = "clkc";
			string_R[0b100110] = "drc";
			string_R[0b100111] = "wdogr";
			string_R[0b101000] = "cbita/iopuca";
			string_R[0b101001] = "sbita";
			string_R[0b101010] = "chipo";
			string_R[0b101100] = "cbite/iopuce";
			string_R[0b101101] = "sbite";
			string_R[0b101110] = "cbitd/iopucd";
			string_R[0b101111] = "flashc";
			string_R[0b110110] = "cbitb";
			string_R[0b110111] = "sbitb";
			string_R[0b111000] = "cbitc/iopucc";
			string_R[0b111001] = "sbitc";
			string_R[0b111010] = "timer1";
			string_R[0b111011] = "line1/adc2";
			string_R[0b111100] = "line2/adc3";
			string_R[0b111101] = "adc4";
			string_R[0b111110] = "cdc";
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

