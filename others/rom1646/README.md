This is ROM dumped from Lucent 1646 Winmodem chip.

It was taken with double indirect method. Using ROM functionality code was
injected into Y RAM. This code copy data (256 words at once) from directly not
accessible X ROM into Y RAM. Then data from Y RAM was copied again using ROM
functionality into host system.

ROM is running other code in same time, which can alter copied earlier data.
An in fact it did it for sure when larger parts of memory was copied at once.
So expect some errors in this dump.

X ROM is 0xC000 words in size and begins at 0x0000. Addresses 0xC000-0xE7FF
are X RAM shared with Y RAM with addresses 0x0000-0x27FF.
