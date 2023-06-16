// LCD 8 Bit Modus

// Anschluss wie folgt:
// - Versorgungsspannung Display
//   - Display-Pin 1 (VSS) mit Masse
//   - Display-Pin 2 (VDD) mit Versorgungsspannung (5V)
//   - Display-Pin 3 (VEE, Kontrast-Spannung): Spannungsteiler bauen
//       - Widerstand 100k mit VCC verbinden
//       - Andere Seite des Widerstands mit dem Schleifer eines 5k-Trimmerpotis verbinden
//       - Endpunkt des Trimmerpotis mit Masse verbinden
//       - Display-Pin 3 wird dann mit der Verbindungsleitung 1k-Widerstand <-> Schleifer
//         Trimmerpoti verbunden
//
// - Die Display-Steuerleitungen RS, RW und E sind mit PORTD wie folgt zu verbinden:
//   - PD4 = RS
//   - PD5 = RW
//   - PD6 = E
// 
// - Die Datenleitungen des Displays (D0, D1, ... D7) werden mit dem PORTB des Prozessors
//   wie folgt verbunden:
//   - Display-Pin D0 mit Prozessor-Pin PB0
//   - Display-Pin D1 mit Prozessor-Pin PB1
//   - ...
//   - Display-Pin D7 mit Prozessor-Pin PB7
//
// - Pin 15 (LED+) des Displays wird über einen 330 Ohm Widerstand 
//   mit Versorgungsspannung (VCC) verbunden
// - Pin 16 (LED-) des Displays wird mit Masse verbunden


#include <avr/io.h>


#define RS 16
#define RW 32
#define E 64


void lcd_write_command(const unsigned char data)
{
}


unsigned char lcd_busy(void)
{
}


void lcd_write_data(unsigned char data)
{
}


void lcd_write_string(char * s)
{
}



int main(void)
{

	// ***** Initialisierungen *****


	// ***** "Hello world!" durch Aufruf von "lcd_write_string()" ausgeben *****

}
