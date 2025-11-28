#include <ncursesw/curses.h>
#include <string.h>
#include <stdlib.h>
#include "../include/menu.h"
#include "../include/client.h"

void clearInfoInput(WINDOW *infoWin, const int startY) {
    int maxW = getmaxx(infoWin);
    for (int y = startY; y < maxW; y++) {
        mvwhline(infoWin, y, 0, ' ', maxW);
    }
    wrefresh(infoWin);
}

void printError(WINDOW *infoWin, const int y, const char *msg) {
    wattron(infoWin, COLOR_PAIR(1));
    mvwprintw(infoWin, y, 0, "%s", msg);
    wattroff(infoWin, COLOR_PAIR(1));
    wrefresh(infoWin);
    napms(1500);
}

void insertClientCommand(WINDOW *infoWin, ClientList *clientList) {
    Client newClient;
    char buffer[200];
    int row = 0;

    curs_set(1);
    echo();

    werase(infoWin);

    while (1) {
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row++, 0, "Digite o ID (Numerico) do cliente: ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, buffer, 10);

        // try to convert to int
        const int id = atoi(buffer);

        if (id <= 0) {
            printError(infoWin, row + 1, "ID inválido. Digite um numero positivo.");
            row--;
            continue;
        }

        newClient.id = id;
        break;
    }
}

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

    // write the system name
    mvwprintw(borderWindow, 0, 1, "🅼 🅰 🆁 🅲 🅷 ❷ ❺ ");

    wrefresh(borderWindow);
}

