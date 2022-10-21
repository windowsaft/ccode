#include "xmc_common.h"
#include "xmc4500_lib.h"

void init_timer(void);
void init_port(void);

uint16_t phasen[] = {0x0E, 0x2C, 0x23, 0x0B, 0x38, 0x32};
uint16_t phasen2[] = {0x32, 0x38, 0x0B, 0x23, 0x2C, 0x0E};
uint8_t x = 0;


int main (void) {
    init_port();
    init_timer();

    //Interrupt Freigaben
    CCU40_CC40->INTE |= 1;
    NVIC_EnableIRQ(CCU40_0_IRQn);


    while (1);
}

void init_port(void) {
    PORT0->IOCR0 = 0x80808080;
    PORT0->IOCR4 = 0x80808080;
    PORT3->IOCR0 = 0x00000000;
}

void init_timer(void) {
    ccu4x_enable();
    CCU40->GIDLC = 0x10F;
    CCU40_CC40->TC = 0x08;
    CCU40_CC40->PSC = 0x7;
    CCU40_CC40->PRS = 37500;
    CCU40->GCSS = 1;
    CCU40_CC40->TCSET = 1;
}

void CCU40_0_IRQHandler(void) {
	if ((PORT3->IN & 0b0001) == 1) {
		if (x < 6) {
			PORT0->OUT = phasen[x];
			x++;
		}

		else x = 0;
	}

	else {
		if (x < 6) {
			PORT0->OUT = phasen2[x];
			x++;
		}

		else x = 0;
	}
}
