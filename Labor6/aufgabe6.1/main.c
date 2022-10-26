/*
 * main.c
 *
 *  Created on: 2022 Oct 25 12:38:16
 *  Author: Dennis
 */
/*
 * Aufgabe 6.1 bis 6.3
 * Erzeugen sie an einem Controllerausgang einen Triggerimpuls von ca. 100µs Länge.
 * Mit diesem Triggerimpuls wird beim Ultraschallsensor ein Messvorgang gestartet
 * (vgl. Timing Diagramm). Der Triggerimpuls soll periodisch alle 500ms wiederholt werden.
 *
 *   • Messen sie mit dem Oszilloskop den Triggerimpuls und den vom Sensor erzeugten Echoimpuls.
 *   • Simulieren sie mit einem Gegenstand verschiedene Distanzen (Auswirkung auf den Echoimpuls).
 *   • Überprüfen sie rechnerisch den Zusammenhang: Breite des Echoimpulses - Distanz
 */


#include "DAVE.h"                       //Declarations from DAVE Code Generation (includes SFR declaration)
#include "XMC4500.h"
#include "xmc4500_lib.h"

uint8_t state = 0;
uint32_t ticks;
uint16_t abstand;

void init_pwm(uint32_t vorteiler, uint32_t periode, uint32_t w);
void init_timer(void);
void ccu40_enable ( void );

int main(void) {
  DAVE_Init();                          // Initialization of DAVE APPs  --> PIN Interrupt
  init_pwm(0b1010, 58596, 58584);       // pwm-timer Vorteiler: 0b1010 ... Periode: 58596 = 500ms/8.533us = 12 ... Compare-Wert = 58584 = 58596 - (100us/500ms)*58596
  init_timer();                         // timer

  lcd_init();                           // LC-Display
  lcd_clear();                          // LC-Display

  while(1U) {                           
	  if (state == 1) {                   // Abfrage nach der Falling Edge
		  lcd_setcursor(1,1);
		  ticks = ticks * 533.33/1000.0;			                                  // Umrechnung: Ticks in Mikrosekunden
		  abstand = 344 * 0.5 * ticks * (1.0/1000000.0) * 100.0;                // Umrechnung: Mikrosekunden in cm
		  lcd_int(abstand);                                                     // Abstand aufs Display Ausgeben
		  lcd_char(0x63);                                                       // Einheit augeben
		  lcd_char(0x6D);                                                       // Einheit augeben
		  state = 0;
	  }
  }
}

void IRQHandler(void) {                                                     // Interrupt Handler --> Wenn das PWM Signal eine rising / falling edge hat
  if (PIN_INTERRUPT_GetPinValue(&PIN_INTERRUPT_0) > 0) {                    // case detection: rising oder falling edge?
	  CCU40_CC40->TCSET = 0x1;                                                // Rising Edge: Timer starten
	  state = 2;
  }
  else {                                                                    // Falling Edge
	  CCU40_CC40->TCCLR = 1;                                                  // Timer Stop
	  ticks = CCU40_CC40->TIMER;                                              // Timer Wert auslesen
	  CCU40_CC40->TCCLR = 0x2;                                                // Timer clear
	  state = 1;
  }
}

void init_pwm(uint32_t vorteiler, uint32_t periode, uint32_t w) {
  /*
    *   PWM init
    *   Muss vor init_timer() ausgeführt werden
    *   CCU42-Timermodul
    */
  ccu4x_enable();                   // Freigabe CCU Timermodule
  CCU42->GIDLC = 0x10F;             // Vorteiler enable, IDLE CCU40 disable
  CCU42_CC40->TC = 0x04;            // Shadow transfer enable on clear
  CCU42_CC40->PSC = vorteiler;      // Vorteiler-Wert festlegen,
  CCU42_CC40->PRS = periode;        // Perioden Wert festlegen

  CCU42_CC40->CRS = w;              // Compare-Wert festlegen
  CCU42->GCSS = 1;                  // Shadow transfer request
  PORT3->IOCR0 = 0x98;              // Output-Port PWM Signal festlegen: Alt3 =>OUT bei P3.0
  CCU42_CC40->TCSET = 0x01;         // Zähler starten
}

void init_timer(void) {
  /*
    *   Timer init
    *   CCU40-Timermodul
    */
  CCU40->GIDLC = 0x10F;             // Vorteiler enable, IDLE CCU40 disable
  CCU40_CC40->TC = 0x04;            // Shadow transfer enable on clear
  CCU40_CC40->PSC = 0b0110;         // Vorteiler festlegen
  CCU40_CC40->PRS = 33752;          // Perioden Wert festlegen
  CCU40->GCSS = 1;                  // Shadow transfer request
}
