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

int validateCNPJ(const char *cnpjInput);
int sameNumbers(const int *cpf_nums);
int checkDigitVerifier(int DVs);
int validateCPF(const char *cpf);

#endif //MARCH25_CLIENT_H