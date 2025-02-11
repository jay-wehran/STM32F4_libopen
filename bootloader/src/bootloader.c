#include "common-defines.h"
#include <libopencm3/stm32/memorymap.h>

#define BOOTLOADER_SIZE             (0x8000U)                                   // where bootloader is
#define MAIN_APP_START_ADDRESS      (FLASH_BASE + BOOTLOADER_SIZE)              // main app address

//!Wont fit
const uint8_t data[0x8000] = {0};

// 1.find interrupt vector table (start of flash)
// 2.find first entry that isn't SP (reset vector)
// 3. read address that is in that table 
// 4. convert address to function
// 5. call function
void jump_to_main(void) {
    typedef  void (*void_fn)(void); 

    uint32_t* reset_vector_entry = (uint32_t*)(MAIN_APP_START_ADDRESS+ 4U);     // pointer to start of reset vector
    uint32_t* reset_vector = (uint32_t*)(*reset_vector_entry);                  // reset_vector is now a pointer to address
    void_fn jump_fn = (void_fn)reset_vector;                                    // Setting reset vector to function pointer

    jump_fn(); 
}

int main(void) {
//! Wont fit
    volatile uint8_t x = 0;
    for (uint32_t i = 0; i < 0x8000; i++) {
        x += data[i];
    }


    jump_to_main();

    return 0;
}