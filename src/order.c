#include <stdlib.h>
#include "../include/order.h"

#define INITIAL_CAPACITY 10

void initOrderList(OrderList *list) {
    list->data = (Order*) malloc(INITIAL_CAPACITY * sizeof(Order));
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}