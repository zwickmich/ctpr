// LCD 8 Bit Modus


#include <avr/io.h>
#include <avr/interrupt.h>


//-->

#define RS 16
#define RW 32
#define E 64


void lcd_write_command(const unsigned char data)
{
	// make port B become output (1 cylce)
	DDRB = 0xFF;

	// clear RS and RW (3 cycles)
	PORTD &= (~ (RS | RW));

	// read data (1 cylce)
	PORTB = data;

	// 20 MHz => 1 clk = 50 ns
	// wait 140 ns => 3 clk
	asm volatile(
			"nop\n"
			"nop\n"
			"nop\n");
	
	// rise E
	PORTD |= E;

	// wait at least 450 ns => 10 clk = 500 ns
	asm volatile(
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		);

	// falling edge on E
	PORTD &= ~ E;

}


unsigned char lcd_busy(void)
{
	unsigned char data;

	// make port B become an input (1 cycle)
	DDRB = 0x00;

	// clear RS (2 cylces)
	PORTD &= ~RS;

	// set RW (2 cycles)
	PORTD |= RW;

	// wait at least 140 ns, i.e. 3 clks => 150 ns
	asm volatile(
		"nop\n"
		"nop\n"
		"nop\n");

	// rise E
	PORTD |= E;

	// wait at least 320 ns until data is valid => 7 clk = 350 ns
	asm volatile(
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		);

	// read data (2 cycles)
	data = PINB;

	// wait to have a high level of at least 500 ns
	asm volatile(
		"nop\n"
		);		

	// falling edge on E (2 cylces)
	PORTD &= ~E;	

	// extract and return busy flag (4 cycles)
	return data >> 7;

}


void lcd_write_data(unsigned char data)
{
	// make port B become an output (1 cycle)
	DDRB = 0xFF;

	// set RS (2 cycles)
	PORTD |= RS;

	// clear RW (2 cycles)
	PORTD &= ~ RW;

	// wait at least 140 ns, i.e. 3 clks => 150 ns
	asm volatile(
		"nop\n"
		"nop\n"
		"nop\n");

	// send data to the lcd
	PORTB = data;

	// wait 140 ns => 3 clk = 150 ns
	asm volatile(
			"nop\n"
			"nop\n"
			"nop\n");
	
	// rise E
	PORTD |= E;

	// wait at least 450 ns => 10 clk = 500 ns
	asm volatile(
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		);

	// falling edge on E
	PORTD &= ~ E;


}


void lcd_write_string(char * s)
{
	unsigned char i;

	for(i = 0; s[i]; i++)
	{
		while(lcd_busy());
		lcd_write_data(s[i]);
	}
}




ISR(PCINT2_vect)
{

	// only check rising edge
	if(PINC & 1)
	{
		asm volatile(
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
		);


		if(PINC & 2)
			PORTD = 1;
		else
			PORTD = 0;

	}


	// static char pinc0 = -1;
	// // PORTD ^= 1;

	// // erstes Mal
	// if(pinc0 == -1)
	// {
	// 	// aktuellen Zustand für nächsten Durchlauf abspeichern
	// 	pinc0 = PINC & 1;

	// 	return;
	// }

	// // rising edge on PINC0
	// if(PINC & 1 && !pinc0)
	// {
	// 	if(PINC & 2)
	// 		PORTD = 1;
	// }

	// // falling edge on PINC0
	// else if(!(PINC & 1) && pinc0)
	// {

	// 		PORTD = 0;

	// }

	// // aktuellen Zustand für nächsten Durchlauf abspeichern
	// pinc0 = PINC & 1;

}


int main(void)
{

	SREG = 0x80;				// globally enable interrupts
	PCICR = 4;					// enable interrupt in pin change interrupt control register
	PCMSK2 = 1;					// enable PCINT16


// EICRA=((1 << ISC21) | (1 << ISC20)); // set sense bits for rising edge
// EIMSK=(1 << INT2); // set intrupt #2 enable mask bits
// PCICR=(1 << PCIE2); // set intrupt #2 pin change bits
// PCMSK2=(1 << PCINT16); // set port k/pin 0 change mask bit


	DDRD=1;
	
	PORTD = 1;



	while(lcd_busy());
	lcd_write_command(0x38);		// function set: 8 bit mode, 2 lines, 5x7
	while(lcd_busy());
	lcd_write_command(0x0C);		// turn display on, cursor on/off, blink on/off, cursor position on/off
	while(lcd_busy());
	lcd_write_command(0x01);		// clear display, set cursor on position (1,1)
	while(lcd_busy());
	lcd_write_command(0x06);		// cursor auto increment

	while(lcd_busy());
	lcd_write_command(0x80);

	// while(lcd_busy());
	// lcd_write_data('A');


	lcd_write_string("Hello world!");

	while(1)
	{
		asm volatile(
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
		);
	}

}

//<--