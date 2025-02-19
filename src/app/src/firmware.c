#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdio.h>
// #include <libopencm3/cm3/scb.h>

#include "app/inc/core/system.h"
#include "uart/inc/uart.h"

// #define BOOTLOADER_SIZE (0x8000U)

#define LED_PORT        (GPIOG)
#define LED_PIN         (GPIO13)

#define UART_PORT       (GPIOA)
#define RX_PIN          (GPIO3)
#define TX_PIN          (GPIO2)


//! vector setup, permitting bootloader
// static void vector_setup(void) {
//     SCB_VTOR = BOOTLOADER_SIZE;
// }


// set cf for gpio, enable gpio PG13
static void gpio_setup(void) {
    // LED SETUP
    rcc_periph_clock_enable(RCC_GPIOG);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);   

    // UART setup
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TX_PIN | RX_PIN);
    gpio_set_af(UART_PORT, GPIO_AF7, TX_PIN | RX_PIN);


}


int main(void) {

    // vector_setup();
    
    system_setup();
    gpio_setup();
    uart_setup();


    uint64_t start_time = system_get_ticks();

    while (1) {
        if(system_get_ticks() - start_time >= 1000){
            gpio_toggle(LED_PORT, LED_PIN);
            start_time = system_get_ticks();
        }
        
        if (uart_data_available()) {
            uint8_t data = uart_read_byte();
            uart_write_byte(data + 1);  
        }

        // more useful work
        
    }

    return 0;
}
