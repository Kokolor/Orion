#ifndef FAT_H
#define FAT_H

#include <stdint.h>
#include <lib/lib.h>

typedef struct
{
    uint8_t bootjmp[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t table_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t media_type;
    uint16_t table_size_16;
    uint16_t sectors_per_track;
    uint16_t head_side_count;
    uint32_t hidden_sector_count;
    uint32_t total_sectors_32;
    uint8_t extended_section[54];
} __attribute__((packed)) Bootsector;

void read_sector(int disk, uint32_t lba, uint8_t* buffer);

#endif
