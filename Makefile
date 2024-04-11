CC=gcc
CFLAGS=-Wall -g -std=gnu99 -flto
INCLUDES=-I/usr/include
LIBS=-lm
MOD=cmdline.o ddsp16xx.o fields.o instructions.o tools.o

all: ddsp16xx

$(MOD): %.o: %.c ddsp16xx.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

ddsp16xx: $(MOD)
	$(CC) $(CFLAGS) $(LIBS) -o ddsp16xx $(MOD)

clean:
	rm -f *o ddsp16xx

rebuild: clean all

.PHONY: all clean rebuild