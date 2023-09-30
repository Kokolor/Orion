#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdarg.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
int strcmp(const char *str1, const char *str2);

#endif