#ifndef __DDSP_H__
#define __DDSP_H__

#include <stdint.h>

union INSTR {
	uint16_t i;
	struct {
		unsigned p:11;
		unsigned t:5;
	};
};

union INSTR_F1 {
	uint16_t i;
	struct {
		unsigned y:4;
		unsigned x:1;
		unsigned f1:4;
		unsigned s:1;
		union {
			unsigned d:1;
			unsigned aT:1;
		};
		unsigned t:5;
	};
};

union INSTR_F4 {
	uint16_t i;
	struct {
		unsigned ja:11;
		unsigned t:5;
	};
};

#endif