void showClientMenu(
    WINDOW *menuWin,
    WINDOW *menuSuppWin,
    WINDOW *infoWin,
    WINDOW *footerWin,
    WINDOW *borderWindow,
    const int borderColX,
    ClientList *clientList
) {
    char options[6][50] = {
        "inserir_cliente.exe",
        "listar_clientes.exe",
        "editar_cliente.exe",
        "remover_cliente.exe",
        "",
        "voltar.exe"
    };

    char optionsSupp[6][50] = {
        "--CMD",
        "--CMD",
        "--CMD",
        "--CMD",
        "",
        "--CMD",
    };

    char descriptions[6][100] = {
        "Cadastrar um novo cliente no sistema.",
        "Listar todos os clientes registrados.",
        "Editar um cliente especifico.",
        "Remover um cliente.",
        "",
        "Voltar para o menu principal."
    };

    int nOptions = 6;
    int highlight = 0;
    int key;

    keypad(menuWin, TRUE);
    flushinp();
    keypad(menuWin, TRUE);

    while (1) {
        // clean windows
        werase(menuWin);
        werase(menuSuppWin);
        werase(infoWin);
        werase(footerWin);

        for (int i = 0; i < nOptions; i++) {
            // if it's option 5, needs to be blank, so do nothing
            if (i == 4) continue;

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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->", everything bold and background red
                wattron(menuSuppWin, COLOR_PAIR(1));
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->"
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
                mvwprintw(menuSuppWin, i, getmaxx(menuSuppWin) - 1, ">");
                wattroff(menuSuppWin, A_BOLD);
            }
        }

        // this prints a line of '-' in the blank space, with gray color
        wattron(menuWin, A_DIM);
        mvwhline(menuWin, 4, 0, '-', getmaxx(menuWin));
        wattroff(menuWin, A_DIM);

        wattron(menuSuppWin, A_DIM);
        mvwhline(menuSuppWin, 4, 0, '-', getmaxx(menuSuppWin));
        wattroff(menuSuppWin, A_DIM);

        mvwprintw(footerWin, 0, 0, "PWD: /CLIENTES/ │ Modulo de CLIENTES. ENTER para confirmar.");

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
                if (highlight == 4) highlight--;
                if (highlight < 0) highlight = nOptions - 1;
                break;

            case KEY_DOWN:
                highlight++;
                if (highlight == 4) highlight++;
                if (highlight > nOptions - 1) highlight = 0;
                break;

            case 10:
                switch (highlight) {
                    // insert client
                    case 0:
                        insertClientCommand(infoWin, clientList);
                        break;
                    // list clients
                    case 1:
                        break;
                    // edit client
                    case 2:
                        break;
                    // remove
                    case 3:
                        break;
                    // go back
                    case 5:
                        return;
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

}

void showProductMenu(WINDOW *menuWin, WINDOW *menuSuppWin, WINDOW *infoWin, WINDOW *footerWin, WINDOW *borderWindow, int borderColX) {
    char options[6][50] = {
        "inserir_produto.exe",
        "listar_produtos.exe",
        "editar_produto.exe",
        "remover_produto.exe",
        "",
        "voltar.exe"
    };

    char optionsSupp[6][50] = {
        "--CMD",
        "--CMD",
        "--CMD",
        "--CMD",
        "",
        "--CMD",
    };

    char descriptions[6][100] = {
        "Cadastrar um novo produto no sistema.",
        "Listar todos os produtos registrados.",
        "Editar um produto especifico.",
        "Remover um produto.",
        "",
        "Voltar para o menu principal."
    };

    int nOptions = 6;
    int highlight = 0;
    int key;

    keypad(menuWin, TRUE);
    flushinp();
    keypad(menuWin, TRUE);

    while (1) {
        // clean windows
        werase(menuWin);
        werase(menuSuppWin);
        werase(infoWin);
        werase(footerWin);

        for (int i = 0; i < nOptions; i++) {
            // if it's option 5, needs to be blank, so do nothing
            if (i == 4) continue;

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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->", everything bold and background red
                wattron(menuSuppWin, COLOR_PAIR(1));
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->"
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
                mvwprintw(menuSuppWin, i, getmaxx(menuSuppWin) - 1, ">");
                wattroff(menuSuppWin, A_BOLD);
            }
        }

        // this prints a line of '-' in the blank space, with gray color
        wattron(menuWin, A_DIM);
        mvwhline(menuWin, 4, 0, '-', getmaxx(menuWin));
        wattroff(menuWin, A_DIM);

        wattron(menuSuppWin, A_DIM);
        mvwhline(menuSuppWin, 4, 0, '-', getmaxx(menuSuppWin));
        wattroff(menuSuppWin, A_DIM);

        mvwprintw(footerWin, 0, 0, "PWD: /PRODUTOS/ │ Modulo de PRODUTOS. ENTER para confirmar.");

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
                if (highlight == 4) highlight--;
                if (highlight < 0) highlight = nOptions - 1;
                break;

            case KEY_DOWN:
                highlight++;
                if (highlight == 4) highlight++;
                if (highlight > nOptions - 1) highlight = 0;
                break;

            case 10:
                switch (highlight) {
                    // insert product
                    case 0:
                        break;
                    // list products
                    case 1:
                        break;
                    // edit product
                    case 2:
                        break;
                    // remove product
                    case 3:
                        break;
                    // go back
                    case 5:
                        return;
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

}

void showOrderMenu(WINDOW *menuWin, WINDOW *menuSuppWin, WINDOW *infoWin, WINDOW *footerWin, WINDOW *borderWindow, int borderColX) {
    char options[5][50] = {
        "adicionar_pedido.exe",
        "mostrar_pedidos.exe",
        "consultar_pedido.exe",
        "",
        "voltar.exe"
    };

    char optionsSupp[5][50] = {
        "--CMD",
        "--CMD",
        "--CMD",
        "",
        "--CMD",
    };

    char descriptions[5][100] = {
        "Cadastrar um novo pedido no sistema.",
        "Listar todos os pedidos registrados.",
        "Consultar detalhes de um pedido especifico.",
        "",
        "Voltar para o menu principal."
    };

    int nOptions = 5;
    int highlight = 0;
    int key;

    keypad(menuWin, TRUE);
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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->", everything bold and background red
                wattron(menuSuppWin, COLOR_PAIR(1));
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->"
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
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

        mvwprintw(footerWin, 0, 0, "PWD: /PEDIDOS/ │ Modulo de PEDIDOS. ENTER para confirmar.");

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

            case 10:
                switch (highlight) {
                    // insert order
                    case 0:
                        break;
                    // list orders
                    case 1:
                        break;
                    // view order
                    case 2:
                        break;
                        // go back
                    case 4:
                        return;
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

}

void showMainMenu(
    WINDOW *menuWin,
    WINDOW *menuSuppWin,
    WINDOW *infoWin,
    WINDOW *footerWin,
    WINDOW *borderWindow,
    const int borderColX,
    const int mainBlockW,
    const int menuW,
    const int menuSuppW,
    const int topRowH,
    ClientList *clientList
) {
    char options[5][50] = {
        "CLIENTES/",
        "PRODUTOS/",
        "PEDIDOS/",
        "",
        "sair.exe"
    };

    char optionsSupp[5][50] = {
        "--FOLDER",
        "--FOLDER",
        "--FOLDER",
        "",
        "--CMD",
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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->", everything bold and background red
                wattron(menuSuppWin, COLOR_PAIR(1));
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
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

                // this prints the option supplement and fills the empty space with the rest to form the arrow "--->"
                wattron(menuSuppWin, A_BOLD);
                mvwprintw(menuSuppWin, i, 0, "%s", optionsSupp[i]);
                mvwhline(menuSuppWin, i, strlen(optionsSupp[i]), '-', getmaxx(menuSuppWin) - strlen(optionsSupp[i]));
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

        mvwprintw(footerWin, 0, 0, "PWD: / │ Use SETAS para navegar e ENTER para selecionar.");

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

            case 10:
                switch (highlight) {
                    // clients module
                    case 0:
                        showClientMenu(menuWin, menuSuppWin, infoWin, footerWin, borderWindow, borderColX, clientList);
                        drawBorderWindow(borderWindow, mainBlockW, menuW, menuSuppW, topRowH);
                        touchwin(borderWindow);
                        wrefresh(borderWindow);
                        break;
                    // products module
                    case 1:
                        showProductMenu(menuWin, menuSuppWin, infoWin, footerWin, borderWindow, borderColX);
                        drawBorderWindow(borderWindow, mainBlockW, menuW, menuSuppW, topRowH);
                        touchwin(borderWindow);
                        wrefresh(borderWindow);
                        break;
                    // orders module
                    case 2:
                        showOrderMenu(menuWin, menuSuppWin, infoWin, footerWin, borderWindow, borderColX);
                        drawBorderWindow(borderWindow, mainBlockW, menuW, menuSuppW, topRowH);
                        touchwin(borderWindow);
                        wrefresh(borderWindow);
                        break;
                    // exit
                    case 4:
                        return;
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}