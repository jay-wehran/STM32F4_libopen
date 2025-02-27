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
#define TX_PIN          (GPIO9)
#define RX_PIN          (GPIO10)

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

static void store_byte(uint8_t* buffer, uint8_t data, uint8_t* iterator) {
    buffer[*iterator] = data;
    (*iterator)++;
}

static void clear_buffer(uint8_t* buffer, uint8_t* iterator) {
    for (uint8_t i = 0; i <= *iterator; i++) {
        buffer[i] = 0;
    }

    *iterator = 0;
}


int main(void) {

    // vector_setup();
    
    system_setup();
    gpio_setup();
    uart_setup();

    uint8_t iterator = 0;
 
    
     uint8_t buffer[128] = {};

     uart_write((uint8_t*)"0 to reset \n", 11);





    // uint64_t start_time = system_get_ticks();

    while (1) {

        // if(system_get_ticks() - start_time >= 1000){
        //     gpio_toggle(LED_PORT, LED_PIN);
        //     start_time = system_get_ticks();
        // }

        

        while (uart_data_available()) {
            uint8_t data = uart_read_byte();
            if (data != '5') {
                store_byte(buffer,data,&iterator);
            }
            if (data == '5' && buffer[0] == 'o' && buffer[1] == 'n') {
                uart_write((uint8_t*) "On!\n", 3);
                gpio_toggle(LED_PORT, LED_PIN);
                clear_buffer(buffer, &iterator);
            }
            if (data == '5' && buffer[0] == 'o' && buffer[1] == 'f' && buffer[2] == 'f'){
                uart_write((uint8_t*) "Off!\n", 4);
                gpio_clear(LED_PORT, LED_PIN);
                clear_buffer(buffer, &iterator);
            }
            if (data == '0'){
                clear_buffer(buffer, &iterator);
            }
        }
        
    }

    return 0;
}
