#include <stdint.h>

extern int16_t w_2117;

int16_t f_C691(int16_t a0h, int16_t a1h) {
	if (a1h) {
		a0h |= 0x0800;
	}
	return (~a0h) & 0x00FF;
}

int16_t f_C69A(int16_t a0h, int16_t a1h) {
	if (!a1h) {
		a0h |= 0x0080;
		a0h ^= 0x0055;
	}
	return a0h;
}

int32_t f_C6A2(int16_t a0h) {
	int16_t base, shift;

	a0h ^= 0x00FF;

	shift = (a0h >> 4) & 0x0007 + 2;
	base = ((a0h & 0x000F) << 1) + 33;
	base <<= shift;

	if (a0h & 0x0080) {
		base = -base;
	}

	return base;
}

int16_t f_C6BF(int16_t a0h) {
	int16_t base, shift;

	a0h ^= 0x00D5;

	shift = (a0h >> 4) & 0x0007;
	if (shift) {
		base = ((a0h & 0x000F) << 1) + 33;
		base <<= shift;
	} else {
		base = ((a0h & 0x000F) << 2) + 2;
	}

	if (a0h & 0x0080) {
		base = -base;
	}

	return (base << 2);
}

int16_t f_C6E2(int16_t yh, int16_t k) {
	if (w_2117 == 0) {
		return f_C691(yh, k);
	} else {
		return f_C69A(yh, k);
	}
}

int16_t f_C6F8(int16_t a0h) {
	if (w_2117 == 0) {
		return f_C6A2(a0h);
	} else {
		return f_C6BF(a0h);
	}
}
