#include <graphics.h>

#include "canvas.h"

void canvasinit() {
    int gdriver = DETECT, gmode;
    initgraph(&gdriver, &gmode, "C:\\TURBOC3\\BGI");
}

void canvasclose() {
    closegraph();
}

void canvasclear() {
    cleardevice();
}