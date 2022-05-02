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
    int mousex, mousey, mouseclick, mousehold = FALSE;
    Point temppos, holdpos, oldpos;
    union Shapes tempshape;
    clrscr();
    canvasinit();
    drawgraph();
    mouseshow();

    /* Main event loop */
    do {
        mousepos(&mousex, &mousey, &mouseclick);
        temppos = worldpoint(mousex, mousey);

        /* If mouse position updated */
        if (!eqpoint(currentpos, temppos)) {
            oldpos = currentpos;    /* Keep track of the old position */
            currentpos = temppos;   /* Update current position */

            /* Things to update only on position update */
            switch (currentinstrument) {
                case RULER:
                    if (mousehold) {
                        canvasclear();
                        drawgraph();
                        drawshapes();
                        if (mouseclick) {
                            drawline(holdpos, currentpos);
                        } else {
                            tempshape.line = drawline(holdpos, oldpos);  /* Draw upto old position if mouse is not held anymore */
                            addshape(tempshape, SHAPE_LINE);
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
                }
                break;
        }
    } while (!kbhit());
    canvasclose();
}