#include "xmc_common.h"

void port_init(void);
uint32_t wert;

int main (void) {
    port_init();

    while (1U) {
        wert = wert << 1;
        PORT0->OUT = wert;
    }
    
}


void port_init(void) {
    PORT0->IOCR0 = 0x80808080;
}