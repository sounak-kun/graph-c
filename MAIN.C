#include <stdio.h>
#include <conio.h>
#include "canvas.h"
#include "draw.h"
#include "mouse.h"

void main() {
    int mx, my, mc;
    clrscr();
    canvasinit();
    graphadd();
    mouseshow();
    do {
        mousepos(&mx, &my, &mc);
        printf("%d,%d", mx, my);
        gotoxy(1, 1);
    } while (!kbhit());
    canvasclose();
}