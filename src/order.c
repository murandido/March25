#include "../include/order.h"

struct Order {
    int id;
    int clientId;
    char data[11];
    int total;
};

struct ItemOrder {
    int orderId;
    int productId;
    int quantity;
    int subtotal;
};