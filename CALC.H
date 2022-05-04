#ifndef CALC
    #define CALC
    #include "defs.h"

    Point worldpoint(int, int);
    void pointworld(Point, int*, int*);
    int eqpoint(Point, Point);
    float distance(Point, Point);
    int slope(Point, Point);
    int relativeangle(int, int);
    bool approxdiv(float, float, float);
    float roundton(float, float);
    Point vector(float, float, Point);
#endif