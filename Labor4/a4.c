/* Erweitern Sie das Programm so, dass die Kanäle 0, 1 ,2 und 3 an der LCD Anzeige
 * als 10 Bit Wert angezeigt werden. In jeder Zeile des Displays soll ein Wert stehen.
 * ( siehe LCD-Funktionen dazu)
 */

#include "xmc_common.h"
//#include "XMC1100_lib.h"
#include "XMC4500_lib.h"

int main(void) {
    uint8_t i;
    uint16_t x;
    adc_init ();
    lcd_init();
    lcd_clear();

    while (1U) {
        // Messwerte lesen
        for(i = 0; i < 4; i++) {
            lcd_setcursor(i+1,1);
            lcd_int((adc_in(i) >> 2));
        }
        for (x = 0; x < 1000000; x++);
    }
}