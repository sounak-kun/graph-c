#include <graphics.h>
#include "canvas.h"
#include "draw.h"

Line lineadd(Point a, Point b) {
    Line l;
    l.a = a; l.b = b;
    line(a.x, a.y, b.x, b.y);
    return l;
}

void graphadd() {
    int x, y;

    setcolor(LIGHTGRAY);
    for (x = 40; x < CANVASX; x += 40) {
        line(x, 0, x, CANVASY);
    }
    for (y = 40; y < CANVASY; y += 40) {
        line(0, y, CANVASX, y);
    }

    setcolor(DARKGRAY);
    for (x = 20; x < CANVASX; x += 40) {
        line(x, 0, x, CANVASY);
    }
    for (y = 20; y < CANVASY; y += 40) {
        line(0, y, CANVASX, y);
    }
}