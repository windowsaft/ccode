#include "xmc_common.h"


void init_timer(uint16_t vorteiler, uint16_t periode);
void ccu40_enable(void);

int main (void) {
    PORT0->IOCR0 = 0x80808080;
    PORT0->IOCR4 = 0x80808080;

    init_timer(5, 0x8000);
    while (1) ;

    return 0;
}

void init_timer(uint16_t vorteiler, uint16_t periode) {
    ccu4x_enable();                 // Freigabe CCU42 Timermodul

    CCU40->GIDLC = 0x10F;           // Vorteiler enable, IDLE CCU40 disable
    CCU40_CC40->TC = 0x04;          // Shadow transfer enable on clear
    CCU40_CC40->PSC = vorteiler;    // Vorteiler, hier 120Mhz/1024 => clock
    CCU40_CC40->PRS = periode;      // Period Register set für 500 ms
    CCU40->GCSS = 1;                // Shadow transfer request
    CCU40_CC40->TCSET = 0x01;       // starts counting
}