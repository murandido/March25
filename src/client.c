#include "../include/client.h"

int sameNumbers(const int *cpf_nums) {
    for (int i = 1; i < 11; i++) {
        if (cpf_nums[i] != cpf_nums[0]){
            return 0; // They're different
        }
    }

    return 1; // The CPF has all its digits the same
}

int checkDigitVerifier(int DVs) {
    DVs %= 11;

    if(DVs < 2){
        return 0;
    }

    return 11 - DVs;
}

int validateCPF(const char *cpf) {
    int cpf2[11], i = 0, j = 0, DVs1 = 0, DVs2 = 0;

    for (i = 0; cpf[i] != '\0' && j < 11; i++) {
        if (cpf[i] >= '0' && cpf[i] <= '9') {
            cpf2[j] = cpf[i] - '0';
            j++;
        }
    }

    // If isn´t 11 numbers, return 0
    if (j != 11) {
        return 0;
    }

    // Calculation of Check Digit 1
    for (i = 0, j = 10; i < 9; i++, j--) {
        DVs1 += cpf2[i] * j;
    }

    DVs1 = verifier(DVs1);

    // Calculation of Check Digit 2
    for (i = 0, j = 11; i < 10; i++, j--) {
        DVs2 += cpf2[i] * j;
    }

    DVs2 = verifier(DVs2);

    // Final verification;
    if (sameNumbers(cpf2)) {
        return 0; // Invalid CPF (all identical)
    }

    if(DVs1 == cpf2[9] && DVs2 == cpf2[10]){
        return 1;  // Valid CPF
    }

    return 0; // Invalid CPF
}