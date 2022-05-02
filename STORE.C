#include <stdlib.h>
#include "store.h"

void pushshape(union Shapes sh, enum ShapeType tp) {
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

void popshape() {
    ShapesNode *first, *second;
    first = drawnshapes;
    if (drawnshapes && drawnshapes->next) {
        while (TRUE) {
            second = first;
            first = first->next;
            if (!first->next) break;
        }
        free(first);
        second->next = NULL;
    } else if (drawnshapes) {
        free(drawnshapes);
        drawnshapes = NULL;
    }
}