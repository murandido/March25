#include "../include/client.h"

int samenumbers(int *cpf_nums){

    for (int i = 1; i < 11; i++) {

        if (cpf_nums[i] != cpf_nums[0]){

            return 0; // São diferentes
        }
    }
    return 1; // O CPF tem todos os seus numeros iguais
}

int verifier(int DVs){

    DVs %= 11;

    if(DVs < 2){

        return 0;
    }

    else{

        return 11 - DVs;
    }
}

int CPF(char *cpf){

    int cpf2[11], i = 0, j = 0, DVs1 = 0, DVs2 = 0;

    for(i = 0; cpf[i] != '\0' && j < 11; i++){

        if(cpf[i] >= '0' && cpf[i] <= '9'){

            cpf2[j] = cpf[i] - '0';
            j++;
        }
    }

    // Se não leu 11 números;

    if (j != 11) {

        return 0;
    }

    // Calculo dos Digito Verificador 1

    for(i = 0, j = 10; i < 9; i++, j--){

        DVs1 += cpf2[i] * j;
    }

    DVs1 = verifier(DVs1);

    // Calculo dos Digito Verificador 2

    for(i = 0, j = 11; i < 10; i++, j--){

        DVs2 += cpf2[i] * j;
    }

    DVs2 = verifier(DVs2);

    // Verificacao final;

    if(samenumbers(cpf2)){

        return 0; //cpf invalido (todos iguais)
    }

    if(DVs1 == cpf2[9] && DVs2 == cpf2[10]){

        return 1;  //CPF valido
    }

    else{

        return 0;   // CPF invalido
    }
}