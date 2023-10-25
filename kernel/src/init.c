#include <stdint.h>
#include <stdarg.h>
#include <terminal/terminal.h>
#include <gdt/gdt.h>
#include <fat/fat.h>

int _start(bootinfo_t *bootinfo)
{
    init_terminal(main_terminal, bootinfo->font, bootinfo->framebuffer, bootinfo->width, bootinfo->height, bootinfo->pitch);
    clear_terminal(0x29282B);
    init_gdt();
    log("SUCCESS", "GDT initialized.");
    log("INFO", "Framebuffer address: 0x%p", bootinfo->framebuffer);
    log("INFO", "Kernel memory map size: %llx", bootinfo->mem_map_size);
    log("INFO", "Memory map descriptor size: %llu", bootinfo->mem_map_desc_size);

    uint8_t buffer[512];
    read_sector(0, 0, buffer);

    Bootsector* boot_sector = (Bootsector*)buffer;

    log("INFO", "OEM Name: %s", boot_sector->oem_name);
    log("INFO", "Bytes per sector: %d", boot_sector->bytes_per_sector);
    log("INFO", "Sectors per cluster: %d", boot_sector->sectors_per_cluster);
    log("INFO", "Reserved sectors: %d", boot_sector->reserved_sector_count);
    log("INFO", "Total sectors: %d", boot_sector->total_sectors_32);

    while (1)
        ;
}
