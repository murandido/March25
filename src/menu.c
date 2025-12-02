#include <ncursesw/curses.h>
#include <string.h>
#include <stdlib.h>
#include "../include/menu.h"
#include "../include/client.h"
#include "../include/product.h"

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

    // id
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

        if (checkClientID(clientList, id)) {
            printError(infoWin, row + 1, "Este ID ja esta cadastrado.");
            row--;
            continue;
        }

        newClient.id = id;
        break;
    }

    // type
    row += 2;
    while (1) {
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row++, 0, "Tipo (0=Fisica, 1=Juridica): ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, buffer, 5);

        if (strcmp(buffer, "0") != 0 && strcmp(buffer, "1") != 0) {
            printError(infoWin, row + 1, "Digite apenas 0 ou 1.");
            row--;
            continue;
        }

        newClient.type = atoi(buffer);
        break;
    }

    // document
    row += 2;
    while (1) {
        clearInfoInput(infoWin, row);
        if (newClient.type == 0) {
            mvwprintw(infoWin, row++, 0, "Digite CPF (apenas numeros): ");
        } else {
            mvwprintw(infoWin, row++, 0, "Digite CNPJ (apenas numeros): ");
        }
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, buffer, 16);

        if (newClient.type == 0) {
            if (!validateCPF(buffer)) {
                printError(infoWin, row + 1, "CPF invalido.");
                row--;
                continue;
            }

            if (checkClientCPF(clientList, buffer)) {
                printError(infoWin, row + 1, "CPF ja cadastrado no sistema.");
                row--;
                continue;
            }

            strcpy(newClient.cpf, buffer);
            strcpy(newClient.cnpj, " ");
        } else {
            if (!validateCNPJ(buffer)) {
                printError(infoWin, row + 1, "CNPJ invalido.");
                row--;
                continue;
            }

            if (checkClientCNPJ(clientList, buffer)) {
                printError(infoWin, row + 1, "CNPJ ja cadastrado no sistema.");
                row--;
                continue;
            }

            strcpy(newClient.cnpj, buffer);
            strcpy(newClient.cpf, " ");
        }

        break;
    }

    // address
    row += 2;
    mvwprintw(infoWin, row++, 0, "Endereco Completo: ");
    wmove(infoWin, row, 0);
    wrefresh(infoWin);
    wgetnstr(infoWin, newClient.address, 199);

    // email
    row += 2;
    mvwprintw(infoWin, row++, 0, "Email: ");
    wmove(infoWin, row, 0);
    wrefresh(infoWin);
    wgetnstr(infoWin, newClient.email, 99);

    if (newClient.type == 0) { // individual client
        row += 2;
        mvwprintw(infoWin, row++, 0, "Nome Completo: ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, newClient.name, 99);

        row += 2;
        mvwprintw(infoWin, row++, 0, "Celular: ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, newClient.phoneNumber, 19);

        // clear individual client fields
        strcpy(newClient.legalName, " ");
        strcpy(newClient.contactName, " ");
    } else { // legal client
        row += 2;
        mvwprintw(infoWin, row++, 0, "Razao Social: ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, newClient.legalName, 99);

        row += 2;
        mvwprintw(infoWin, row++, 0, "Nome de Contato: ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, newClient.contactName, 99);

        // clear individual client fields
        strcpy(newClient.name, " ");
        strcpy(newClient.phoneNumber, " ");
    }

    // saving
    if (addClient(clientList, newClient)) {
        werase(infoWin);

        wattron(infoWin, A_BOLD);
        mvwprintw(infoWin, 5, 2, "CLIENTE ADICIONADO COM SUCESSO!");
        mvwprintw(infoWin, 7, 2, "Pressione qualquer tecla...");
        wattroff(infoWin, A_BOLD);
        wrefresh(infoWin);
    } else {
        printError(infoWin, 5, "Erro ao salvar cliente (Memoria cheia?).");
    }

    noecho();
    curs_set(0);
    wgetch(infoWin);
}

void listClientsCommand(WINDOW *infoWin, WINDOW *footerWin, const ClientList *clientList) {
    int key;
    int page = 0;
    const int ITEMS_PER_PAGE = 10;
    const int totalClients = clientList->count;

    // if there's no client, quit
    if (totalClients == 0) {
        werase(infoWin);
        printError(infoWin, 5, "Nenhum cliente cadastrado.");
        return;
    }

    keypad(infoWin, TRUE);
    curs_set(0);
    noecho();

    while (1) {
        werase(infoWin);

        // pagination calculation
        const int totalPages = (totalClients + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
        const int startIndex = page * ITEMS_PER_PAGE;
        int endIndex = startIndex + ITEMS_PER_PAGE;
        if (endIndex > totalClients) endIndex = totalClients;

        // table header
        wattron(infoWin, A_BOLD);
        mvwprintw(infoWin, 1, 1, "%-2s | %-20s | %-4s", "ID", "NOME/RAZAO", "TIPO");
        mvwhline(infoWin, 2, 1, ACS_HLINE, getmaxx(infoWin) - 2);
        wattroff(infoWin, A_BOLD);

        int row = 3;
        for (int i = startIndex; i < endIndex; i++) {
            const Client c = clientList->data[i];

            char displayName[30];
            char type[5];

            if (c.type == 0) {
                strcpy(type, "PF");
                strncpy(displayName, c.name, 19);
            } else {
                strcpy(type, "PJ");
                strncpy(displayName, c.legalName, 19);
            }
            // guarantees a null terminator if truncation has occurred
            displayName[24] = '\0';

            mvwprintw(infoWin, row, 1, "%-2d | %-20s | %-4s",
                      c.id, displayName, type);
            row++;
        }

        // updates footer
        werase(footerWin);
        mvwprintw(footerWin, 0, 0, "Pagina %d/%d │ < > Navegar │ 'q' Sair", page + 1, totalPages);

        wrefresh(infoWin);
        wrefresh(footerWin);

        key = wgetch(infoWin);

        switch (key) {
            case KEY_RIGHT:
                if (page < totalPages - 1) page++;
                break;
            case KEY_LEFT:
                if (page > 0) page--;
                break;
            case 'q':
            case 'Q':
                // restores the default footer before exiting
                werase(footerWin);
                mvwprintw(footerWin, 0, 0, "PWD: /CLIENTES/ │ Modulo de CLIENTES. ENTER para confirmar.");
                wrefresh(footerWin);
                return;
            default:
                break;
        }
    }
}

void editClientCommand(WINDOW *infoWin, const ClientList *clientList) {
    char buffer[200];
    int row = 0;
    int clientIndex = -1;

    curs_set(1);
    echo();

    // search for the id
    while (1) {
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row, 0, "Digite o ID do cliente a editar: ");
        row++;
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, buffer, 10);
        int id = atoi(buffer);

        if (id <= 0) {
            noecho();
            curs_set(0);
            return;
        }

        // search for the client index
        for (int i = 0; i < clientList->count; i++) {
            if (clientList->data[i].id == id) {
                clientIndex = i;
                break;
            }
        }

        if (clientIndex == -1) {
            printError(infoWin, row + 1, "Cliente nao encontrado.");
            row--;
            continue;
        }
        break;
    }

    // pointer to the client that needs to be edited
    Client *c = &clientList->data[clientIndex];

    row += 2;
    clearInfoInput(infoWin, row);

    // show who is
    char displayName[50];
    if (c->type == 0) strcpy(displayName, c->name);
    else strcpy(displayName, c->legalName);

    mvwprintw(infoWin, row++, 0, "Cliente: %s", displayName);
    mvwprintw(infoWin, row++, 0, "Deseja editar? (S/N): ");
    wrefresh(infoWin);

    wgetnstr(infoWin, buffer, 5);
    if (buffer[0] != 's' && buffer[0] != 'S') {
        noecho();
        curs_set(0);
        werase(infoWin);
        mvwprintw(infoWin, 5, 2, "Edicao cancelada.");
        wrefresh(infoWin);
        napms(1000);
        return;
    }

    // edit form
    row++;
    mvwprintw(infoWin, row++, 0, "Deixe em branco para manter o valor atual.");
    row++;

    // address
    clearInfoInput(infoWin, row);
    mvwprintw(infoWin, row, 0, "Endereco [%s]: ", c->address);
    row++;
    wmove(infoWin, row, 0);
    wrefresh(infoWin);
    wgetnstr(infoWin, buffer, 199);
    if (strlen(buffer) > 0) {
        strcpy(c->address, buffer);
    }

    row += 2;
    // email
    clearInfoInput(infoWin, row);
    mvwprintw(infoWin, row, 0, "Email [%s]: ", c->email);
    row++;
    wmove(infoWin, row, 0);
    wrefresh(infoWin);
    wgetnstr(infoWin, buffer, 99);
    if (strlen(buffer) > 0) {
        strcpy(c->email, buffer);
    }

    // specific fields
    if (c->type == 0) {
        row += 2;
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row, 0, "Nome [%s]: ", c->name);
        row++;
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, buffer, 99);
        if (strlen(buffer) > 0) strcpy(c->name, buffer);

        row += 2;
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row, 0, "Celular [%s]: ", c->phoneNumber);
        row++;
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, buffer, 19);
        if (strlen(buffer) > 0) strcpy(c->phoneNumber, buffer);

    } else {
        row += 2;
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row, 0, "Razao Social [%s]: ", c->legalName);
        row++;
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, buffer, 99);
        if (strlen(buffer) > 0) {
            strcpy(c->legalName, buffer);
        }

        row += 2;
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row, 0, "Contato [%s]: ", c->contactName);
        row++;
        wmove(infoWin, row, 0);
        wrefresh(infoWin);
        wgetnstr(infoWin, buffer, 99);
        if (strlen(buffer) > 0) strcpy(c->contactName, buffer);
    }

    werase(infoWin);
    wattron(infoWin, A_BOLD);
    mvwprintw(infoWin, 5, 2, "CLIENTE ATUALIZADO COM SUCESSO!");
    mvwprintw(infoWin, 7, 2, "Pressione ENTER para voltar...");
    wattroff(infoWin, A_BOLD);
    wrefresh(infoWin);

    noecho();
    curs_set(0);
    wgetch(infoWin);
}

