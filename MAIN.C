#include <stdio.h>
#include <conio.h>

#include "defs.h"
#include "canvas.h"
#include "mouse.h"
#include "draw.h"
#include "store.h"


Point currentpos;
Instruments currentinstrument = RULER;
struct ShapesList shapeslist;

void refresh();

void main() {
    int mousex, mousey, mousec, mousehold;
    bool drawruler = FALSE, firstpointer = TRUE, firstruler = TRUE;
    Point temppos, oldpos, holdstartpos, holdendpos;
    union Shapes tempshape;

    canvasinit();
    drawgraph();
    storeinit();

    /* Event loop */
    do {
        mousepos(&mousex, &mousey, &mousec);
        temppos = worldpoint(mousex, mousey);

        /* Things to update only on position update */
        if (!eqpoint(currentpos, temppos)) {
            oldpos = currentpos;
            currentpos = temppos;

            switch (currentinstrument) {
                case RULER:
                    if (drawruler) {
                        if (!firstruler) {      /* Don't trigger XOR on first draw */
                            drawlinexor(holdstartpos, oldpos);  /* Remove old line using XOR */
                        } else firstruler = FALSE;
                        drawlinexor(holdstartpos, currentpos);
                    }
                    break;
            }

            if (!firstpointer) {            /* Don't trigger XOR on first run */
                drawpointerxor(oldpos);     /* Remove old pointer via XOR */
            } else firstpointer = FALSE;
            drawpointerxor(currentpos);

            drawstatus();
        }

        /* Things to update on every frame */
        switch (currentinstrument) {
            case RULER:
                if (mousec == 1 && !mousehold) {
                    drawruler = TRUE;
                    firstruler = TRUE;
                } else if (mousec == 0 && drawruler) {
                    tempshape.line = drawline(holdstartpos, currentpos);
                    storepush(tempshape, SHAPE_LINE);
                    refresh();
                    drawruler = FALSE;
                }
                break;
        }

        /* Undo last shape on right click */
        if (mousec == 2 && !mousehold) {
            storepop();
            refresh();
        }

        /* Calculate mouse hold */
        if (mousec && !mousehold) holdstartpos = currentpos;
        if (!mousec && mousehold) holdendpos = currentpos;
        mousehold = mousec;
    } while (!kbhit());

    canvasclose();
}

void refresh() {
    canvasclear();
    drawgraph();
    drawshapes();
    drawpointerxor(currentpos);
    drawstatus();
}