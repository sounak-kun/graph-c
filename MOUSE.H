#ifndef MOUSE
    #define MOUSE

    enum Click {
        CLICK_NONE,
        CLICK_LEFT,
        CLICK_RIGHT,
        CLICK_MIDDLE
    };

    void mouseshow();
    void mousehide();
    void mousepos(int*, int*, int*);
    void mousesetpos(int, int);
#endif