#include <stdio.h>
#include "os.h"

#define NUM_PAGES 100

int main() {
    // Allocate a page table
    uint64_t pt = alloc_page_frame() << 12;
    
    // Initialize the page table to map even virtual pages to even physical pages
    for (int i = 0; i < NUM_PAGES; i += 2) {
        page_table_update(pt, i, i);
    }
    
    // Verify that the mapping is correct
    for (int i = 0; i < NUM_PAGES; i += 2) {
        uint64_t ppn = page_table_query(pt, i);
        if (ppn != i) {
            printf("Error: mapping for VPN %d is incorrect (expected %d, got %d)\n", i, i, ppn);
        }
    }
    
    // Update the page table to map odd virtual pages to odd physical pages
    for (int i = 1; i < NUM_PAGES; i += 2) {
        page_table_update(pt, i, i);
    }
    
    // Verify that the mapping is correct
    for (int i = 0; i < NUM_PAGES; i++) {
        uint64_t ppn = page_table_query(pt, i);
        if (i % 2 == 0) {
            if (ppn != i) {
                printf("Error: mapping for VPN %d is incorrect (expected %d, got %d)\n", i, i, ppn);
            }
        } else {
            if (ppn != i) {
                printf("Error: mapping for VPN %d is incorrect (expected %d, got %d)\n", i, i, ppn);
            }
        }
    }
    
    // Deallocate the page table
    for (int i = 0; i < NUM_PAGES; i++) {
        uint64_t ppn = page_table_query(pt, i);
        if (ppn != NO_MAPPING) {
            page_table_update(pt, i, NO_MAPPING);
        }
    }
    free_page_frame(pt >> 12);
    
    printf("Test complete.\n");
    return 0;
}
