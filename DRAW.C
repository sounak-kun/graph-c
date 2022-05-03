#include <graphics.h>
#include <stdio.h>

#include "defs.h"
#include "canvas.h"
#include "draw.h"
#include "store.h"

#define CROSSSIZE 8
#define POINTSIZE 2

void addinstruments(Instruments, char*);

void drawgraph() {
    int x, y;
    setcolor(DARKGRAY);
    for (x = 40; x < CANVASX; x += 40) {
        line(x, 0, x, CANVASY);
    }
    for (y = 40; y < CANVASY; y += 40) {
        line(0, y, CANVASX, y);
    }
}

void drawstatus() {
    char text[16];
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(CANVASX - STATUSX, 0, CANVASX, STATUSY);
    setcolor(WHITE);
    outtextxy(CANVASX - STATUSX + 10, 8, "POS:");
    sprintf(text, "%.1f,%.1f", currentpos.x, currentpos.y);
    outtextxy(CANVASX - STATUSX + 10, 20, text);
    if (extendstatus) {
        outtextxy(CANVASX - STATUSX + 10, 32, extendstatustext);
        sprintf(text, "%.2f", extendstatusnum);
        outtextxy(CANVASX - STATUSX + 10, 44, text);
    }
    addinstruments(RULER, "RULER");
    addinstruments(PROTACTOR, "PROTACTOR");
    addinstruments(COMPASS, "COMPASS");
}

void addinstruments(Instruments ins, char* name) {
    if (currentinstrument == ins) {
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        setcolor(BLACK);
    } else {
        setfillstyle(SOLID_FILL, DARKGRAY);
        setcolor(WHITE);
    }
    bar(CANVASX - STATUSX, STATUS_INDICATOR(ins), CANVASX, STATUS_INDICATOR(ins) + STATUSCONTROLS);
    outtextxy(CANVASX - STATUSX + 10, STATUS_INDICATOR(ins) + 8, name);
}

Line drawline(Point a, Point b) {
    Line l;
    int ax, ay, bx, by;
    pointworld(a, &ax, &ay);
    pointworld(b, &bx, &by);
    setcolor(WHITE);
    line(ax, ay, bx, by);
    l.a = a; l.b = b;
    return l;
}

void drawlinexor(Point a, Point b) {
    setwritemode(XOR_PUT);
    drawline(a, b);
    setwritemode(COPY_PUT);
}

Arc drawarc(Point origin, float radius, int startangle, int endangle) {
    Arc c;
    int x, y, temp;
    pointworld(origin, &x, &y);
    if (endangle < startangle) {
        temp = startangle;
        startangle = endangle;
        endangle = temp;
    }
    setcolor(WHITE);
    arc(x, y, startangle, endangle, radius * 40.0);
    c.origin = origin;
    c.radius = radius;
    c.startangle = startangle;
    c.endangle = endangle;
    return c;
}

void drawshapes() {
    int i;
    for (i = 0; i < shapeslist.filled; i++) {
        switch (shapeslist.type[i]) {
            case SHAPE_LINE:
                drawline(shapeslist.shapes[i].line.a, shapeslist.shapes[i].line.b);
                break;

            case SHAPE_ARC:
                drawarc(shapeslist.shapes[i].arc.origin, shapeslist.shapes[i].arc.radius,
                    shapeslist.shapes[i].arc.startangle, shapeslist.shapes[i].arc.endangle);
                break;
        }
    }
}

void drawpointerxor(Point p) {
    int x, y;
    setcolor(LIGHTRED);
    setwritemode(XOR_PUT);

    pointworld(p, &x, &y);
    line(x - CROSSSIZE, y, x + CROSSSIZE, y);
    line(x, y - CROSSSIZE, x, y + CROSSSIZE);

    setwritemode(COPY_PUT);
}

void drawpoint(Point p) {
    int x, y;
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);

    pointworld(p, &x, &y);
    pieslice(x, y, 0, 360, POINTSIZE);

    circle(x, y, POINTSIZE + 2);
}