#include <stdio.h>
#include <conio.h>

#include "defs.h"
#include "canvas.h"
#include "mouse.h"
#include "draw.h"
#include "store.h"

#define CLICK(n) (##mousec == n && !##mousehold)

Point currentpos;
Instruments currentinstrument = RULER;
struct ShapesList shapeslist;
bool extendstatus = FALSE;
char* extendstatustext = "";
float extendstatusnum = 0.0;

void refresh();

void main() {
    int mousex, mousey, mousec, mousehold, compassstartangle, compassoldangle;
    float compassradius;
    bool drawruler = FALSE, firstpointer = TRUE, firstruler = TRUE, compassoriginset = FALSE, drawcompass = FALSE, mousevisible = FALSE,
        drawprotractor = FALSE, firstprotractor = TRUE;
    Point temppos, oldpos, holdstartpos, holdendpos, compassorigin;
    union Shapes tempshape;

    canvasinit();
    drawgraph();
    storeinit();
    mousesetpos(CANVASX / 2, CANVASY / 2);

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
                        extendstatus = TRUE;
                        extendstatustext = "LENGTH:";
                        extendstatusnum = distance(holdstartpos, currentpos);
                        if (!firstruler) {      /* Don't trigger XOR on first draw */
                            drawlinexor(holdstartpos, oldpos);  /* Remove old line using XOR */
                        } else firstruler = FALSE;
                        drawlinexor(holdstartpos, currentpos);
                    }
                    break;

                case PROTRACTOR:
                    if (drawprotractor) {
                        extendstatus = TRUE;
                        extendstatustext = "ANGLE:";
                        extendstatusnum = slope(holdstartpos, currentpos);
                        if (!firstprotractor) {     /* Don't trigger XOR on first draw */
                            drawlinexor(holdstartpos, oldpos);  /* Remove old line using XOR */
                        } else firstprotractor = FALSE;
                        drawlinexor(holdstartpos, currentpos);
                    }

                case COMPASS:
                    if (compassoriginset) {
                        extendstatus = TRUE;
                        extendstatustext = "RADIUS:";
                        extendstatusnum = distance(compassorigin, currentpos);
                    }
                    if (drawcompass) {
                        refresh();
                        drawarc(compassorigin, compassradius, compassstartangle,
                            compassoldangle = relativeangle(slope(compassorigin, currentpos), compassoldangle));
                    }
                    break;
            }

            /* if mouse inside status range */
            if (mousex > STATUSEDGEX && mousey < STATUSEDGEY) {
                if (!mousevisible) {
                    drawpointerxor(oldpos);     /* Hide the pointer */
                    mouseshow();
                    mousevisible = TRUE;
                }
            } else {
                if (mousevisible) {
                    mousehide();
                    mousevisible = FALSE;
                    firstpointer = TRUE;        /* Prevent additional XOR */
                }

                if (!firstpointer) {            /* Don't trigger XOR on first run */
                    drawpointerxor(oldpos);     /* Remove old pointer via XOR */
                } else firstpointer = FALSE;
                drawpointerxor(currentpos);
            }
            
            drawstatus();
        }

        if (mousevisible) extendstatus = FALSE; /* Hide extended status if pointer on switcher */

        if (mousevisible && CLICK(CLICK_LEFT)) {
            if (mousey > STATUS_INDICATOR(COMPASS)) currentinstrument = COMPASS;
            else if (mousey > STATUS_INDICATOR(PROTRACTOR)) currentinstrument = PROTRACTOR;
            else if (mousey > STATUS_INDICATOR(RULER)) currentinstrument = RULER;
            refresh();
            mousec = -1;    /* Prevent click actions for the rest of the frame */
        }

        /* Things to update on every frame */
        switch (currentinstrument) {
            case RULER:
                if (CLICK(CLICK_LEFT)) {
                    drawruler = TRUE;
                    firstruler = TRUE;
                } else if (!mousec && drawruler) {
                    tempshape.line = drawline(holdstartpos, currentpos);
                    storepush(tempshape, SHAPE_LINE);
                    refresh();
                    drawruler = FALSE;
                    extendstatus = FALSE;
                }
                break;

            case PROTRACTOR:
                if (CLICK(CLICK_LEFT)) {
                    drawprotractor = TRUE;
                    firstprotractor = TRUE;
                } else if (!mousec && drawprotractor) {
                    tempshape.line = drawline(holdstartpos, currentpos);
                    storepush(tempshape, SHAPE_LINE);
                    refresh();
                    drawprotractor = FALSE;
                    extendstatus = FALSE;
                }
                break;

            case COMPASS:
                /* Set compass origin on first click */
                if (!compassoriginset && CLICK(CLICK_LEFT)) {
                    compassorigin = currentpos;
                    compassoriginset = TRUE;
                } else if (compassoriginset && CLICK(CLICK_LEFT)) {     /* Set compass radius and start drawing on hold */
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
                    if (CLICK(CLICK_RIGHT)) {
                        compassoriginset = FALSE;
                        extendstatus = FALSE;
                        refresh();
                        mousec = -1;    /* Lock mouse clicks for the rest of the frame */
                    }
                }
                break;
        }

        /* Undo last shape on right click */
        if (CLICK(CLICK_RIGHT)) {
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