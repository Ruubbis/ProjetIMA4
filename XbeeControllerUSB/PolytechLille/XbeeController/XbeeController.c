#include "XbeeController.h"


uint8_t data_send;
uint8_t state_send = 1;


uint8_t msg_A_ON[DATA_MAX]= {0x5B,0x41,0x2D,0x31,0x5D};
uint8_t msg_B_ON[DATA_MAX]= {0x5B,0x42,0x2D,0x31,0x5D};
uint8_t msg_A_OFF[DATA_MAX]= {0x5B,0x41,0x2D,0x30,0x5D};
uint8_t msg_B_OFF[DATA_MAX]= {0x5B,0x42,0x2D,0x30,0x5D};


ISR(USART1_RX_vect)
{
	data_send = UDR1;
	state_send = 1;
}

void Send_Msg_Serial(uint8_t msg[DATA_MAX]){
	int i;
	for(i=0;i<DATA_MAX;i++){
		Serial_SendByte(msg[i]);	
	}
}

void SetupHardware(void)
{
	Serial_Init(9600,false);
	UCSR1B |= (1 << RXCIE1);
       	// Enable the USART Receive Complete interrupt (USART_RXC)
	DDRD |= 0x30;
	USB_Init();
}

void SendNextReport(void)
{
	if(state_send)
	{
		Endpoint_SelectEndpoint(XC_IN_EPADDR);

		if (Endpoint_IsReadWriteAllowed())
		{
			Endpoint_Write_Stream_LE(&data_send, sizeof(uint8_t), NULL);
			Endpoint_ClearIN();
		}	
		
		state_send = 0;
	}	
}


void ReceiveNextReport(void)
{
	Endpoint_SelectEndpoint(XC_OUT_EPADDR);
	if (Endpoint_IsOUTReceived()){
		if (Endpoint_IsReadWriteAllowed()){
			uint8_t LEDReport = Endpoint_Read_8();
			switch(LEDReport){
				case 0x41:
					Send_Msg_Serial(msg_A_ON);
					break;
				case 0x42:
					Send_Msg_Serial(msg_A_OFF);
					break;
				case 0x43:
					Send_Msg_Serial(msg_B_ON);
					break;
				case 0x44:
					Send_Msg_Serial(msg_B_OFF);
					break;
			}
			Endpoint_ClearOUT();
	
		}
	}
}

	
void PAD_Task(void)
{
	
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;
	
	SendNextReport();
	ReceiveNextReport();
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	Endpoint_ConfigureEndpoint(XC_IN_EPADDR, EP_TYPE_INTERRUPT, XC_EPSIZE, 1);
	Endpoint_ConfigureEndpoint(XC_OUT_EPADDR, EP_TYPE_INTERRUPT, XC_EPSIZE, 1);
	USB_Device_EnableSOFEvents();
}

int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();

	for (;;){
		USB_USBTask();
		PAD_Task();
	}	
}

