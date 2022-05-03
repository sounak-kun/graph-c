#include <math.h>

#include "defs.h"
#include "canvas.h"

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
    int alt = current < 0 ? current + 360 : current - 360;
    return abs(current - old) < abs(alt - old) ? current : alt;
}