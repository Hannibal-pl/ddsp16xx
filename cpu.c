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

struct CPU cpu_list[CPU_DSP_COUNT] = {
	{	.id = CPU_DSP_GENERIC,
		.name = "generic",
		.description = "Universal processor joining all features from rest of CPUs (default)"
	} , {
		.id = CPU_DSP_1604,
		.name = "dsp1604",
		.description = "AT&T DSP 1604"
	} , {
		.id = CPU_DSP_1606,
		.name = "dsp1606",
		.description = "AT&T DSP 1606"
	} , {
		.id = CPU_DSP_1610,
		.name = "dsp1610",
		.description = "AT&T DSP 1610"
	} , {
		.id = CPU_DSP_1611,
		.name = "dsp1611",
		.description = "Lucent DSP 1611"
	}, {
		.id = CPU_DSP_1617,
		.name = "dsp1617",
		.description = "Lucent DSP 1617"
	}, {
		.id = CPU_DSP_1618,
		.name = "dsp1618",
		.description = "Lucent DSP 1618"
	}, {
		.id = CPU_DSP_1627,
		.name = "dsp1627",
		.description = "Lucent DSP 1627"
	}, {
		.id = CPU_DSP_1628,
		.name = "dsp1628",
		.description = "Lucent DSP 1628"
	}, {
		.id = CPU_DSP_1629,
		.name = "dsp1629",
		.description = "Lucent DSP 1629"
	}, {
		.id = CPU_DSP_1650,
		.name = "dsp1650",
		.description = "Lucent FlashDSP 1650"
	}
};

void list_cpu(void) {
	printf("All supported DSP processors:\n");
	for (int i = 0; i < CPU_DSP_COUNT; i++) {
		printf("  %-16s  - %s\n", cpu_list[i].name, cpu_list[i].description);
	}
}

void fix_cpu(unsigned cpu) {
	fix_CON(cpu);
	fix_R(cpu);
}

struct CPU * get_cpu(unsigned cpu) {
	if (cpu < CPU_DSP_COUNT) {
		return &cpu_list[cpu];
	}

	return &cpu_list[CPU_DSP_GENERIC];
}

bool select_cpu(char *name) {
	for (int i = 0; i < CPU_DSP_COUNT; i++) {
		if (strncmp(cpu_list[i].name, name, CPU_NAME_LENGTH) == 0) {
			context.cpu = cpu_list[i].id;
			return true;
		}
	}
	return false;
}
