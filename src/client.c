#include <string.h>
#include <ctype.h>
#include "../include/client.h"

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
