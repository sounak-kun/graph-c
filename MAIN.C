#include <stdio.h>
#include <conio.h>

#include "defs.h"
#include "canvas.h"
#include "mouse.h"
#include "draw.h"
#include "store.h"
#include "calc.h"

#define RULERNATURALINT 1
#define RULERATTR 0.15
#define PROTRACTORNATURALINT 15
#define PROTRACTORATTR 2
#define COMPASSNATURALINT 1
#define COMPASSATTR 0.2

#define CLICK(n) (##mousec == n && !##mousehold)

Point currentpos;
Instruments currentinstrument = RULER;
struct ShapesList shapeslist;
struct Intersections intersections;
bool extendstatus = FALSE;
char* extendstatustext = "";
float extendstatusnum = 0.0;

void refresh();

void main() {
    int mousex, mousey, mousec, mousehold;
    float compassradius, compassstartangle, compassrelangle, relangletemp, attrval;
    bool drawruler = FALSE, firstpointer = TRUE, firstruler = TRUE, compassoriginset = FALSE, drawcompass = FALSE, mousevisible = FALSE,
        drawprotractor = FALSE, firstprotractor = TRUE;
    Point temppos, oldpos, holdstartpos, holdendpos, compassorigin, attrpoint, drawoldpos;
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
                        attrval = distance(holdstartpos, currentpos);
                        if (approxdiv(attrval, RULERNATURALINT, RULERATTR)) attrval = roundton(attrval, RULERNATURALINT);
                        extendstatusnum = attrval;
                        attrpoint = vector(slope(holdstartpos, currentpos), attrval, holdstartpos);
                        if (!firstruler) {      /* Don't trigger XOR on first draw */
                            drawlinexor(holdstartpos, drawoldpos);  /* Remove old line using XOR */
                        } else firstruler = FALSE;
                        drawlinexor(holdstartpos, attrpoint);
                        drawoldpos = attrpoint;
                    }
                    break;

                case PROTRACTOR:
                    if (drawprotractor) {
                        extendstatus = TRUE;
                        extendstatustext = "ANGLE:";
                        attrval = slope(holdstartpos, currentpos);
                        if (approxdiv(attrval, PROTRACTORNATURALINT, PROTRACTORATTR)) attrval = roundton(attrval, PROTRACTORNATURALINT);
                        extendstatusnum = attrval;
                        attrpoint = vector(attrval, distance(holdstartpos, currentpos), holdstartpos);
                        if (!firstprotractor) {     /* Don't trigger XOR on first draw */
                            drawlinexor(holdstartpos, drawoldpos);  /* Remove old line using XOR */
                        } else firstprotractor = FALSE;
                        drawlinexor(holdstartpos, attrpoint);
                        drawoldpos = attrpoint;
                    }

                case COMPASS:
                    if (compassoriginset) {
                        extendstatus = TRUE;
                        extendstatustext = "RADIUS:";
                        if (drawcompass) {
                            extendstatusnum = compassradius;
                        } else {
                            attrval = distance(compassorigin, currentpos);
                            if (approxdiv(attrval, COMPASSNATURALINT, COMPASSATTR)) attrval = roundton(attrval, COMPASSNATURALINT);
                            extendstatusnum = attrval;
                        }
                    }
                    if (drawcompass) {
                        relangletemp = relativeangle(slope(compassorigin, currentpos), compassstartangle + compassrelangle);
                        if ((compassrelangle * relangletemp) < 0) {     /* Trigger refresh if both have opposite sign */
                            refresh();
                            firstpointer = TRUE;                        /* Prevent old position overlap */
                            drawpointerxor(currentpos);                 /* Prevent current position from being empty */
                        }
                        drawarc(compassorigin, compassradius, compassstartangle, compassrelangle += relangletemp);
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
                    tempshape.line = drawline(holdstartpos, drawoldpos);
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
                    tempshape.line = drawline(holdstartpos, drawoldpos);
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
                    attrval = distance(compassorigin, currentpos);
                    if (approxdiv(attrval, COMPASSNATURALINT, COMPASSATTR)) attrval = roundton(attrval, COMPASSNATURALINT);
                    compassradius = attrval;
                    compassstartangle = slope(compassorigin, currentpos);
                    compassrelangle = 0;
                } else if (compassoriginset && !mousec && drawcompass) {        /* Push arc into store once hold is over */
                    tempshape.arc = drawarc(compassorigin, compassradius, compassstartangle, compassrelangle);
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