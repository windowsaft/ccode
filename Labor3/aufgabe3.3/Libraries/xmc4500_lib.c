#include <XMC4500.h>		//SFR declarations of the selected device

// ******************************************************************
// LCD-Funktionen fuer XMC4500 Trainingsboard
// R. Birk April 14
// A. Schilling 10/16 (Datentypen)
// A. Schilling 8/17  Neue Funktionsnamen; neue Cursorsteuerung
//
// Ansteuerung LCD-Display seriell (4 * 20 Zeichen LCD)
// Filename:  xmc4500_lib.c
// ******************************************************************
void unlock_rtc (void);
uint16_t adc_in (uint8_t kanal);
void adc_init (void);
void ccu4x_enable(void);
void lcd_init(void);		// Init LCD
void lcd_clear(void);	 	// LCD L�schen
void lcd_byte(uint8_t);  	// Ausgabe unsigned char  => 3 stellig
void lcd_int(uint16_t);		// Ausgabe Int Wert => 5 stellig
void lcd_char(int8_t);	   	// Ausgabe eines ASCII Zeichens
void lcd_print(char *ptr);	// Ausgabe String
void lcd_setcursor (uint8_t row, uint8_t column);	// Cursor setzen
void wait_bsy1(void);  		// kurz warten
void wait_bsy2(void); 		// lang warten
void send_byte(uint8_t,uint8_t);
void wait_0(void);
//****************************************************************
// Anschluss des Displays an:
// clk = P5.2  Clock Anschluss LCD
// sod = p5.1
// sid = P5.0	// seriell Input LCD
//****************************************************************
// Sende ein Byte zum Display, senddate ist Bytewert
// rs => Command = 0 und Data = 1
void send_byte(uint8_t senddata, uint8_t rs)
{
	uint8_t i;
	uint32_t sendwert, zw;
	zw = (uint32_t)senddata;   // Typconvertierung
	sendwert = 0x001f + ((zw & 0xf0)<<12) + ((zw&0x0f)<<8);
	if (rs == 1) sendwert = sendwert + 0x40;
	for (i = 0; i<24;i++)	// serielle Ausgabe von 24Bit
	{  	CLR_BIT (PORT5->OUT,2)  ; // clk = 0;
		if ((sendwert & 0x00000001) == 0)  CLR_BIT(PORT5->OUT,0);  //sid = 0;
		else SET_BIT(PORT5->OUT,0);
		wait_0();
		SET_BIT ( PORT5->OUT, 2);	//sclk = 1;
		wait_0();
		sendwert = sendwert >> 1;
	}
}
void wait_0(void)	// ca. 50 us warten
{
	uint16_t zaehler ;
	for (zaehler = 0; zaehler < 0xf; zaehler++);  // ca. 1 uSek Verz�gerung
}
void wait_bsy1(void)	// ca. 50 us warten
{
	uint16_t zaehler ;
	for (zaehler = 0; zaehler < 0x7ff; zaehler++);
}
void wait_bsy2(void)	  // ca. 1.6 ms warten
{
	uint16_t zaehler ;
	for (zaehler = 0; zaehler < 0x7fff; zaehler++);
}
void lcd_init(void)
{ 	// P5.0 => OUT
	PORT5->IOCR0 = PORT5->IOCR0 & 0xff000000; 	// P5.0, P5.1, P5.2
	PORT5->IOCR0 = PORT5->IOCR0 | 0x00800080;	// P5.0, P5.2 => OUT

	wait_bsy2();
	wait_bsy2();
	// LCD Init
	send_byte(0x34,0); 			// 8 Bit Mode
	wait_bsy2();
	send_byte(0x09,0); 			// 4 Zeilen Mode
	wait_bsy2();
	send_byte(0x30,0); 			// 8 Bit Datenl�ge
	wait_bsy2();
	send_byte(0x0f,0); 			// display ein , Cursor ein
	wait_bsy2();
	send_byte(0x01,0); 			// clear Display
	wait_bsy2();
	send_byte(0x07,0); 			// Cursor autoincrement
	wait_bsy2();
}
void lcd_clear(void)
{   send_byte( 01 ,0);  // CLR Befehl
	wait_bsy2();
	send_byte( 0x0e ,0); // Cursor on
	wait_bsy1();
}
void lcd_char(int8_t zeichen)
{
	send_byte(zeichen,1);
	wait_bsy1();
	wait_bsy1();
}
void lcd_print(char *ptr)
{
	uint8_t i=0;
	while (*ptr != 0 )
	{
		lcd_char(*ptr);
	  	ptr++;
	  	i++;
	  	if (i == 20) lcd_setcursor(2,1);
	  	if (i == 40) lcd_setcursor(3,1);
	  	if (i == 60) lcd_setcursor(4,1);
	}
}
void lcd_byte(uint8_t wert)
{
	uint8_t i;

	i = wert / 100;
	wert %= 100;
	i += 0x30;
	lcd_char(i);
	i = wert / 10;
	wert %= 10 ;
	i += 0x30;
	lcd_char(i);
	wert += 0x30;
	lcd_char(wert);
}
void lcd_int(uint16_t wert_16)
{	// Ausgabe von 0 bis 99999 !!!!
	uint8_t i;

	i = wert_16 / 10000; 	// Anzahl 1000er
	wert_16 %= 10000;			// Rest nach 1000er Division
	lcd_char(i+0x30);   	 	// Ausgabe als ASCII Zeichen
	i = wert_16 / 1000; 	// Anzahl 1000er
	wert_16 %= 1000;			// Rest nach 1000er Division
	lcd_char(i+0x30);    	// Ausgabe als ASCII Zeichen
	i= wert_16 / 100;
	wert_16 %= 100;
	lcd_char(i+0x30);
	i = wert_16 / 10;
	wert_16 %= 10;
	lcd_char(i+0x30);
	wert_16 += 0x30;
	lcd_char((char)wert_16);
}
void lcd_setcursor( uint8_t zeile, uint8_t spalte )
{
  uint8_t position;
  switch(zeile)
  {
    case 1:  position = 0x00; break;
    case 2:  position = 0x20; break;
    case 3:  position = 0x40; break;
    case 4:  position = 0x60; break;
    default: position = 0x00; break;
  }

  position += (spalte-1);
  send_byte(position | 0x80, 0);
  wait_bsy1();
  wait_bsy1();
}
//----------------------------------------------------------------------------
//Initialisierung ADC:

