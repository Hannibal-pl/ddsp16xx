#include <stdint.h>

extern int16_t w_0083;
extern int16_t w_00AA;
extern int16_t w_00AB;
extern int16_t w_0801;
extern int16_t w_082E;
extern int16_t w_0A51;
extern int16_t w_0CC8;
extern int16_t w_0CD1;
extern int16_t w_0CD6;
extern int16_t w_0CD8;
extern int16_t w_0CDC;
extern int16_t w_0CDD;
extern int16_t w_0DFA;
extern void (*f_0080)(int16_t);		//next machine state code ptr
extern void (*f_0F20)(void);		//machine state hook funtion ptr
extern void (*f_42F8)(void);		//empty hook function
extern void f_91DE(int16_t);
extern void f_9239(int16_t);
extern void f_9256(int16_t);
extern void f_9276(int16_t);
extern void f_927C(int16_t);
extern void f_928C(int16_t);
extern void f_9292(int16_t);
extern void f_92B8(int16_t);
extern void f_92C0(int16_t);
extern void f_9311(int16_t);
extern void f_9319(int16_t);
extern void f_9356(int16_t);
extern void f_95A0(int16_t);
extern void f_95FB(int16_t);
extern void f_9618(int16_t);
extern void f_9638(int16_t);
extern void f_963E(int16_t);
extern void f_9662(int16_t);
extern void f_9673(int16_t);
extern void f_96B0(int16_t);
extern void f_96B8(int16_t);
extern void f_974D(int16_t);
extern void f_9A4B(void);
extern void f_9AA2(void);
extern void f_9D74(int16_t);

void f_D842(void);
void f_D89B(int16_t k);
void f_D8A3(int16_t k);
void f_D8AB(int16_t k);
void f_D8B5(int16_t k);
void f_D8BF(int16_t k);
void f_D8C7(int16_t k);
void f_D8D4(int16_t k);
int16_t f_D8DE(void);


void f_D800(void) {
	if ((f_0080 == f_9256) || (f_0080 == f_9618)) {
		return;
	}

	w_0A51 = 0x0000;
	f_9AA2();

	w_0CD8 = 0x0000;
	w_0CC8 = 0x0000;
	w_0CD6 = 0x0011;
	f_9A4B();

	w_082E |= 0xFFFE;
	w_0DFA = w_0801;
	f_9D74(w_0083 ? 0x0000 : 0x000C);

	w_0CD1 = 0x0011;
	f_0F20 = f_D842;	//install state machine hook
	f_D842();
}

//This is state machine hook function, it executes before each machine state does.
//It alters code for some states to code from this file.

void f_D842(void) {
	if (f_0080 == f_9276) {
		w_00AA = 0x0001;
		w_00AB = 0x0001;
		f_0080 = f_D89B;
	} else if (f_0080 == f_928C) {
		f_0080 = f_D8A3;
	} else if (f_0080 == f_92B8) {
		f_0080 = f_D8AB;
	} else if (f_0080 == f_9311) {
		f_0080 = f_D8B5;
	} else if (f_0080 == f_9638) {
		w_082E |= 0x0002;
		w_00AA = 0x0001;
		w_00AB = 0x0001;
		f_0080 = f_D8BF;
	} else if (f_0080 == f_9662) {
		f_0080 = f_D8C7;
	} else if (f_0080 == f_96B0) {
		f_0080 = f_D8D4;
	} else if ((f_0080 == f_9239) || (f_0080 == f_95FB)) {
		w_082E |= 0x0001;
	} else if ((f_0080 == f_9356) || (f_0080 == f_974D)) {
		f_0F20 = f_42F8;	//unistall state machine hook
	}
}


//Those are machine state funtions.

//Technically they are not real functions, they are goto'ed to beginning
//address, but this is probably only optimisation. In most cases they can
//be treated as functions with parameter in k register containig counter
//(mainly negative) incremented before each 'call' - using mostly as delay.

//Following ones also goto at the end. I treat this as tail function call.

void f_D89B(int16_t k) {
	if (f_D8DE()) {
		f_91DE(k);
	} else {
		f_927C(k);
	}
}

void f_D8A3(int16_t k) {
	if (f_D8DE()) {
		f_91DE(k);
	} else {
		f_9292(k);
	}
}

void f_D8AB(int16_t k) {
	if (k < 0) {
		return;
	}

	if (f_D8DE()) {
		f_91DE(k);
	} else {
		f_92C0(k);
	}
}

void f_D8B5(int16_t k) {
	if (k < 0) {
		return;
	}

	if (f_D8DE()) {
		f_91DE(k);
	} else {
		f_9319(k);
	}
}

void f_D8BF(int16_t k) {
	if (f_D8DE()) {
		f_95A0(k);
	} else {
		f_963E(k);
	}
}

void f_D8C7(int16_t k) {
	if (k < 0) {
		return;
	}

	if (f_D8DE()) {
		f_95A0(k);
	} else {
		f_9673(k);
	}
}

void f_D8D4(int16_t k) {
	if (k < 0) {
		return;
	}

	if (f_D8DE()) {
		f_95A0(k);
	} else {
		f_96B8(k);
	}
}

//end of machine state functions

int16_t f_D8DE(void) {
	if (w_0CD8 == 0) {
		return 0;
	}

	w_0CDD = 0xFFFF;
	f_9D74(w_0083 ? 0x0003 : 0x000F);

	if (w_0CDC != w_0CDD) {
		w_0CD8 = 0x0000;
		return 0;
	} else {
		return w_0CDC;
	}
}
