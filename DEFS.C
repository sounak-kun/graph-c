#include <stdlib.h>
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

void addshape(union Shapes sh, enum ShapeType tp) {
    ShapesNode* front = drawnshapes;

    
    if (!drawnshapes) {     /* If first node */
        drawnshapes = (ShapesNode*) malloc(sizeof(ShapesNode));
        front = drawnshapes;
    } else {                /* If not first node */
        while (front->next) front = front->next;
        front->next = (ShapesNode*) malloc(sizeof(ShapesNode));
        front = front->next;
    }

    front->shape = sh;
    front->type = tp;
    front->next = NULL;
}