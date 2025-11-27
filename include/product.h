#ifndef MARCH25_PRODUCT_H
#define MARCH25_PRODUCT_H

typedef struct {
    int id;
    char description[100];
    char name[100];
    int price;
    int stock;
} Product;

typedef struct {
    Product *data;
    int count;
    int capacity;
} ProductList;

#endif //MARCH25_PRODUCT_H