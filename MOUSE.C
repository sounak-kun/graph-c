#include <dos.h>

#include "mouse.h"

union REGS in, out;

void mouseshow() {
    in.x.ax = 1;
    int86(51, &in, &out);
}

void mousehide() {
    in.x.ax = 2;
    int86(51, &in, &out);
}

void mousepos(int* x, int* y, int* c) {
    in.x.ax = 3;
    int86(51, &in, &out);
    *x = out.x.cx;
    *y = out.x.dx;
    *c = out.x.bx;
}