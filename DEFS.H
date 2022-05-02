#ifndef DEFS
    #define DEFS

    #define TRUE 1
    #define FALSE 0

    typedef enum Instruments {
        RULER,
        PROTACTOR,
        COMPASS
    } Instruments;

    typedef struct Point {
        float x;
        float y;
    } Point;

    typedef struct Line {
        Point a;
        Point b;
    } Line;

    extern Point currentpos;
    extern Instruments currentinstrument;

    Point worldpoint(int, int);
    void pointworld(Point, int*, int*);
    int eqpoint(Point, Point);
#endif