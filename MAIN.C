#include <stdio.h>
#include <conio.h>
#include "canvas.h"
#include "graph.h"
#include "mouse.h"

void main() {
    int mx, my, mc;
    clrscr();
    canvasinit();
    mouseshow();
    do {
        mousepos(&mx, &my, &mc);
        printf("%d,%d", mx, my);
        gotoxy(1, 1);
    } while (!kbhit());
    canvasclose();
}