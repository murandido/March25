#ifndef MARCH25_PRODUCT_H
#define MARCH25_PRODUCT_H

typedef struct {
    int id;
    char description[100];
    char name[100];
    int price;
} Product;

typedef struct {
    Product *date;
    int count;
    int capacity;
} ProductList;

void initProductList(ProductList *list);
int addProduct(ProductList *list, Product product);
int productIdAnalysis(const ProductList *list, int id);
Product Consultproduct(ProductList *list, int id);
int DeleteProduct(ProductList *list, int id);
void listAllProducts(const ProductList *list);
int persistProducts(const ProductList *list);
#endif //MARCH25_PRODUCT_H