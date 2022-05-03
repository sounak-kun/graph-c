#ifndef STORE
    #define STORE
    #include "defs.h"

    union Shapes {
        Line line;
        Arc arc;
    };

    enum ShapeType {
        SHAPE_LINE,
        SHAPE_ARC
    };

    struct ShapesList {
        int size;
        int filled;
        union Shapes* shapes;
        enum ShapeType* type;
    };

    extern struct ShapesList shapeslist;

    void storeinit();
    void storepush(union Shapes, enum ShapeType);
    void storepop();
#endif