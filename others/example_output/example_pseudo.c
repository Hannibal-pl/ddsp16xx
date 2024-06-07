#include <stdint.h>

extern int16_t w_007F;
extern int16_t w_0080;
extern void (*f_0F20)(void);
extern void (*f_42F8)(void);

#define XFIX(x) (x + 0xC000)

void f_1818(void);

int16_t w_1800 = 0x000C;

void f_1801(void) {
	if ((w_0080 == 0x96A0) || (w_0080 == 0x92A3)) {
		w_007F += w_1800;
		f_0F20 = XFIX(f_1818);
	}
}

void f_1818(void) {
	if (w_0080 == 0x977F) {
		w_007F += 0x0080;
		f_0F20 = f_42F8;
	}
}
