CC=gcc
CFLAGS=-Wall -g -std=gnu99 -flto
INCLUDES=-I/usr/include
LIBS=-lm
MOD=cmdline.o ddsp.o fields.o instructions.o tools.o

all: ddsp

$(MOD): %.o: %.c ddsp.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

ddsp: $(MOD)
	$(CC) $(CFLAGS) $(LIBS) -o ddsp $(MOD)

clean:
	rm -f *o ddsp

rebuild: clean all

.PHONY: all clean rebuild