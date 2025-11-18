#ifndef MARCH25_CLIENT_H
#define MARCH25_CLIENT_H

typedef struct {
    int id;
    int type;
    char name[100];
    char legalName[100];
    char address[200];
    char phoneNumber[20];
    char cpf[12];
    char cnpj[15];
    char email[100];
    char contactName[100];
} Client;

#endif //MARCH25_CLIENT_H