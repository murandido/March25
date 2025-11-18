#include "../include/menu.h"

void drawBorderWindow(WINDOW *borderWindow, int mainBlockW, int menuW, int menuSuppW, int topRowH) {
    // draw outline border
    box(borderWindow, 0, 0);

    // draw vertical lines
    mvwvline(borderWindow, 1, menuW, ACS_VLINE, topRowH - 1);
    mvwvline(borderWindow, 1, menuW + menuSuppW, ACS_VLINE, topRowH - 1);

    // draw horizontal line
    mvwhline(borderWindow, topRowH, 1, ACS_HLINE, mainBlockW - 2);

    // draw "┴" connections
    mvwaddch(borderWindow, topRowH, menuW, ACS_BTEE);
    mvwaddch(borderWindow, topRowH, menuW + menuSuppW, ACS_BTEE);

    // draw "├" and "┤" connections
    mvwaddch(borderWindow, topRowH, 0, ACS_LTEE);
    mvwaddch(borderWindow, topRowH, mainBlockW - 1, ACS_RTEE);

    // draw "┬" connections
    mvwaddch(borderWindow, 0, menuW, ACS_TTEE);
    mvwaddch(borderWindow, 0, menuW + menuSuppW, ACS_TTEE);

    wrefresh(borderWindow);
}