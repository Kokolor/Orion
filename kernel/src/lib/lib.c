#include <lib/lib.h>

void outb(uint16_t port, uint8_t val)
{
        asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
        uint8_t ret;
        asm volatile("inb %1, %0"
                     : "=a"(ret)
                     : "Nd"(port));
        return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

int strcmp(const char *str1, const char *str2)
{
        while (*str1 != '\0' && *str2 != '\0')
        {
                if (*str1 != *str2)
                {
                        return *str1 - *str2;
                }
                str1++;
                str2++;
        }

        return *str1 - *str2;
}
