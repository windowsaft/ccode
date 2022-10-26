//---------------------------------------------------------------------
//
// A. Schilling 8/17:  Neue Funktionsnamen; neue Cursorsteuerung
//
//---------------------------------------------------------------------
extern void lcd_init      ( void );                            // Initialisierung des Displays
extern void lcd_clear     ( void );
extern void lcd_setcursor ( uint8_t zeile, uint8_t spalte );     // Setzen der Cursorposition
                                                               // row (Zeile) = 1,2, ...
                                                               // column (Spalte) = 1,2,3, ...
extern void lcd_print     ( char  text[] );                    // Textausgabe ab Cursorposition, bis '\0'
extern void lcd_char      ( uint8_t  value  );                 // Ausgabe eines Zeichens an der aktuellen Cursorposition
extern void lcd_byte      ( uint8_t  value  );                 // 0 ... 255
extern void lcd_int       ( uint16_t value  );                 // 0 ... 65535


extern void adc_init (void);
extern uint16_t adc_in (uint8_t kanal);
extern void unlock_rtc (void);
extern void ccu4x_enable(void);
