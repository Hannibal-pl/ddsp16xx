#include <stdarg.h>

#include "ddsp.h"


uint16_t get_page(void) {
	return (context.org_cur & 0xF000);
}

void oprintf(char* format, ...) {
	va_list ap;
	va_start(ap, format);
	if (context.is_org) {
		printf("0x%04X:    ", context.org_cur);
	}
	if (context.is_indent) {
		printf("    ");
	}
	vprintf(format, ap);
	va_end(ap);
}
