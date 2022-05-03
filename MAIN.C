#include <stdio.h>
#include <conio.h>

#include "defs.h"
#include "canvas.h"
#include "mouse.h"
#include "draw.h"
#include "store.h"


Point currentpos;
Instruments currentinstrument = COMPASS;
struct ShapesList shapeslist;

void refresh();

void main() {
    int mousex, mousey, mousec, mousehold, compassstartangle, compassoldangle;
    float compassradius;
    bool drawruler = FALSE, firstpointer = TRUE, firstruler = TRUE, compassoriginset = FALSE, drawcompass = FALSE;
    Point temppos, oldpos, holdstartpos, holdendpos, compassorigin;
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

                case COMPASS:
                    if (drawcompass) {
                        refresh();
                        drawarc(compassorigin, compassradius, compassstartangle,
                            compassoldangle = relativeangle(slope(compassorigin, currentpos), compassoldangle));
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
                } else if (!mousec && drawruler) {
                    tempshape.line = drawline(holdstartpos, currentpos);
                    storepush(tempshape, SHAPE_LINE);
                    refresh();
                    drawruler = FALSE;
                }
                break;

            case COMPASS:
                /* Set compass origin on first click */
                if (!compassoriginset && mousec == 1 && !mousehold) {
                    compassorigin = currentpos;
                    compassoriginset = TRUE;
                } else if (compassoriginset && mousec == 1 && !mousehold) {     /* Set compass radius and start drawing on hold */
                    drawcompass = TRUE;
                    compassradius = distance(compassorigin, currentpos);
                    compassstartangle = slope(compassorigin, currentpos);
                    compassoldangle = compassstartangle;
                } else if (compassoriginset && !mousec && drawcompass) {        /* Push arc into store once hold is over */
                    tempshape.arc = drawarc(compassorigin, compassradius, compassstartangle, compassoldangle);
                    storepush(tempshape, SHAPE_ARC);
                    refresh();
                    drawcompass = FALSE;
                };
                if (compassoriginset) {
                    drawpoint(compassorigin);
                    if (mousec == 2 && !mousehold) {
                        compassoriginset = FALSE;
                        refresh();
                        mousec = -1;    /* Lock mouse clicks for the rest of the frame */
                    }
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