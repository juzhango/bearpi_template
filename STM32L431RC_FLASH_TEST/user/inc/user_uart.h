//
// Created by luozw on 2021/3/6.
//

#ifndef UART_USER_UART_H
#define UART_USER_UART_H

#define FIFO_MAX_SIZE   512

typedef struct {
    unsigned int    rx_read_point;
    unsigned int    rx_write_point;
    unsigned int    rx_counter;
    unsigned char   rx_ring_buf[FIFO_MAX_SIZE];
} ST_UART_FIFO;


void fifo_init(ST_UART_FIFO *fifo);
unsigned int __fifo_put( ST_UART_FIFO *fifo, unsigned char *buffer, unsigned int len);
unsigned int __fifo_get( ST_UART_FIFO *fifo, unsigned char *buffer, unsigned int len);

#endif //UART_USER_UART_H
