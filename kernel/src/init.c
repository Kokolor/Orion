#include <stdint.h>
#include <stdarg.h>
#include <terminal/terminal.h>
#include <gdt/gdt.h>

int _start(bootinfo_t *bootinfo)
{
    init_terminal(main_terminal, bootinfo->font, bootinfo->framebuffer, bootinfo->width, bootinfo->height, bootinfo->pitch);
    clear_terminal(0x29282B);
    init_gdt();
    log("SUCCESS", "GDT initialized.");
    log("INFO", "Framebuffer address: 0x%p", bootinfo->framebuffer);
    log("INFO", "Kernel memory map size: %llx", bootinfo->mem_map_size);
    log("INFO", "Memory map descriptor size: %llu", bootinfo->mem_map_desc_size);

    while (1)
        ;
}