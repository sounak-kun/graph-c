#include <stdio.h>
#include <conio.h>
#include "canvas.h"
#include "draw.h"
#include "mouse.h"
#include "defs.h"

Point currentpos;

void main() {
    int mousex, mousey, mouseclick;
    Point temppos;
    clrscr();
    canvasinit();
    drawgraph();
    mouseshow();
    do {
        mousepos(&mousex, &mousey, &mouseclick);
        temppos = worldpoint(mousex, mousey);
        if (currentpos.x != temppos.x || currentpos.y != temppos.y) {
            currentpos = temppos;
            drawstatus();
        }
    } while (!kbhit());
    canvasclose();
}