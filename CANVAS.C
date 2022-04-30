#include <graphics.h>
#include "canvas.h"

int canvasinit() {
    int gdriver = DETECT, gmode;
    initgraph(&gdriver, &gmode, "C:\\TURBOC3\\BGI");
}

int canvasclose() {
    closegraph();
}