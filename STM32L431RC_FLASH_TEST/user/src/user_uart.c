//
// Created by luozw on 2021/3/6.
//
#include "user_uart.h"
#include "main.h"



#define min(a, b)				(((a) < (b)) ? (a) : (b))

void fifo_init(ST_UART_FIFO *fifo)
{
    fifo->rx_counter = 0;
    fifo->rx_write_point = 0;
    fifo->rx_read_point = 0;
    for(unsigned int i = 0; i < FIFO_MAX_SIZE; i++)
    {
        fifo->rx_ring_buf[i] = 0;
    }
}

unsigned int __fifo_put(ST_UART_FIFO *fifo, unsigned char *buffer, unsigned int len)
{
    len = min(len, (FIFO_MAX_SIZE - fifo->rx_counter));

    for(unsigned int i = 0; i < len; i++)
    {
        fifo->rx_ring_buf[fifo->rx_write_point] = *buffer;
        uint8_t a = fifo->rx_ring_buf[fifo->rx_write_point];
        fifo->rx_counter++;
        buffer++;
        fifo->rx_write_point++;
        if(fifo->rx_write_point >= FIFO_MAX_SIZE)
        {
            fifo->rx_write_point = 0;
        }
    }
    return len;
}

unsigned int __fifo_get(ST_UART_FIFO *fifo, unsigned char *buffer, unsigned int len)
{
    len = min(len, fifo->rx_counter);
    for(unsigned int i = 0; i < len; i++)
    {
        *buffer = fifo->rx_ring_buf[fifo->rx_read_point];
        fifo->rx_counter--;
        buffer++;
        fifo->rx_read_point++;
        if(fifo->rx_read_point >= FIFO_MAX_SIZE)
        {
            fifo->rx_read_point = 0;
        }
    }
    return len;
}






