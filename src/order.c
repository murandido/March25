#include "../include/order.h"

struct Order {
    int id;
    int clientId;
    char data[11];
    int total;
};