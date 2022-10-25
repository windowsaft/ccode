#include "DAVE.h"

int main (void) {
    DAVE_Init(); // Initialization of DAVE APPs --> DAVE APP muss hinzugefügt werden, Einstellunge über GUI
    PORT0->IOCR0 = 0x80808080;

    while (1) {
        /* code */
    }

    return 0;
}

void ext_ISR (void) {
    /*
     * Diese Funktion wurde über das GUI hinterlegt,
     * sodass sie beim Interrupt ausgeführt wird.
     * 
     * Impuls liegt z.B. bei P3.1 an.
     */
    PORT0->OUT++;
}