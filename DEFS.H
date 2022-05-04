#ifndef DEFS
    #define DEFS

    typedef enum Bool {
        FALSE,
        TRUE
    } bool;

    typedef enum Instruments {
        RULER,
        PROTRACTOR,
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

    typedef struct Arc {
        Point origin;
        float radius;
        float startangle;
        float endangle;
    } Arc;

    extern Point currentpos;
    extern Instruments currentinstrument;

    Point worldpoint(int, int);
    void pointworld(Point, int*, int*);
    int eqpoint(Point, Point);
    float distance(Point, Point);
    int slope(Point, Point);
    int relativeangle(int, int);
#endif