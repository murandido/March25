#ifndef MARCH25_MENU_H
#define MARCH25_MENU_H
#include <ncursesw/curses.h>

#include "order.h"
#include "../include/client.h"
#include  "../include/product.h"

void clearInfoInput(WINDOW *infoWin, int startY);
void printError(WINDOW *infoWin, int y, const char *msg);
void insertClientCommand(WINDOW *infoWin, ClientList *clientList);
void listClientsCommand(WINDOW *infoWin, WINDOW *footerWin, const ClientList *clientList);
void editClientCommand(WINDOW *infoWin, const ClientList *clientList);
void removeClientCommand(WINDOW *infoWin, ClientList *clientList);
void insertProductCommand(WINDOW *infoWin, ProductList *productList);
void listProductsCommand(WINDOW *infoWin, WINDOW *footerWin, const ProductList *productList);
void editProductCommand(WINDOW *infoWin, ProductList *productList);
void removeProductCommand(WINDOW *infoWin, ProductList *productList);
void drawBorderWindow(WINDOW *borderWindow, int mainBlockW, int menuW, int menuSuppW, int topRowH);
void showClientMenu(
    WINDOW *menuWin,
    WINDOW *menuSuppWin,
    WINDOW *infoWin,
    WINDOW *footerWin,
    WINDOW *borderWindow,
    int borderColX,
    ClientList *clientList
);
void showOrderMenu(WINDOW *menuWin, WINDOW *menuSuppWin, WINDOW *infoWin, WINDOW *footerWin, WINDOW *borderWindow, int borderColX, OrderList *orderList, ClientList *clientList, ProductList *productList);
void showProductMenu(WINDOW *menuWin, WINDOW *menuSuppWin, WINDOW *infoWin, WINDOW *footerWin, WINDOW *borderWindow, int borderColX, ProductList *productList);
void showMainMenu(
    WINDOW *menuWin,
    WINDOW *menuSuppWin,
    WINDOW *infoWin,
    WINDOW *footerWin,
    WINDOW *borderWindow,
    int borderColX,
    int mainBlockW,
    int menuW,
    int menuSuppW,
    int topRowH,
    ClientList *clientList,
    ProductList *productList,
    OrderList *orderList
);

#endif //MARCH25_MENU_H