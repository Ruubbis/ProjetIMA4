#include <avr/io.h>		// for the input/output registers
#include <avr/interrupt.h>
#include <util/delay.h>

#define CPU_FREQ        16000000L       // Assume a CPU frequency of 16Mhz

void init_serial(int speed){
	cli();
	UBRR0 = CPU_FREQ/(((unsigned long int)speed)<<4)-1;
	UCSR0B = (1<<TXEN0 | 1<<RXEN0);
	UCSR0C = (1<<UCSZ01 | 1<<UCSZ00);
	UCSR0A &= ~(1 << U2X0);
	sei();	
}


void send_serial(unsigned char c){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}


int input_get(void){
	int value;
	value = (((PINB&0x01)!=0)?1:0) + (((PINB&0x02)!=0)?2:0) + (((PINB&0x04)!=0)?4:0);
	return value;
}
int main(void){
	DDRB |= 0x10;
	init_serial(9600);
	PORTB |= 0x10;
	int value = 0;
	for(;;){
		value = input_get();
		send_serial(0x30+value);
		send_serial('\n');
		_delay_ms(200);
	}
}
