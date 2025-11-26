#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/client.h"

#define INITIAL_CAPACITY 10

void initClientList(ClientList *list) {
    list->data = (Client*) malloc(INITIAL_CAPACITY * sizeof(Client));
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

int validateCNPJ(const char *cnpjInput) {
    int numbers[14];
    int length = strlen(cnpjInput);
    int count = 0;

    // extract only digits from the input string
    for (int i = 0; i < length; i++) {
        if (isdigit(cnpjInput[i])) {
            if (count < 14) {
                // convert char to int
                numbers[count] = cnpjInput[i] - '0';
                count++;
            } else {
                // too many digits
                return 0;
            }
        }
    }

    // a valid CNPJ must have exactly 14 digits
    if (count != 14) {
        return 0;
    }

    // check for invalid patterns (all digits equal)
    int allEqual = 1;
    for (int i = 1; i < 14; i++) {
        if (numbers[i] != numbers[0]) {
            allEqual = 0;
            break;
        }
    }
    if (allEqual) {
        return 0;
    }

    // calculate first verifier digit
    // weights: 5,4,3,2,9,8,7,6,5,4,3,2
    int sum1 = 0;
    int weights1[] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};

    for (int i = 0; i < 12; i++) {
        sum1 += numbers[i] * weights1[i];
    }

    int remainder1 = sum1 % 11;
    int digit1 = (remainder1 < 2) ? 0 : (11 - remainder1);

    // verify first digit
    if (numbers[12] != digit1) {
        return 0;
    }

    // calculate second verifier digit
    // weights: 6,5,4,3,2,9,8,7,6,5,4,3,2
    int sum2 = 0;
    int weights2[] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};

    for (int i = 0; i < 13; i++) {
        sum2 += numbers[i] * weights2[i];
    }

    int remainder2 = sum2 % 11;
    int digit2 = (remainder2 < 2) ? 0 : (11 - remainder2);

    // verify second digit
    if (numbers[13] != digit2) {
        return 0;
    }

    // CNPJ is valid
    return 1;
}

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

    DVs1 = checkDigitVerifier(DVs1);

    // Calculation of Check Digit 2
    for (i = 0, j = 11; i < 10; i++, j--) {
        DVs2 += cpf2[i] * j;
    }

    DVs2 = checkDigitVerifier(DVs2);

    // Final verification;
    if (sameNumbers(cpf2)) {
        return 0; // Invalid CPF (all identical)
    }

    if(DVs1 == cpf2[9] && DVs2 == cpf2[10]){
        return 1;  // Valid CPF
    }

    return 0; // Invalid CPF
}
