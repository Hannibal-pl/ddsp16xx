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

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "ddsp16xx.h"

const struct option longopt[] = {
	{"bin", 0, NULL, 'b'},
	{"crc", 0, NULL, 'c'},
	{"help", 0, NULL, 'h'},
	{"orgin", 1, NULL, 'o'},
	{"raw", 0, NULL, 'r'},
	{"version", 0, NULL, 'v'},
	{NULL, 0, NULL, 0}};

void parseparams(int argc, char *argv[]) {
	int opt;

	while (true) {
		opt = getopt_long(argc, argv, "bcho:rv?", longopt, NULL);
		if (opt == -1) {
			break;
		}

		switch (opt) {
			case 'b':
				context.is_bin = true;
				break;
			case 'c':
				context.check_crc = true;
				break;
			case '?':
			case 'h':
				printf("Usage: %s [OPTIONS] filename\n", argv[0]);
				printf("  -b, --bin\t\t\tSet input file format to BIN, default.\n");
				printf("  -c, --crc\t\t\tCheck input file control sum. BIN format only.\n");
				printf("  -h, --help\t\t\tPrint this help and exit.\n");
				printf("  -o, --orgin=ADDRESS\tOverwrite default code orgin.\n");
				printf("  -r, --raw\t\t\tSet input file format to RAW.\n");
				printf("  -v, --version\t\t\tPrint version number and exit.\n");
				exit(0);
				break;
			case 'o':
				uint32_t org;
				if (optarg == NULL) {
					printf("You must specify code orgin addres.\n");
					exit(-1);
				}
				org = strtoul(optarg, NULL, 0);
				if (org > 0xFFFF) {
					printf("Provided code orgin addres don't fit in 16bit addres space.\n");
					exit(-1);
				}
				context.org_start = org & 0xFFFF;
				context.org_cur = context.org_start - 1;
				context.is_org_cmdline = true;
				break;
			case 'r':
				context.is_bin = false;
				break;
			case 'v':
				printf("%s: version: %s\n", argv[0], VERSION);
				exit(0);
				break;
		}
	}

	if (optind >= argc) {
		printf("You must provide input filename\n");
		exit(-1);
	}

	context.file = fopen(argv[optind], "r");
	if (context.file == NULL) {
		printf("Input file not found: %s\n", argv[optind]);
		exit(-1);
	}
}
