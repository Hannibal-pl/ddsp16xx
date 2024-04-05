#include <string.h>

#include "ddsp.h"

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



char *format_Y[4] = {	"",
			"r%1$i++",
			"r%1$i--",
			"r%1$i++j"};

unsigned field_Y(char *buf, unsigned bufsize, unsigned reg, unsigned op) {
	snprintf(buf, bufsize, format_Y[op & 0x3], reg);
	return strlen(buf);
}
