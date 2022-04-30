#ifndef DRAW
    #define DRAW

    typedef struct Point {
        int x;
        int y;
    } Point;

    typedef struct Line {
        Point a;
        Point b;
    } Line;

    void graphadd();
    Line lineadd(Point, Point);
#endif