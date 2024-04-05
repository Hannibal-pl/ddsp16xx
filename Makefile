all: ddsp

ddsp: ddsp.c ddsp.h
	gcc ddsp.c -Wall -o ddsp

clean:
	rm ddsp
