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
	{"dsp", 1, NULL, 'd'},
	{"help", 0, NULL, 'h'},
	{"hidden", 0, NULL, 'H'},
	{"list-dsp", 0, NULL, 'l'},
	{"orgin", 1, NULL, 'o'},
	{"no-lablel", 0, NULL, 'n'},
	{"raw", 0, NULL, 'r'},
	{"start", 1, NULL, 's'},
	{"version", 0, NULL, 'v'},
	{NULL, 0, NULL, 0}};

void usage(char *appname) {
				printf("\nUsage: %s [OPTIONS] filename\n", appname);
				printf("  -b, --bin\t\tSet input file format to BIN, default.\n");
				printf("  -c, --crc\t\tCheck input file control sum. BIN format only.\n");
				printf("  -d, --dsp\t\tSelect DSP version. See oprion `-l` for list.\n");
				printf("  -h, --help\t\tPrint this help and exit.\n");
				printf("  -H, --hidden\t\tShow hidden 'always true/false' conitional code.\n");
				printf("  -l  --list-dsp\tList all supported DSPs and exit.\n");
				printf("  -n  --no-lablel\tDon't print line addres lables.\n");
				printf("  -o, --orgin=ADDRESS\tOverwrite default file orgin.\n");
				printf("  -r, --raw\t\tSet input file format to RAW.\n");
				printf("  -s, --start=ADDRESS\tCode start addres. Data before it won't be dissassebled.\n");
				printf("  -v, --version\t\tPrint version number and exit.\n\n");
}

void parseparams(int argc, char *argv[]) {
	int opt;

	while (true) {
		opt = getopt_long(argc, argv, "bcd:hHlo:nrs:v?", longopt, NULL);
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
			case 'd':
				if (optarg == NULL) {
					printf("You must provide DSP name.\n");
					exit(-1);
				}
				if (!select_cpu(optarg)) {
					printf("Invalid DSP name '%s'. use option `-l` for list.\n", optarg);
					exit(-1);
				}
				break;
			case '?':
			case 'h':
				usage(argv[0]);
				exit(0);
				break;
			case 'H':
				context.is_hidden = false;
				break;
			case 'l':
				list_cpu();
				exit(0);
				break;
			case 'o':
				uint32_t org;
				if (optarg == NULL) {
					printf("You must specify file orgin addres.\n");
					exit(-1);
				}
				org = strtoul(optarg, NULL, 0);
				if (org > 0xFFFF) {
					printf("Provided file orgin addres don't fit in 16bit addres space.\n");
					exit(-1);
				}
				context.org_start = org & 0xFFFF;
				context.org_cur = context.org_start - 1;
				context.is_org_cmdline = true;
				break;
			case 'n':
				context.is_org = false;
				break;
			case 'r':
				context.is_bin = false;
				break;
			case 's':
				uint32_t start;
				if (optarg == NULL) {
					printf("You must specify code start addres.\n");
					exit(-1);
				}
				start = strtoul(optarg, NULL, 0);
				if (org > 0xFFFF) {
					printf("Provided code start addres don't fit in 16bit addres space.\n");
					exit(-1);
				}
				context.start = start & 0xFFFF;
				context.is_start_cmdline = true;
				break;
			case 'v':
				printf("%s: version: %s\n", argv[0], VERSION);
				exit(0);
				break;
		}
	}

	if (optind >= argc) {
		printf("You must provide input filename.\n");
		if (argc == 1) {
			//no options at all
			usage(argv[0]);
		}
		exit(-1);
	}

	context.file = fopen(argv[optind], "r");
	if (context.file == NULL) {
		printf("Input file not found: '%s'.\n", argv[optind]);
		exit(-1);
	}
}
