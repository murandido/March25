#ifndef MARCH25_PRODUCT_H
#define MARCH25_PRODUCT_H

typedef struct {
    int id;
    char description[100];
    char name[100];
    int price;
} Product;

typedef struct {
    Product *data;
    int count;
    int capacity;
} ProductList;

void initProductList(ProductList *list);
int addProduct(ProductList *list, Product product);
int checkProductID(const ProductList *list, int id);
Product getProduct(const ProductList *list, int id);
int removeProduct(ProductList *list, int id);

#endif //MARCH25_PRODUCT_H