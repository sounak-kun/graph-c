#ifndef STORE
    #define STORE
    #include "defs.h"

    union Shapes {
        Line line;
    };

    enum ShapeType {
        SHAPE_LINE
    };

    typedef struct ShapesNode {
        union Shapes shape;
        enum ShapeType type;
        struct ShapesNode* next;
    } ShapesNode;

    extern ShapesNode* drawnshapes;

    void pushshape(union Shapes, enum ShapeType);
    void popshape();
#endif