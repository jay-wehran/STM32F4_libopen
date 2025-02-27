#ifndef SPI_H
#define SPI_H

#include "common-defines.h"

void spi5_setup(void);
void spi5_send_byte(uint8_t data);
uint8_t spi5_recieve_byte(void);
uint8_t spi5_transfer(uint8_t data);


#endif // SPI_H