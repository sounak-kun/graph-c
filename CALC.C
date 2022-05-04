#include <math.h>

#include "defs.h"
#include "canvas.h"
#include "calc.h"

float round(float fn) {
    float in = (int) fn;
    if ((fn - in) > 0.5) return in + 1.0;
    return in; 
}

Point worldpoint(int x, int y) {
    Point p;
    p.x = round(x / 4.0) / 10.0;
    p.y = round((CANVASY - y) / 4.0) / 10.0;
    return p;
}

void pointworld(Point p, int* x, int* y) {
    *x = p.x * 40;
    *y = CANVASY - (p.y * 40);
}

int eqpoint(Point a, Point b) {
    return (a.x == b.x && a.y == b.y);
}

float distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int slope(Point a, Point b) {
    return atan2(b.y - a.y, b.x - a.x) * 180.0/M_PI;
}

int relativeangle(int current, int old) {
    int minangle = current, mingap = abs(current - old);
    if (abs(current - old + 360) < mingap) {
        mingap = abs(current - old + 360);
        minangle = current + 360;
    }
    if (abs(current - old - 360) < mingap) minangle = current - 360;
    return minangle;
}