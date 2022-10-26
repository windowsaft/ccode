/* Schreiben Sie ein Programm, das die Spannung von AN0 in Volt mit 1/100 Ausgabe ausgibt.
 * Beispiel für die Ausgabe: U = 2.50 V
 * Hinweis: Benutzen Sie zur Ausgabe der Ziffern die Funktion „asc_char“ und wandeln Sie den Wert
 * des AD-Wandlers vorher in 3 ASCII Zeichen um. Eventuellen Überlauf von Daten beachten!
 */

/*
 * 3.30V: 4095 / x = 330
 *               x = 12.4
 */


#include "xmc_common.h"
//#include "XMC1100_lib.h"
#include "XMC4500_lib.h"

int main(void) {
    uint16_t m, dec;
    adc_init ();
    lcd_init();
    lcd_clear();

    while (1U) {
        m = adc_in(0);
        m = (m / 12.4)
        lcd_print( "U = " );
        lcd_char((dec / 100) + 48);
        lcd_print( "." );
        lcd_char(((dec % 100) / 10) + 48);
        lcd_char((dec % 10) +4 8);
        lcd_print( " V" );
        // Messwerte lesen
    }
}