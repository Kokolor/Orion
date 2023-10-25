#include <fat/fat.h>

void read_sector(int disk, uint32_t lba, uint8_t* buffer)
{
    outb(0x1F6, disk == 0 ? 0xA0 : 0xB0);

    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t)(lba & 0xFF));
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F6, 0xE0 | ((disk & 1) << 4) | ((lba >> 24) & 0x0F));

    outb(0x1F7, 0x20);

    while (!(inb(0x1F7) & 0x08))
    {
    }

    for (int i = 0; i < 256; i++)
    {
        ((uint16_t*)buffer)[i] = inw(0x1F0);
    }
}
