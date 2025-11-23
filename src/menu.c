#include <ncursesw/curses.h>
#include <string.h>
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

void showMainMenu(WINDOW *menuWin, WINDOW *menuSuppWin, WINDOW *infoWin, WINDOW *footerWin, WINDOW *borderWindow, int borderColX) {
    char options[5][50] = {
        "clientes.exe",
        "produtos.exe",
        "pedidos.exe",
        "",
        "sair.exe"
    };

    char descriptions[5][100] = {
        "Gerenciar cadastro de clientes.",
        "Criar e listar produtos.",
        "Criar e listar pedidos de venda.",
        "",
        "Encerrar o sistema."
    };

    int nOptions = 5;
    int highlight = 0;
    int key;

    flushinp();
    keypad(menuWin, TRUE);

    while (1) {
        // clean windows
        werase(menuWin);
        werase(menuSuppWin);
        werase(infoWin);
        werase(footerWin);

        for (int i = 0; i < nOptions; i++) {
            // if it's option 4, needs to be blank, so do nothing
            if (i == 3) continue;

            if (i == highlight) {
                // this prints the highlighted option and fill the empty space with ' ' (spaces), to everything be red
                wattron(menuWin, COLOR_PAIR(1));
                mvwprintw(menuWin, i, 0, "%s", options[i]);
                mvwhline(menuWin, i, strlen(options[i]), ' ', getmaxx(menuWin) - strlen(options[i]));
                wattroff(menuWin, COLOR_PAIR(1));

                // this turns the '│' of the line red
                wattron(borderWindow, COLOR_PAIR(1));
                mvwaddch(borderWindow, i + 1, borderColX, ACS_VLINE);
                wattroff(borderWindow, COLOR_PAIR(1));

                // this prints "--FILE" and fills the empty space with the rest to form the arrow "--->", everything bold and background red
                wattron(menuSuppWin, COLOR_PAIR(1));
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", "--FILE");
                mvwhline(menuSuppWin, i, 6, '-', getmaxx(menuSuppWin) - 6);
                mvwprintw(menuSuppWin, i, getmaxx(menuSuppWin) - 1, ">");
                wattroff(menuSuppWin, A_BOLD);
                wattroff(menuSuppWin, COLOR_PAIR(1));

                // this prints the description of the highlighted option
                mvwprintw(infoWin, 0, 0, "%s", descriptions[i]);
            } else {
                // this print the not highlighted option
                mvwprintw(menuWin, i, 0, "%s", options[i]);

                // this turns the '│' of the line back to normal
                wattroff(borderWindow, COLOR_PAIR(1));
                mvwaddch(borderWindow, i + 1, borderColX, ACS_VLINE);

                // this prints "--FILE" and fills the empty space with the rest to form the arrow "--->"
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", "--FILE");
                mvwhline(menuSuppWin, i, 6, '-', getmaxx(menuSuppWin) - 6);
                mvwprintw(menuSuppWin, i, getmaxx(menuSuppWin) - 1, ">");
                wattroff(menuSuppWin, A_BOLD);
            }
        }

        // this prints a line of '-' in the blank space, with gray color
        wattron(menuWin, A_DIM);
        mvwhline(menuWin, 3, 0, '-', getmaxx(menuWin));
        wattroff(menuWin, A_DIM);

        wattron(menuSuppWin, A_DIM);
        mvwhline(menuSuppWin, 3, 0, '-', getmaxx(menuSuppWin));
        wattroff(menuSuppWin, A_DIM);

        mvwprintw(footerWin, 0, 0, "Use SETAS para navegar e ENTER para selecionar.");

        // refresh the windows
        wrefresh(borderWindow);
        wrefresh(menuWin);
        wrefresh(menuSuppWin);
        wrefresh(infoWin);
        wrefresh(footerWin);

        // get the pressed key
        key = wgetch(menuWin);

        // keys logic
        switch (key) {
            case KEY_UP:
                highlight--;

                if (highlight == 3) highlight--;

                if (highlight < 0) highlight = nOptions - 1;
                break;

            case KEY_DOWN:
                highlight++;

                if (highlight == 3) highlight++;

                if (highlight > nOptions - 1) highlight = 0;
                break;

            default:
                break;
        }
    }
}