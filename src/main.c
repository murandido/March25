#include <ncursesw/curses.h>
#include <locale.h>
#include "../include/menu.h"
#include  "../include/order.h"
#include "../include/client.h"
#include  "../include/product.h"
#include "../include/utils.h"

int main() {
    setlocale (LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    // TODO: remove after finish the project
    napms(200);
    refresh();

    if (has_colors() == FALSE) {
        endwin();
        printf("Seu terminal nao suporta cores!");
        return 1;
    }

    // init dynamic arrays

    // check the data dir
    if (!checkDataDir()) {
        return 1;
    }

    // client array
    ClientList clientList;
    initClientList(&clientList);
    loadClientsFromCSV(&clientList, "data/clients.csv");

    // product array
    ProductList productList;
    initProductList(&productList);
    loadProductsFromCSV(&productList, "data/products.csv");

    // order array
    OrderList orderList;
    initOrderList(&orderList);
    loadOrdersFromCSV(&orderList, "data/orders.csv");

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);

    int screenH, screenW;
    getmaxyx(stdscr, screenH, screenW);

    // main block size
    int mainBlockH = (int) (screenH * 0.9);
    int mainBlockW = (int) (screenW * 0.6);

    // start coordinates
    int startY = (screenH - mainBlockH) / 2;
    int startX = (screenW - mainBlockW) / 2;

    // menu window size
    int menuWinH = mainBlockH - 3;
    int menuWinW = (int) (mainBlockW * 0.30);

    // menu supplement window size
    int menuSuppWinH = mainBlockH - 3;
    int menuSuppWinW = (int) (mainBlockW * 0.15);

    // info window size
    int infoWinH = mainBlockH - 3;
    int infoWinW = mainBlockW - (menuWinW + menuSuppWinW);

    // footer window size
    int footerWinH = 3;
    int footerWinW = mainBlockW;

    // extra info
    int topRowH = mainBlockH - footerWinH;

    // create windows
    WINDOW *menuWin = newwin(
        menuWinH - 1,
        menuWinW - 1,
        startY + 1,
        startX + 1
    );

    WINDOW *menuSuppWin = newwin(
        menuSuppWinH - 1,
        menuSuppWinW - 1,
        startY + 1,
        startX + menuWinW + 1
    );

    WINDOW *infoWin = newwin(
        infoWinH - 1,
        infoWinW - 2,
        startY + 1,
        startX + menuWinW + menuSuppWinW + 1
    );

    WINDOW *footerWin = newwin(
        footerWinH - 2,
        footerWinW - 2,
        startY + topRowH + 1,
        startX + 1
    );

    WINDOW *borderWindow = newwin(mainBlockH, mainBlockW, startY, startX);

    drawBorderWindow(borderWindow, mainBlockW, menuWinW, menuSuppWinW, topRowH);

    showMainMenu(menuWin, menuSuppWin, infoWin, footerWin, borderWindow, menuWinW, mainBlockW, menuWinW, menuSuppWinW, topRowH, &clientList);

    keypad(borderWindow, TRUE);

    // delete windows
    delwin(borderWindow);
    delwin(menuWin);
    delwin(menuSuppWin);
    delwin(infoWin);
    delwin(footerWin);

    // save dynamic arrays
    saveClientsToCSV((&clientList), "data/clients.csv");
    freeClientList(&clientList);
    saveProductsToCSV((&productList), "data/products.csv");
    freeProductList(&productList);
    saveOrdersToCSV(&orderList, "data/orders.csv");
    freeOrderList(&orderList);

    endwin();

    return 0;
}