/* Programmieren Sie eine Abgleichhilfe für AN0 nach folgenden Vorgaben
 */

#include "xmc_common.h"
//#include "XMC1100_lib.h"
#include "XMC4500_lib.h"

int main(void) {
    uint8_t i;
    uint16_t mw;
    PORT0->IOCR0 = 0x80808080;
	PORT0->IOCR4 = 0x80808080;
    adc_init ();

    while (1U) {
        // Messwerte lesen
        mw = adc_in(0);
        if (mw < 2482) PORT0->OUT = 0x1;
        else {
            if (mw > (1241*3)) PORT0->OUT = 0x20;
            else PORT0->OUT = 0x40; 
        }
    }
}