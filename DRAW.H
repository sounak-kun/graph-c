#ifndef DRAW
    #define DRAW
    #include "defs.h"
    #include "canvas.h"

    #define STATUSX 100
    #define STATUSY 60
    #define STATUSGAPS 10
    #define STATUSCONTROLS 20

    #define STATUS_INDICATOR(n) (STATUSY + STATUSGAPS + (STATUSGAPS + STATUSCONTROLS) * n)

    #define STATUSEDGEX CANVASX - STATUSX
    #define STATUSEDGEY STATUS_INDICATOR(COMPASS) + STATUSCONTROLS

    extern bool extendstatus;
    extern char* extendstatustext;
    extern float extendstatusnum;

    void drawgraph();
    void drawstatus();
    void drawshapes();
    void drawpointerxor(Point);
    Line drawline(Point, Point);
    void drawlinexor(Point, Point);
    Arc drawarc(Point, float, int, int);
    void drawpoint(Point);
#endif