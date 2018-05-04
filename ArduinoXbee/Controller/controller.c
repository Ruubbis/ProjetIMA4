#include <avr/io.h>		// for the input/output registers
#include <avr/interrupt.h>
#include <util/delay.h>

#define CPU_FREQ 16000000L // Assume a CPU frequency of 16Mhz
#define DATA_MAX 5

unsigned char msg_A_ON[DATA_MAX]= {0x5B,0x41,0x2D,0x31,0x5D};
unsigned char msg_B_ON[DATA_MAX]= {0x5B,0x42,0x2D,0x31,0x5D};
unsigned char msg_A_OFF[DATA_MAX]= {0x5B,0x41,0x2D,0x30,0x5D};
unsigned char msg_B_OFF[DATA_MAX]= {0x5B,0x42,0x2D,0x30,0x5D};

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
		if(s[i]!=0){ send_char(s[i]);}
		else {return;}
	}
}

int input_get(void){
	int value;
	value = (((PINB&0x01)!=0)?1:0) + (((PINB&0x02)!=0)?2:0) + (((PINB&0x04)!=0)?4:0);
	return value;
}


int main(){
	DDRB |= 0x10;
	PORTB = 0x10;
	init_serial(9600);
	for(;;){
		send_string(msg_A_ON);
		_delay_ms(1000);
	}
	return 0;
}
