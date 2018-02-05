#include <avr/io.h>		// for the input/output registers
#include <avr/interrupt.h>
#include <util/delay.h>

#define CPU_FREQ 16000000L // Assume a CPU frequency of 16Mhz
#define DATA_MAX 3

int in_progress = 0;
int received = 0;
int success = 0;


void init_serial(int speed){
	cli();
	UBRR0 = CPU_FREQ/(((unsigned long int)speed)<<4)-1;
	UCSR0B = (1<<TXEN0 | 1<<RXEN0);
	UCSR0C = (1<<UCSZ01 | 1<<UCSZ00);
	UCSR0A &= ~(1 << U2X0);
	sei(); 
}

void send_char(unsigned char c){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

void send_string(unsigned char s[DATA_MAX]){
	int i;
	for(i=0;i<DATA_MAX;i++){
		send_char(s[i]);
	}
}

unsigned char get_char(void){
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}


int main(){
	unsigned char c;
	unsigned char data[DATA_MAX];
	init_serial(9600);
	DDRB |= 0x20;
	for(;;){
		while(success == 0){
			c = get_char();
			if(c == '['){
				in_progress = 1;
			}
			else if(c != ']' && in_progress == 1){
				data[received] = c;
				received++;
			}
			else if(c == ']' && in_progress == 1){
				in_progress = 0;
				success = 1;
				received = 0;
			}
		}
		if (data[0] == 0x41){
			if(data[2] == 0x31){PORTB ^= 0x20;}
			if(data[2] == 0x30){PORTB ^= 0x20;}
		}
		success = 0;
	}
	return 0;
}
