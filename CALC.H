#ifndef CALC
    #define CALC
    #include "defs.h"

    struct Intersections {
        int count;
        Point points[2];
    };

    extern struct Intersections intersections;

    Point worldpoint(int, int);
    void pointworld(Point, int*, int*);
    int eqpoint(Point, Point);
    float distance(Point, Point);
    float slope(Point, Point);
    float relativeangle(float, float);
    bool approxdiv(float, float, float);
    float roundton(float, float);
    Point vector(float, float, Point);
    void linexline(Line, Line);
    void linexarc(Line, Arc);
    void arcxarc(Arc, Arc);
#endif