// LCD im 4 Bit Modus ansprechen


#include <avr/io.h>


// Taktrate Prozessor: 20 MHz => 1 Takt (1 cycle) dauert 50 ns


// Register Select: 0: Instruction Register; 1: Data Register
#define RS 1

// Read/Write
#define RW 2

// Enable
#define E 4


// rs: 1 if data, 0 if instruction 
// rw: 1 if read operation, 0 if write operation
// send_data: the data to be sent
// return value: the value read from the display
unsigned char lcd(unsigned char rs, unsigned char rw, unsigned  char send_data, unsigned char eight_bit_mode)
{
	unsigned char read_data = 0;

	PORTB = 0x00;


	// make port B become output (2 cycles)
	DDRB = 0xF7;



	// if it is a read operation, set RW
	if(rw) PORTB |= RW;

	// if the transmission is about data and not 
	// an instruction operation, set RS
	if(rs) PORTB |= RS;


	// wait for at least tAS = 140 ns
	// 3 cycles => 150 ns
	asm volatile(
		"nop\n"
		"nop\n"
		"nop\n"
		);

	// ***** write upper four bits *****

	// rise E (1 cycle)
	PORTB |= E;


	// if it is a read operation (2 cycles)
	if(rw)	
	{
		DDRB = 0x07;

		// fulfill tDDR = 320 cycles
		// wait for another 320 - 100 = 220 ns => 5 cycles
		asm volatile(
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			);

		// read upper four data bits (2 cycles)
		read_data = (PINB & 0xF0);


		// PWEH = 450 ns; add an additional cycle for safety
		asm volatile(
			"nop\n"
			);

		DDRB = 0xF7;
	}

	// if it is a write operation (3 cycles from PORTB | E)
	else
	{
		// delete the upper four bits (3 cycles)
		PORTB &= 0x0F;

		// write the upper four bits (4 cycles)
		PORTB |= (send_data & 0xF0);

		// fulfill tDSW = 195 ns => 5 cycles = 250 ns
		asm volatile(
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			);

		// PWEH = 450 ns (9 cycles) fulfilled automatically
	}

	
	// falling edge of E
	PORTB &= (~E);


	// wait for at least PWEH = 450 ns + max(t_H, t_DHR) = 20 ns => 470 ns
	// 10 cylces => 500 ns
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

	if(eight_bit_mode) return 0;


	// ***** lower four bits *****

	// rise E (1 cycle)
	PORTB |= E;


	// if it is a read operation (2 cycles)
	if(rw)	
	{

		DDRB = 0x07;

		// fulfill tDDR = 320 cycles
		// wait for another 320 - 100 = 220 ns => 5 cycles
		asm volatile(
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			);

		// read lower four data bits (4 cycles)
		read_data |= (((PINB & 0xF0) >> 4) & 0x0F);

		DDRB = 0xF7;


		// PWEH = 450 ns; no additional cycles needed
	}

	// if it is a write operation (3 cycles from PORTB | E)
	else
	{
		// delete the upper four bits (3 cycles)
		PORTB &= 0x0F;

		// write the lower four bits (4 cycles)
		PORTB |= (((send_data & 0x0F) << 4) & 0xF0);

		// fulfill tDSW = 195 ns => 5 cycles = 250 ns
		asm volatile(
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			);

		// PWEH = 450 ns (9 cycles) fulfilled automatically
	}

	
	// falling edge of E
	PORTB &= (~E);


	// wait for at least PWEH = 450 ns + max(t_H, t_DHR) = 20 ns => 470 ns
	// 10 cylces => 500 ns
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


	return read_data;

}


void lcd_write_command_eight_bit_mode(const unsigned char data)
{
	lcd(0, 0, data, 1);
}

void lcd_write_command(const unsigned char data)
{
	lcd(0, 0, data, 0);
}

void lcd_write_data(const unsigned char data)
{
	lcd(1, 0, data, 0);
}


unsigned char lcd_busy_eight_bit_mode(void)
{
	return ((lcd(1, 1, 0, 1) >> 7) & 1);
}


unsigned char lcd_busy(void)
{
	return ((lcd(1, 1, 0, 0) >> 7) & 1);
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


int main(void)
{

	// LED output
	DDRD=0x01;
	PORTD = 1;
	


	// *** starting in 8 bit mode ***
	while(lcd_busy_eight_bit_mode());
	lcd_write_command_eight_bit_mode(0x28);		// function set: set 4 bit mode (and 2 lines, 5x7) 
	while(lcd_busy());


	// *** now in four bit mode ***

	lcd_write_command(0x28);		// function set: set 4 bit mode, 2 lines, 5x7
	while(lcd_busy());

	lcd_write_command(0x0C);		// turn display on, cursor off, cursor blink off
	while(lcd_busy());

	lcd_write_command(0x01);		// clear display, set cursor on position (1,1)
	while(lcd_busy());

	lcd_write_command(0x06);		// cursor auto increment
	while(lcd_busy());
	

	lcd_write_command(0x80);		// set DD RAM address to 0x00
	while(lcd_busy());
	

	lcd_write_string("Hello, world!");


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