#include <stdlib.h>

#include "store.h"

#define LIST_INIT_SIZE 10
#define LIST_SIZE_INTERVAL 5

void storeinit() {
    shapeslist.size = LIST_INIT_SIZE;
    shapeslist.filled = 0;
    shapeslist.shapes = (union Shapes*) malloc(sizeof(union Shapes) * LIST_INIT_SIZE);
    shapeslist.type = (enum ShapeType*) malloc(sizeof(enum ShapeType) * LIST_INIT_SIZE);
}

void storepush(union Shapes sh, enum ShapeType tp) {
    /* Allocate more memory if needed */
    if (shapeslist.size == shapeslist.filled) {
        shapeslist.shapes = (union Shapes*) realloc(shapeslist.shapes, sizeof(union Shapes) * (shapeslist.size + LIST_SIZE_INTERVAL));
        shapeslist.type = (enum ShapeType*) realloc(shapeslist.type, sizeof(enum ShapeType) * (shapeslist.size + LIST_SIZE_INTERVAL));
        shapeslist.size += LIST_SIZE_INTERVAL;
    }

    shapeslist.shapes[shapeslist.filled] = sh;
    shapeslist.type[shapeslist.filled] = tp;
    shapeslist.filled++;
}

void storepop() {
    if (shapeslist.filled) shapeslist.filled--;
}