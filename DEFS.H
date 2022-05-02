#ifndef DEFS
    #define DEFS

    #define TRUE 1
    #define FALSE 0

    typedef enum Instruments {
        RULER,
        PROTACTOR,
        COMPASS
    } Instruments;

    typedef struct Point {
        float x;
        float y;
    } Point;

    typedef struct Line {
        Point a;
        Point b;
    } Line;

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

    extern Point currentpos;
    extern Instruments currentinstrument;
    extern ShapesNode* drawnshapes;

    void pushshape(union Shapes, enum ShapeType);
    void popshape();

    Point worldpoint(int, int);
    void pointworld(Point, int*, int*);
    int eqpoint(Point, Point);
#endif