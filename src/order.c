#include <stdlib.h>
#include <stdio.h>
#include "../include/order.h"

#define INITIAL_CAPACITY 10

void initOrderList(OrderList *list) {
    list->data = (Order*) malloc(INITIAL_CAPACITY * sizeof(Order));
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

int addOrder(OrderList *list, const Order order) {
    // if the array is full, doubles the size
    if (list->count >= list->capacity) {
        list->capacity *= 2;

        void *temp = realloc(list->data, list->capacity * sizeof(Order));

        // if the realloc fails, just return the function
        if (temp == NULL) {
            return 0;
        }

        list->data = (Order*) temp;
    }

    // add the order to the end of the list
    list->data[list->count] = order;
    list->count++;
    return 1;
}

int removeOrder(OrderList *list, const int id) {
    int i;
    // search the index of the matching id
    for (i = 0; i < list->count && list->data[i].id != id; i++) {};

    // if didn't find, return 0
    if (i == list->count) {
        return 0;
    }

    // move all elements from right to left
    for (; i < list->count - 1; i++) {
        list->data[i] = list->data[i + 1];
    }

    list->count--;
    return 1;
}

int loadOrdersToCSV(OrderList *list, const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        return 0;
    }

    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        Order temp;

        if (sscanf(line, "%d,%d,%[^,],%d", &temp.id, &temp.clientId, temp.date, &temp.total) == 4) {
            addOrder(list, temp);
        }
    }

    fclose(file);
    return 1;
}

int saveOrdersToCSV(const OrderList *list, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < list->count; i++) {
        fprintf(
            file,
            "%d,%d,%s,%d\n",
            list->data[i].id,
            list->data[i].clientId,
            list->data[i].date,
            list->data[i].total
        );
    }

    fclose(file);
    return 1;
}