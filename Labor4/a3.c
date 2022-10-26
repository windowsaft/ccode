/* Nehmen Sie den AD-Wandler mit der vorhandenen Library XMCxx00_lib.C in Betrieb und geben
   Sie die 8 höchstwertigen Bits von AN0 an Port 1 aus
*/

#include "xmc_common.h"
#include "XMC1100_lib.h"

int main(void) {
    uint16_t mw;
	PORT1->IOCR8 = 0x80808080;
	PORT1->IOCR12 = 0x80808080;
    adc_init ();

    while (1U) {
        mw = adc_in(0);
        PORT1->OUT = (mw >> 4) << 8;
    }
}