#include "defs.h"
#include "canvas.h"

Point worldpoint(int x, int y) {
    Point p;
    p.x = (x / 4) / 10.0;
    p.y = ((CANVASY - y) / 4) / 10.0;
    return p;
}