Very simple AT&T/Lucent/Agere DSP16xx Digital Signal Processor dissassembler.

I wrote it to myself. The aim was understanging how dissassembled code works,
not to create proper assembly which can be compiled. It doesn't contain any
check for code correctness. It simply fetch word by word and try match to
instruction to it. So expect weird results like undefined instructions or
parameters or nested cache instructions (do N {}) in case of dissassemby data
or code from unsupported processor.

It was written mainly based on DSP1611/17/18/27/28/29 manual but it should
generate more less proper code for rest of the 16xx family like 164x Winmodem
DSPs. Differeces, in field of view of this project, are only changed names for
some conditional instructions and registers.


It supports two type of files:

 - BIN - default format used in Lucent Winmodem drivers for 164X DSPs, It have
following format:

 -- 0x00: WORD - Data orgin.

 -- 0X02: WORD - Size of data in words.

 -- 0x04: WORD - Control sum - result of xor all data words.

 -- 0x06:      - Actual data


 - RAW - simple raw data
