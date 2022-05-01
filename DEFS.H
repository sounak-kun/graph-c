#ifndef DEFS
    #define DEFS

    #define TRUE 1
    #define FALSE 0

    typedef struct Point {
        float x;
        float y;
    } Point;

    typedef struct Line {
        Point a;
        Point b;
    } Line;

    extern Point currentpos;

    Point worldpoint(int, int);
#endif