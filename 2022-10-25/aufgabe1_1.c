// Aufgabe 1 in der Vorlesung (Servo-Aufgabe)

#include <DAVE.h>
#include <xmc4500_lib.h>

#define periode 37500
#define vorteiler 0x6

uint16_t adcwert;
uint16_t wert1;

void init_timer(void);

int main(void) {
    init_timer();
    adc_init();
    while (1) {
        adcwert = adc_in(0);

        wert1 = periode - (1875 + (adcwert * (1875.0/4095))); //.0 damit float gerechnet wird sonst division immer 0
        CCU42_CC40->CRS = wert1; // Neuen Compare Wert laden  --> macht pwm möglich
        CCU42->GCSS |= 1;       // Shadow Transfer Request
    }

    return 0;
}


void timer_init(void) {
    // Vorteiler: 64 0b0110 = 0x6
    // Periode: 
    ccu4x_enable();                 // Freigabe CCU42 Timermodul

    CCU42->GIDLC = 0x10F;           // Vorteiler enable, IDLE CCU40 disable
    CCU42_CC40->TC = 0x04;          // Shadow transfer enable on clear
    CCU42_CC40->PSC = 0x6;          // Vorteiler, 
    CCU42_CC40->PRS = 37502343;      // Period Register set für 500 ms
    
    // CCU42 Compare (Ausgangspin 3)
    CCU42_CC40->CRS = 1500
    CCU42->GCSS = 1;                // Shadow transfer request
    PORT3->IOCR0 = 0x98;            //Alt3 =>OUT bei P3.0    
    CCU40_CC40->TCSET = 0x01;       // starts counting
}