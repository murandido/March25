#ifndef MARCH25_ORDER_H
#define MARCH25_ORDER_H

typedef struct {
    int id;
    int clientId;
    char date[11];
    int total;
} Order;

typedef struct {
    int orderId;
    int productId;
    int quantity;
    int subtotal;
} ItemOrder;

typedef struct {
    Order *data;
    int count;
    int capacity;
} OrderList;

void initOrderList(OrderList *list);
int addOrder(OrderList *list, Order order);
int removeOrder(OrderList *list, int id);
int loadOrdersToCSV(OrderList *list, const char *fileName);

#endif //MARCH25_ORDER_H