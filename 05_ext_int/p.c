#include <avr/io.h>
#include <avr/interrupt.h>


#define SET_LED_ON (PORTD |= 1)
#define SET_LED_OFF (PORTD &= ~1)
#define TOGGLE_LED (PORTD ^= 1)


ISR(INT0_vect)
{

}


int main(void)
{










}