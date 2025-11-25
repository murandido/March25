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