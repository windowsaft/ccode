// Aufgabe 1 in der Vorlesung (Servo-Aufgabe)

#include <DAVE.h>
#include <xmc4500_lib.h>

#define periode 37500
#define vorteiler 0x6

void timer_init(void);
void timer_pwm (uint16_t winkel);

uint32_t adcwert, winkel;
uint32_t comparewert;
uint32_t wert1;

int main (void) {

    timer_init();
    adc_init();

    while (1) {
        adcwert = acc_in(0);
        winkel = adcwert * (180.0/4095);
        timer_pwm(winkel);
    }

    return 0;
}

void timer_init(void) {
    // Vorteiler: 64 0b0110 = 0x6
    // Periode: 
    ccu4x_enable();                 // Freigabe CCU42 Timermodul

    CCU42->GIDLC = 0x10F;           // Vorteiler enable, IDLE CCU40 disable
    CCU42_CC40->TC = 0x04;          // Shadow transfer enable on clear
    CCU42_CC40->PSC = periode;          // Vorteiler, 
    CCU42_CC40->PRS = vorteiler;      // Period Register set für 500 ms
    
    // CCU42 Compare (Ausgangspin 3)
    CCU42_CC40->CRS = 1500
    CCU42->GCSS = 1;                // Shadow transfer request
    PORT3->IOCR0 = 0x98; //Alt3 =>OUT bei P3.0    
    CCU40_CC40->TCSET = 0x01;       // starts counting
}

void timer_pwm (uint8_t winkel) {
    /*
    0°   = 1mSek
    90°  = 1,5mSek
    180° = 2mSek
    */

    wert1 = periode - ( 1875 + (winkel * (10.416))); //winkel 0 ... 180
    CCU42_CC40->CRS = wert1; // Neuen Compare Wert laden  --> macht pwm möglich
    CCU42->GCSS |= 1;       // Shadow Transfer Request
}