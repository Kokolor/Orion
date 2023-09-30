#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <stdarg.h>
#include <taha.h>

typedef struct
{
        PSF1_FONT *font;
        unsigned int *framebuffer;
        unsigned int width;
        unsigned int height;
        unsigned int pitch;
        unsigned int cursor_x;
        unsigned int cursor_y;
        uint32_t color;
} Terminal;

extern Terminal *current_terminal;
extern Terminal *main_terminal;

void init_terminal(Terminal *terminal, PSF1_FONT *font, unsigned int *framebuffer, unsigned int width, unsigned int height, unsigned int pitch);
void switch_terminal(Terminal *terminal);
void clear_terminal(uint32_t color);
void terminal_putpixel(unsigned int x, unsigned int y, uint32_t color);
uint32_t terminal_getpixel(unsigned int x, unsigned int y);
void printf(char *format, ...);

#endif