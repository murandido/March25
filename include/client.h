//
// Created by murandido on 11/4/25.
//

#ifndef MARCH25_CLIENT_H
#define MARCH25_CLIENT_H

typedef struct {
    int PessoaJuridicaId;
    int CNPJ[14];
    int CNPJclone[14];
    char RazaoSocial[100];
} PessoaJuridicaId;

//prototipo da função de validar CPNJ.
int validarCNPJ();

#endif //MARCH25_CLIENT_H