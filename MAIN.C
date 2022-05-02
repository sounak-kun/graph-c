#include <stdio.h>
#include <conio.h>
#include "canvas.h"
#include "draw.h"
#include "mouse.h"
#include "defs.h"

Point currentpos;
Instruments currentinstrument = RULER;
ShapesNode* drawnshapes = NULL;

void main() {
    int mousex, mousey, mouseclick, mousehold = FALSE, righthold = FALSE;
    int firstpointer = TRUE, firstdraw = TRUE;
    Point temppos, holdpos, oldpos;
    union Shapes tempshape;
    clrscr();
    canvasinit();
    drawgraph();

    /* Main event loop */
    do {
        mousepos(&mousex, &mousey, &mouseclick);
        temppos = worldpoint(mousex, mousey);

        /* If mouse position updated */
        if (!eqpoint(currentpos, temppos)) {
            oldpos = currentpos;    /* Keep track of the old position */
            currentpos = temppos;   /* Update current position */

            if (!firstpointer) {
                drawpointerxor(oldpos);     /* Remove pointer from old position */
            } else firstpointer = FALSE;
            drawpointerxor(currentpos);     /* Add pointer to new position */

            /* Things to update only on position update */
            switch (currentinstrument) {
                case RULER:
                    if (mousehold) {
                        if (mouseclick) {
                            if (!firstdraw) {
                                drawlinexor(holdpos, oldpos);   /* Remove old line */
                            } else firstdraw = FALSE;
                            drawlinexor(holdpos, currentpos);   /* Add new line */
                        } else {
                            tempshape.line = drawline(holdpos, oldpos);  /* Draw upto old position if mouse is not held anymore */
                            pushshape(tempshape, SHAPE_LINE);
                            mousehold = FALSE;
                        }
                    }
                    break;
            }
            drawstatus();
        }

        /* Things to update every frame */
        switch (currentinstrument) {
            case RULER:
                if (!mousehold && mouseclick) {
                    holdpos = currentpos;
                    mousehold = TRUE;
                    firstdraw = TRUE;
                }
                break;
        }

        /* Remove last shape on right click */
        if (mouseclick == 2) {
            if (!righthold) {
                popshape();
                canvasclear();
                drawgraph();
                drawshapes();
                drawpointerxor(currentpos);
                drawstatus();
                righthold = TRUE;
            }
        } else righthold = FALSE;
    } while (!kbhit());
    canvasclose();
}