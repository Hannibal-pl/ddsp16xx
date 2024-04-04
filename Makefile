all: ddsp

ddsp: ddsp.c
	gcc ddsp.c -Wall -o ddsp

clean:
	rm ddsp
