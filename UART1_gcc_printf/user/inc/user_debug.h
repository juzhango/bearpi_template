//
// Created by luozw on 2021/3/7.
//

#ifndef UART_USER_DEBUG_H
#define UART_USER_DEBUG_H
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "usart.h"

void u_print(const char *fmt, ...);
#endif //UART_USER_DEBUG_H
