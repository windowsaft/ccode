#include "DAVE.h"

int main (void) {
    DAVE_Init();
    /* 
     * Initialization von DAVE APPs
     * --> DAVE APP muss hinzugefügt werden,
     *  Einstellunge über das GUI
     */

    PORT0->IOCR0 = 0x80808080;

    while (1) {
        /* code */
    }

    return 0;
}

void RX_ISR (void) {
    /*
     * Diese Funktion wurde über das GUI hinterlegt,
     * sodass sie aufgerufen wird, wenn die serielle
     * Schnittstelle Daten emfängt.
     * 
     * Ports werden auch im GUI festgelegt.
     */
    
    /* something */
}