#include "xmc_common.h"

void init_timer(void);
void init_port(void);

uint16_t phasen[] = {0x0E, 0x2C, 0x23, 0x0B, 0x38, 0x32};
uint8_t x;

int main (void) {
    init_port();
    init_timer();

    //Interrut Freigaben
    CCU40_CC40->INTE |= 1;
    NVIC_EnableIRQ(CCU40_0_IRQn);


    while (1) ;   
}

void init_port(void) {
    PORT0->IOCR0 = 0x80808080;
    PORT0->IOCR4 = 0x80808080;
}

void init_time(void) {
    ccu4x_enable();
    CCU40->GIDLC = 0x10F;
    CCU40_CC40->TC = 0x04;
    CCU40_CC40->PSC = 0xA;
    CCU40_CC40->PRS = 58596;
    CCU40->GCSS = 1;
    CCU40_CC40->TCSET = 1;
}

void CCU40_0_IRQHandler(void) {
    if (x < 6) {
        PORT0->OUT = phasen[x];
        x++;
    }
    
    else x = 0;
}