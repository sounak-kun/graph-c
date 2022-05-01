#include <graphics.h>
#include <stdio.h>
#include "canvas.h"
#include "draw.h"
#include "defs.h"

#define STATUSX 80
#define STATUSY 120

void drawgraph() {
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

void drawstatus() {
    int points[8] = {CANVASX - STATUSX, 0, CANVASX - STATUSX, STATUSY, CANVASX, STATUSY, CANVASX, 0};
    char postext[16];
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    fillpoly(4, points);
    setcolor(BLACK);
    outtextxy(CANVASX - STATUSX + 10, 8, "Pos:");
    sprintf(postext, "%.1f,%.1f", currentpos.x, currentpos.y);
    outtextxy(CANVASX - STATUSX + 10, 20, postext);
}

Line drawline(Point a, Point b) {
    Line l;
    l.a = a; l.b = b;
    setcolor(WHITE);
    line(a.x, a.y, b.x, b.y);
    return l;
}