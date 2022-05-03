#ifndef DRAW
    #define DRAW
    #include "defs.h"

    void drawgraph();
    void drawstatus();
    void drawshapes();
    void drawpointerxor(Point);
    Line drawline(Point, Point);
    void drawlinexor(Point, Point);
    Arc drawarc(Point, float, int, int);
    void drawpoint(Point);
#endif