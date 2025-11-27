#include <stdio.h>
#include <stdlib.h>
#include "../include/product.h"
#define INITIAL_CAPACITY 10

void initProductList(ProductList *list) {
    list->data = (Product*) malloc(INITIAL_CAPACITY * sizeof(Product));
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}