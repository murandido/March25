#include <stdio.h>
#include <stdlib.h>
#include "../include/product.h"
#define INITIAL_CAPACITY 10

void initProductList(ProductList *list) {
    list->data = (Product*) malloc(INITIAL_CAPACITY * sizeof(Product));
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

int checkProductID(const ProductList *list, const int id) {
    // go through all the products
    for (int i = 0; i < list->count; i++) {
        // checks if the current product id matches the searched id
        if (list->data[i].id == id) {
            return 1; // found the product id
        }
    }

    return 0;
}

int addProduct(ProductList *list, const Product product){
    if (checkProductID(list, product.id)) {
        return 0;
    }

    // verify if it's full
    if (list->count >= list->capacity) {
        // doubles the size
        list->capacity *= 2;

        void *temp = realloc(list->data, list->capacity * sizeof(Product));

        // if the realloc fails, just return the function
        if (temp == NULL) {
            return 0;
        }

        list->data = (Product*) temp;
    }

    // add the product to the end of the list
    list->data[list->count] = product;
    list->count++;
    return 1;
}

int removeProduct(ProductList *list, const int id) {
    int i;
    // find the product to be removed
    for (i = 0; i < list->count && id != list->data[i].id; i++);

    // if i == list->count, the loop finished without finding the id
    if (list->count == i) {
        return 0;
    }

    // move all elements from right to left
    for (; i < list->count - 1; i++) {
        list->data[i] = list->data[i + 1]; // move the next element to the current position
    }

    list->count--;

    return 1;
}

Product getProduct(const ProductList *list, const int id) {
    int i;
    // find the product
    for (i = 0; i < list->count && id != list->data[i].id; i++) {};

    Product product = {-1, "", "", -1};

    // if i == list->count, the loop finished without finding the id
    if (list->count == i) {
        return product;
    }

    product = list->data[i];

    return product;
}