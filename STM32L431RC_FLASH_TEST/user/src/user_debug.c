//
// Created by luozw on 2021/3/7.
//

#include "user_debug.h"

void u_print(const char *fmt, ...) // custom printf() function
{
    unsigned char string[80];

    va_list arg;
    va_start(arg, fmt);
    if (0 < vsprintf((char *)string, fmt, arg)) // build string
    {
        HAL_UART_Transmit(&huart1, string, strlen((const char *)string), 0xffffff); // send message via UART
    } else {}
    va_end(arg);
}