void adc_init (void)
{
	 SCU_RESET->PRCLR0 	|= 0x01 ;  //Reset ADC
	 VADC->CLC       		=   0;  	//enable the VADC module clock
	 VADC->GLOBCFG   		= 0x8000830e;   //  =>set DIVA to E , WR Enable
	 VADC_G0->ARBCFG 		= 0x00000003 ; // ANONS normal operation
	 VADC_G0->ARBPR  = (1UL<< 26)|(1UL<< 8);//en arbitration slot 02,priority set to 1
	 VADC_G0->RCR[0]	|= (1UL << 24); // Wait for Read Mode
	 VADC_G0->RCR[0] |=  (1UL << 20); 	// DMM = 01
	 VADC_G0->RCR[0] |=  (0x9UL << 16); 	// FIR Filter
	 //VADC->BRSMR    |=  (1UL <<  4);  // Autoscan
	 VADC_G0->ICLASS[0] |= 0x0000010f;
	 VADC->GLOBICLASS[0] |=0x0000010f;
	 // BACKGROUND SOURCE
	 VADC->BRSMR     = 0x01;         /* ENGT = 01B     Conv. Request m�glich     */
}
//**********************************************************************************
uint16_t adc_in (uint8_t kanal)
{
#define anzahl 2
uint16_t adwert;
		
		
	//VADC->BRSMR    |=  (1UL <<  4);  // Autoscan
	VADC->BRSSEL[0] =  0 |(1UL << kanal);	//Input channel
	VADC->BRSCTRL		= ((1UL <<23)|(1UL<<15));
	VADC->BRSMR    |= 	0x01;	/* ENGT = 01B  Conv. Request m�glich     */
	VADC->BRSMR    |=  (1UL <<  9);            /* generate Load event           */
	while (!(VADC_G0->RES[0] & ( 1UL << 31))){};   /* Wait for Conversion end     */
    adwert = (VADC_G0->RES[0] & 0x0fff); /* Store converted value  ,12 Bit       */
	//VADC->BRSMR   &= ~(0x00000013);	// ENGT = 0; Scan = 0;
		
	return(adwert);
}
//************************************************************************************
void unlock_rtc (void)
{
	SCU_POWER->PWRSET |= 0x01;		// Power set HIB = 1
	SCU_RESET->RSTCLR |= 1 <<9 ;   	// Ruecksetzen "Schlafmodus"
	// Zugriff auf SCU Register erst nach Abfrage Status :
	while ((SCU_GENERAL->MIRRSTS ) & 0x20) ;	// OSC Control Busy ?
	SCU_HIBERNATE ->OSCSICTRL = 0x00;			// Osc einschalten
	while ((SCU_GENERAL->MIRRSTS ) & 0x40) ;	// OSC Control Busy ?
	SCU_HIBERNATE ->OSCULCTRL |= 0x01;		// OSC in Operation
	while ((SCU_GENERAL->MIRRSTS ) & 0x80) ;	// RTC Control Busy ?
	RTC->CTR |= 0x1;	// Enable RTC
}
//****************************************************************************
void ccu4x_enable(void)
{ //System control unit
  SCU_RESET->PRSET0 |= 0x1c;	// Reset CCU40,CCU41,CCU42 =>				
  SCU_RESET->PRCLR0 |= 0x1c; 	// re assert RESET
  SCU_CLK->CLKSET   |= 0x10; 	// CCU-CLK enable
  SCU_GENERAL->CCUCON |= 0x0f;	//CCU42_CC40 global enable
}
//****************************************************************************




