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
        float relangle;
    } Arc;

    extern Point currentpos;
    extern Instruments currentinstrument;
#endif