void removeClientCommand(WINDOW *infoWin, ClientList *clientList) {
    char buffer[200];
    int row = 0;
    int clientIndex = -1;

    curs_set(1);
    echo();

    werase(infoWin);

    // search for id
    while (1) {
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row, 0, "Digite o ID do cliente a remover: ");
        row++;
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, buffer, 10);
        int id = atoi(buffer);

        if (id <= 0) {
            noecho();
            curs_set(0);
            return;
        }

        // search for the index
        for (int i = 0; i < clientList->count; i++) {
            if (clientList->data[i].id == id) {
                clientIndex = i;
                break;
            }
        }

        if (clientIndex == -1) {
            printError(infoWin, row + 2, "Cliente nao encontrado.");
            row--;
            continue;
        }
        break;
    }

    // get the data to confirm
    Client c = clientList->data[clientIndex];
    char displayName[50];
    if (c.type == 0) strcpy(displayName, c.name);
    else strcpy(displayName, c.legalName);

    row += 2;
    clearInfoInput(infoWin, row);
    mvwprintw(infoWin, row++, 0, "Cliente: %s", displayName);
    mvwprintw(infoWin, row++, 0, "Documento: %s", (c.type == 0) ? c.cpf : c.cnpj);

    row++;
    wattron(infoWin, COLOR_PAIR(1));
    mvwprintw(infoWin, row++, 0, "TEM CERTEZA? Essa acao e irreversivel! (S/N): ");
    wattroff(infoWin, COLOR_PAIR(1));

    // row++;
    wmove(infoWin, row, 0);
    wrefresh(infoWin);

    wgetnstr(infoWin, buffer, 5);

    if (buffer[0] != 's' && buffer[0] != 'S') {
        noecho();
        curs_set(0);
        werase(infoWin);
        mvwprintw(infoWin, 5, 2, "Remocao cancelada.");
        wrefresh(infoWin);
        napms(1000);
        return;
    }

    if (removeClient(clientList, c.id)) {
        werase(infoWin);
        wattron(infoWin, A_BOLD);
        mvwprintw(infoWin, 5, 2, "CLIENTE REMOVIDO COM SUCESSO!");
        mvwprintw(infoWin, 7, 2, "Pressione qualquer tecla...");
        wattroff(infoWin, A_BOLD);
        wrefresh(infoWin);
    } else {
        printError(infoWin, 5, "Erro desconhecido ao remover");
    }

    noecho();
    curs_set(0);
    wgetch(infoWin);
}

