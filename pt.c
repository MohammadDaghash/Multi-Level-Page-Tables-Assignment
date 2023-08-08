#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "os.h"

#define PAGE_SIZE 4096
#define PTE_SIZE 8
#define PAGE_ENTRIES (PAGE_SIZE / PTE_SIZE)
#define NUM_LEVELS 5
#define LEVEL_MASK 0x1ff

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    uint64_t* current_table = phys_to_virt(pt << 12);
    uint64_t level_entry;
    
    int level = NUM_LEVELS - 1;
    while (level > 0) {
        level_entry = (vpn >> (9 * level)) & LEVEL_MASK;
        if ((current_table[level_entry] & 1) == 0) {
            if (ppn == NO_MAPPING) {
                return;
            }
            current_table[level_entry] = (alloc_page_frame() << 12) | 1;
        }
        current_table = phys_to_virt(current_table[level_entry] & ~0xfffULL);
        --level;
    }

    level_entry = vpn & LEVEL_MASK;
    if (ppn == NO_MAPPING) {
        current_table[level_entry] = 0;
    } else {
        current_table[level_entry] = (ppn << 12) | 1;
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    uint64_t* current_table = phys_to_virt(pt << 12);
    uint64_t level_entry;
    
    int level = NUM_LEVELS - 1;
    while (level > 0) {
        level_entry = (vpn >> (9 * level)) & LEVEL_MASK;
        if ((current_table[level_entry] & 1) == 0) {
            return NO_MAPPING;
        }
        current_table = phys_to_virt(current_table[level_entry] & ~0xfffULL);
        --level;
    }

    level_entry = vpn & LEVEL_MASK;
    if ((current_table[level_entry] & 1) == 0) {
        return NO_MAPPING;
    }
    return current_table[level_entry] >> 12;
}





