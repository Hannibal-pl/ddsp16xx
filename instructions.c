#include "ddsp.h"

void instr_b00000(uint16_t word) {
	union INSTR_F4 instr;

	instr.i = word;
	oprintf("goto 0x%04X", get_page() | instr.ja);
}