void insertProductCommand(WINDOW *infoWin, ProductList *productList) {
    Product newProduct;
    char buffer[200];
    int row = 0;

    curs_set(1);
    echo();

    werase(infoWin);

    // id
    while (1) {
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row++, 0, "Digite o ID (Numerico) do produto: ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, buffer, 10);

        // try to convert to int
        const int id = atoi(buffer);

        if (id <= 0) {
            printError(infoWin, row + 1, "ID invalido. Digite um numero positivo.");
            row--;
            continue;
        }

        if (checkProductID(productList, id)) {
            printError(infoWin, row + 1, "Este ID ja esta cadastrado.");
            row--;
            continue;
        }

        newProduct.id = id;
        break;
    }

    // name
    row += 2;
    while (1) {
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row++, 0, "Nome do Produto: ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, newProduct.name, 99);

        if (strlen(newProduct.name) == 0) {
            printError(infoWin, row + 1, "O nome nao pode ser vazio.");
            row--;
            continue;
        }
        break;
    }

    // description
    row += 2;
    mvwprintw(infoWin, row++, 0, "Descricao: ");
    wmove(infoWin, row, 0);
    wrefresh(infoWin);
    wgetnstr(infoWin, newProduct.description, 99);

    // price
    row += 2;
    while (1) {
        clearInfoInput(infoWin, row);
        mvwprintw(infoWin, row++, 0, "Preco (apenas numeros): ");
        wmove(infoWin, row, 0);
        wrefresh(infoWin);

        wgetnstr(infoWin, buffer, 10);

        // try to convert to int
        const float priceFloat = atof(buffer);

        if (priceFloat <= 0) {
            printError(infoWin, row + 1, "Preco invalido. Digite um valor maior que 0.");
            row--;
            continue;
        }

        newProduct.price = (int) (priceFloat * 100);
        break;
    }

    if (addProduct(productList, newProduct)) {
        werase(infoWin);

        wattron(infoWin, A_BOLD);
        mvwprintw(infoWin, 5, 2, "PRODUTO ADICIONADO COM SUCESSO!");
        mvwprintw(infoWin, 7, 2, "Pressione qualquer tecla...");
        wattroff(infoWin, A_BOLD);
        wrefresh(infoWin);
    } else {
        printError(infoWin, 5, "Erro ao salvar produto (Memoria cheia?).");
    }

    noecho();
    curs_set(0);
    wgetch(infoWin);
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
                        listClientsCommand(infoWin, footerWin, clientList);
                        touchwin(infoWin);
                        touchwin(footerWin);
                        break;
                    // edit client
                    case 2:
                        editClientCommand(infoWin, clientList);
                        break;
                    // remove
                    case 3:
                        removeClientCommand(infoWin, clientList);
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

void showProductMenu(
    WINDOW *menuWin,
    WINDOW *menuSuppWin,
    WINDOW *infoWin,
    WINDOW *footerWin,
    WINDOW *borderWindow,
    int borderColX,
    ProductList *productList
) {
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
                        insertProductCommand(infoWin, productList);
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
    ClientList *clientList,
    ProductList *productList
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
                        showProductMenu(menuWin, menuSuppWin, infoWin, footerWin, borderWindow, borderColX, productList);
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