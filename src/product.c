#include <stdio.h>
#include <stdlib.h>
#include "../include/product.h"
#define INITIAL_CAPACITY 10

void initProductList(ProductList *list) {
    list->date = (Product*) malloc(INITIAL_CAPACITY * sizeof(Product));
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

int addProduct(ProductList *list, Product product){
    // Veridfica se esta cheio.
    if (list->count >= list->capacity) {
        // dobra o tamanho.
        list->capacity *= 2;

        Product *newDate = (Product*) realloc(list->date, list->capacity * sizeof(Product));

        // if realoca a memoria falhar, retorna 0.
        if (newDate == NULL) {
            return 0;
        }

        list->date = (Product*) newDate;
        printf("Capacidade de produtos aumentada para %d\n", list->capacity);
    }

    // adiciona o novo produto ao final da lista.
    list->date[list->count] = product;
    // incrementa a contagem de produtos na lista.
    list->count++;
    return 1;
}