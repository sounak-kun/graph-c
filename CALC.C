#include <math.h>

#include "defs.h"
#include "canvas.h"
#include "calc.h"

#define SQ(n) ((n) * (n))

float round(float fn) {
    float in = floor(fn);
    if ((fn - in) > 0.5) return in + 1.0;
    return in; 
}

int mod(int a, int b) {
    while (a < 0) a += b;
    return a % b;
}

float floatmod(float a, float b) {
    while (a < 0) a += b;
    return fmod(a ,b);
}

float vcross(float ax, float ay, float bx, float by) {
    return ax * by - bx * ay;
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
    return sqrt(SQ(a.x - b.x) + SQ(a.y - b.y));
}

float slope(Point a, Point b) {
    return atan2(b.y - a.y, b.x - a.x) * 180.0 / M_PI;
}

float relativeangle(float current, float old) {
    float r = floatmod(current, 360) - floatmod(old, 360);
    if (r >  180) return r - 360;
    if (r < -180) return r + 360;
    return r;
}

bool approxdiv(float a, float b, float th) {
    a = floatmod(a, b);
    return a <= th || a >= b - th;
}

float roundton(float num, float base) {
    return round(num / base) * base;
}

Point vector(float angle, float distance, Point start) {
    Point q;
    q.x = start.x + distance * cos(angle / 180.0 * M_PI);
    q.y = start.y + distance * sin(angle / 180.0 * M_PI);
    return q;
}

void linexline(Line p, Line q) {
    float dx, dy, rx, ry, sx, sy, den, t, u;
    intersections.count = 0;    /* By default consider there are 0 intersections */

    rx = p.b.x - p.a.x;
    ry = p.b.y - p.a.y;
    sx = q.b.x - q.a.x;
    sy = q.b.y - q.a.y;
    den = vcross(rx, ry, sx, sy);
    if (den == 0) return;

    dx = q.a.x - p.a.x;
    dy = q.a.y - p.a.y;
    t = vcross(dx, dy, sx, sy) / den;
    if (t < 0 || t > 1) return;

    u = vcross(dx, dy, rx, ry) / den;
    if (u < 0 || u > 1) return;

    intersections.count = 1;
    intersections.points[0].x = p.a.x + t * rx;
    intersections.points[0].y = p.a.y + t * ry;
}

void linexarc(Line p, Arc q) {
    float qa, qb, qc, disc,  ua, ub, angle;
    intersections.count = 0;

    qa = SQ(p.b.x - p.a.x) + SQ(p.b.y - p.a.y);
    qb = 2 * ((p.b.x - p.a.x) * (p.a.x - q.origin.x) + (p.b.y - p.a.y) * (p.a.y - q.origin.y));
    qc = SQ(q.origin.x - p.a.x) + SQ(q.origin.y - p.a.y) - SQ(q.radius);

    disc = SQ(qb) - 4 * qa * qc;
    if (disc < 0) return;

    if (disc == 0) {
        ua = (- qb) / (2 * qa);
        intersections.points[0].x = p.a.x + ua * (p.b.x - p.a.x);
        intersections.points[0].y = p.a.y + ua * (p.b.y - p.a.y);
        angle = relativeangle(slope(q.origin, intersections.points[0]), q.startangle);
        if (((angle - q.relangle) * angle) <= 0) intersections.count = 1;
        return;
    }

    disc = sqrt(disc);
    ua = ((- qb) + disc) / (2 * qa);
    ub = ((- qb) - disc) / (2 * qa);
    if (!(ua < 0 || ua > 1)) {
        intersections.points[intersections.count].x = p.a.x + ua * (p.b.x - p.a.x);
        intersections.points[intersections.count].y = p.a.y + ua * (p.b.y - p.a.y);
        angle = relativeangle(slope(q.origin, intersections.points[intersections.count]), q.startangle);
        if (((angle - q.relangle) * angle) <= 0) intersections.count++;
    }
    if (!(ub < 0 || ub > 1)) {
        intersections.points[intersections.count].x = p.a.x + ub * (p.b.x - p.a.x);
        intersections.points[intersections.count].y = p.a.y + ub * (p.b.y - p.a.y);
        angle = relativeangle(slope(q.origin, intersections.points[intersections.count]), q.startangle);
        if (((angle - q.relangle) * angle) <= 0) intersections.count++;
    }
}

void arcxarc(Arc p, Arc q) {
    float d, a, h, x2, y2, angle;
    intersections.count = 0;

    d = distance(p.origin, q.origin);
    if ((d > (p.radius + q.radius)) || (d < fabs(p.radius - q.radius)) || (d == 0 && p.radius == q.radius)) return;

    a = (SQ(p.radius) - SQ(q.radius) + SQ(d)) / (2 * d);
    h = sqrt(SQ(p.radius) - SQ(a));

    x2 = p.origin.x + a * (q.origin.x - p.origin.x) / d;
    y2 = p.origin.y + a * (q.origin.y - p.origin.y) / d;

    if (d == (p.radius + q.radius) || d == fabs(p.radius - q.radius)) {
        intersections.points[0].x = x2 + h * (q.origin.y - p.origin.y) / d;
        intersections.points[0].y = y2 + h * (q.origin.x - p.origin.x) / d;
        angle = relativeangle(slope(p.origin, intersections.points[0]), p.startangle);
        if (((angle - p.relangle) * angle) <= 0) {
            angle = relativeangle(slope(q.origin, intersections.points[0]), q.startangle);
            if (((angle - q.relangle) * angle) <= 0) intersections.count = 1;
        }
        return;
    }

    intersections.points[intersections.count].x = x2 + h * (q.origin.y - p.origin.y) / d;
    intersections.points[intersections.count].y = y2 + h * (q.origin.x - p.origin.x) / d;
    angle = relativeangle(slope(p.origin, intersections.points[intersections.count]), p.startangle);
    if (((angle - p.relangle) * angle) <= 0) {
        angle = relativeangle(slope(q.origin, intersections.points[intersections.count]), q.startangle);
        if (((angle - q.relangle) * angle) <= 0) intersections.count++;
    }

    intersections.points[intersections.count].x = x2 - h * (q.origin.y - p.origin.y) / d;
    intersections.points[intersections.count].y = y2 - h * (q.origin.x - p.origin.x) / d;
    angle = relativeangle(slope(p.origin, intersections.points[intersections.count]), p.startangle);
    if (((angle - p.relangle) * angle) <= 0) {
        angle = relativeangle(slope(q.origin, intersections.points[intersections.count]), q.startangle);
        if (((angle - q.relangle) * angle) <= 0) intersections.count++;
    }
}