#include <stdio.h>
#include <stdlib.h>
#include "../include/product.h"
#define INITIAL_CAPACITY 10

void initProductList(ProductList *list) {
    list->data = (Product*) malloc(INITIAL_CAPACITY * sizeof(Product));
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

int productIdAnalysis(const ProductList *list, int id) {
    // Percorre todos os produtos.
    for (int i = 0; i < list->count; i++) {
        // Verifica se o ID do produto atual corresponde ao ID procurado
        if (list->data[i].id == id) {
            return i; // Retorna onde o produto foi encontrado
        }
    }
    return -1;
}

int addProduct(ProductList *list, const Product product){
    if (productIdAnalysis(list, product.id) != -1) { 
        printf("ERRO: O produto com ID %d já está cadastrado. Adição cancelada.\n", product.id);
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

int DeleteProduct(ProductList *list, int id) {
    int i;
    // 1. Localização: Encontrar o produto a ser removido
    for (i = 0; i < list->count && id != list->data[i].id; i++);

    // 2. Verificação de Falha.
    // Se i == list->count, o loop terminou sem encontrar o ID.
    if (list->count == i) {
        printf("ERRO: Produto com ID %d não encontrado.\n", id);
        return 0; // 0 significa falha
    }

    // 3. Deslocamento (Shift): Preencher o espaço vazio
    // O loop começa no índice 'i' onde o produto a ser removido está.
    for (; i < list->count - 1; i++) {
        list->data[i] = list->data[i + 1]; // Move o próximo elemento para a posição atual
    }
    
    // 4. Decrementa a contagem
    list->count--;

    printf("Produto com ID %d removido com sucesso. Itens restantes: %d.\n", id, list->count);
    return 1;
}
// 1. Localizar a caixa
// 2. Verifica se o produto foi encontrado
// Retorna a CÓPIA (por valor) do produto na variavel encontrada.
Product Consultproduct(ProductList *list, int id) {
    
    int box = productIdAnalysis(list, id);

    if (box != -1) {
        
        return list->data[box];
    }
}
void listAllProducts(const ProductList *list) {
    printf("Lista de Produtos:\n");
    printf("ID\tNome\tDescrição\tPreço\n");

    for (int i = 0; i < list->count; i++) {
        Product p = list->data[i];
        printf("%d\t%s\t%s\t%d\n", p.id, p.name, p.description, p.price);
    }
}