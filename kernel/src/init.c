#include <stdint.h>
#include <stdarg.h>
#include <terminal/terminal.h>
#include <gdt/gdt.h>

int _start(bootinfo_t *bootinfo)
{
    init_terminal(main_terminal, bootinfo->font, bootinfo->framebuffer, bootinfo->width, bootinfo->height, bootinfo->pitch);
    clear_terminal(0x000022);
    init_gdt();

    while (1)
        ;
}