#ifndef MARCH25_MENU_H
#define MARCH25_MENU_H
#include <ncursesw/curses.h>

void drawBorderWindow(WINDOW *borderWindow, int mainBlockW, int menuW, int menuSuppW, int topRowH);

void showOrderMenu(WINDOW *menuWin, WINDOW *menuSuppWin, WINDOW *infoWin, WINDOW *footerWin, WINDOW *borderWindow, int borderColX);

void showMainMenu(WINDOW *menuWin, WINDOW *menuSuppWin, WINDOW *infoWin, WINDOW *footerWin, WINDOW *borderWindow, int borderColX);

#endif //MARCH25_MENU_H