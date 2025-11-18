#include <ncursesw/curses.h>

#ifndef MARCH25_MENU_H
#define MARCH25_MENU_H

void drawBorderWindow(WINDOW *borderWindow, int mainBlockW, int menuW, int menuSuppW, int topRowH);

#endif //MARCH25_MENU_H