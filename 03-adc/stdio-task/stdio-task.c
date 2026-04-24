#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "stdio-task.h"

#define CMD_BUFFER_SIZE 64

static char cmd_buffer[CMD_BUFFER_SIZE];
static int cmd_index = 0;

void stdio_task_init(void)
{
    cmd_index = 0;
    memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
}

char* stdio_task_handle(void)
{
    // Используем getchar() с блокировкой, но с таймаутом
    int ch = getchar_timeout_us(1000);  // ждём 1 мс
    
    if (ch != PICO_ERROR_TIMEOUT)
    {
        // Обработка разных вариантов окончания строки
        if (ch == '\r' || ch == '\n')
        {
            if (cmd_index > 0)
            {
                cmd_buffer[cmd_index] = '\0';
                printf("\n");  // перевод строки для красоты
                cmd_index = 0;
                return cmd_buffer;
            }
        }
        else if (ch == '\b' || ch == 127)  // Backspace
        {
            if (cmd_index > 0)
            {
                cmd_index--;
                printf("\b \b");  // удаляем символ в терминале
            }
        }
        else if (cmd_index < CMD_BUFFER_SIZE - 1)
        {
            // Печатаем только печатные символы
            if (ch >= 32 && ch <= 126)
            {
                cmd_buffer[cmd_index++] = (char)ch;
                putchar((char)ch);  // эхо
            }
        }
    }
    
    return NULL;
}