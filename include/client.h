#ifndef MARCH25_CLIENT_H
#define MARCH25_CLIENT_H

typedef struct {
    int id;
    int type;
    char name[100];
    char legalName[100];
    char address[200];
    char phoneNumber[20];
    char cpf[15];
    char cnpj[15];
    char email[100];
    char contactName[100];
} Client;

typedef struct {
    Client *data;
    int count;
    int capacity;
} ClientList;

int validateCNPJ(const char *cnpjInput);
int sameNumbers(const int *cpf_nums);
int checkDigitVerifier(int DVs);
int validateCPF(const char *cpf);
void initClientList(ClientList *list);
int addClient(ClientList *list, const Client client);
int loadClientsFromCSV(ClientList *list, const char *fileName);
int saveClientsToCSV(const ClientList *list, const char *fileName);
void freeClientList(ClientList *list);



#endif //MARCH25_CLIENT_H