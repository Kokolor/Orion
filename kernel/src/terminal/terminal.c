#include <terminal/terminal.h>

Terminal *current_terminal;
Terminal *main_terminal;

void init_terminal(Terminal *terminal, PSF1_FONT *font, unsigned int *framebuffer, unsigned int width, unsigned int height, unsigned int pitch)
{
        terminal->font = font;
        terminal->framebuffer = framebuffer;
        terminal->width = width;
        terminal->height = height;
        terminal->pitch = pitch;
        terminal->cursor_x = 0;
        terminal->cursor_y = 0;
        terminal->color = 0xFFFFFF;
        current_terminal = terminal;
}

void write_to_serial_port(char c, uint16_t port)
{
        while ((inb(port + 5) & 0x20) == 0)
                ;
        outb(port, c);
}

void switch_terminal(Terminal *terminal)
{
        current_terminal = terminal;
}

void clear_terminal(uint32_t color)
{
        unsigned int *pixel = current_terminal->framebuffer;
        for (unsigned int y = 0; y < current_terminal->height; ++y)
        {
                for (unsigned int x = 0; x < current_terminal->width; ++x)
                {
                        *pixel++ = color;
                }
                pixel += (current_terminal->pitch / 4) - current_terminal->width;
        }
}

void terminal_putpixel(unsigned int x, unsigned int y, uint32_t color)
{
        unsigned int *pixel = current_terminal->framebuffer + (y * (current_terminal->pitch / 4) + x);
        *pixel = color;
}

uint32_t terminal_getpixel(unsigned int x, unsigned int y)
{
        unsigned int *pixel = current_terminal->framebuffer + (y * (current_terminal->pitch / 4) + x);
        return *pixel;
}

void scroll_up()
{
        uint32_t color;

        for (int y = 0; y < current_terminal->height - 16; y++)
        {
                for (int x = 0; x < current_terminal->width; x++)
                {
                        color = terminal_getpixel(x, y + 16);
                        terminal_putpixel(x, y, color);
                }
        }

        for (int i = 0; i < 0 + current_terminal->width; i++)
        {
                for (int j = current_terminal->height - 15; j < current_terminal->height - 16 + 16; j++)
                {
                        terminal_putpixel(i, j, color);
                }
        }
}

void print_character(char c)
{
        if (c == '\n')
        {
                current_terminal->cursor_x = 0;
                current_terminal->cursor_y += 16;
        }
        if (current_terminal->cursor_x >= current_terminal->width)
        {
                current_terminal->cursor_x = 0;
                current_terminal->cursor_y += 16;
        }

        if (current_terminal->cursor_y >= current_terminal->height)
        {
                scroll_up();
                current_terminal->cursor_y -= 16;
        }

        if (c != '\n')
        {
                uint8_t *offset = (uint8_t *)current_terminal->font->glyphBuffer + (c * current_terminal->font->psf1_Header->charsize);

                for (int i = 0; i < 16; i++)
                {
                        for (int j = 0; j < 8; j++)
                        {
                                if (offset[i] & (1 << j))
                                {
                                        terminal_putpixel(current_terminal->cursor_x + 8 - j, current_terminal->cursor_y + i, current_terminal->color);
                                }
                        }
                }
                current_terminal->cursor_x += 8;
        }

        write_to_serial_port(c, 0x3F8);
}

void print_string(char *str)
{
        for (int i = 0; str[i] != '\0'; i++)
        {
                print_character(str[i]);
        }
}

void print_integer(int v, int base, const char *digits)
{
        char buf[33];
        char *ptr = &buf[sizeof(buf) - 1];
        *ptr = '\0';

        if (v == 0)
        {
                print_character('0');
                return;
        }

        if (v < 0 && base == 10)
        {
                print_character('-');
                v = -v;
        }

        while (v)
        {
                *--ptr = digits[v % base];
                v /= base;
        }

        print_string(ptr);
}

void print_integer_64(uint64_t v, int base, const char *digits)
{
        char buf[65];
        char *pointer = &buf[sizeof(buf) - 1];
        *pointer = '\0';

        if (v == 0)
        {
                print_character('0');
                return;
        }

        while (v)
        {
                *--pointer = digits[v % base];
                v /= base;
        }

        print_string(pointer);
}

void vprintf(char *format, va_list args)
{
        const char *hex_digits = "0123456789ABCDEF";

        while (*format != '\0')
        {
                if (*format == '%')
                {
                        format++;
                        switch (*format)
                        {
                        case 's':
                        {
                                char *str = va_arg(args, char *);
                                print_string(str);
                                break;
                        }
                        case 'c':
                        {
                                char c = (char)va_arg(args, int);
                                print_character(c);
                                break;
                        }
                        case 'd':
                        {
                                int d = va_arg(args, int);
                                print_integer(d, 10, hex_digits);
                                break;
                        }
                        case 'u':
                        {
                                unsigned int u = va_arg(args, unsigned int);
                                print_integer(u, 10, hex_digits);
                                break;
                        }
                        case 'x':
                        {
                                int x = va_arg(args, int);
                                print_integer(x, 16, hex_digits);
                                break;
                        }
                        case 'p':
                        {
                                void *p = va_arg(args, void *);
                                print_integer_64((uint64_t)p, 16, hex_digits);
                                break;
                        }
                        case 'l':
                        {
                                format++;
                                if (*format == 'l')
                                {
                                        format++;
                                        if (*format == 'u')
                                        {
                                                uint64_t llu = va_arg(args, uint64_t);
                                                print_integer_64(llu, 10, hex_digits);
                                        }
                                        else if (*format == 'x')
                                        {
                                                uint64_t llx = va_arg(args, uint64_t);
                                                print_integer_64(llx, 16, hex_digits);
                                        }
                                        else
                                        {
                                                print_string("Invalid format specifier");
                                        }
                                }
                                else
                                {
                                        print_string("Invalid format specifier");
                                }
                                break;
                        }
                        default:
                                print_character('%');
                                print_character(*format);
                                break;
                        }
                }
                else
                {
                        print_character(*format);
                }
                format++;
        }
}

void printf(char *format, ...)
{
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
}

void log(char *level, char *format, ...)
{
        uint32_t original_color = current_terminal->color;
        current_terminal->color = 0xCCCCCC;
        printf("[ ");

        if (strcmp(level, "SUCCESS") == 0)
        {
                current_terminal->color = COLOR_SUCCESS;
        }
        else if (strcmp(level, "INFO") == 0)
        {
                current_terminal->color = COLOR_INFO;
        }
        else if (strcmp(level, "WARNING") == 0)
        {
                current_terminal->color = COLOR_WARNING;
        }
        else if (strcmp(level, "ERROR") == 0)
        {
                current_terminal->color = COLOR_ERROR;
        }

        printf("%s", level);
        current_terminal->color = 0xCCCCCC;
        printf(" ] ");

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\n");
        current_terminal->color = original_color;
}
