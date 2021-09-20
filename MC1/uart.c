/* File Comment
 * ---------------------------------------------------------------------------------------------------
 * [FILE NAME]: <uart.c>
 *
 * [AUTHOR]: Abdallah Hossam-Eldin Hosny
 *
 * [DATE CREATED]: <15 - 12 - 2020>
 *
 * [DESCRIPTION]: <Source file for UART driver>
 * ---------------------------------------------------------------------------------------------------
 */
#include "uart.h"

#define BAUD_PRESCALE(UART_BAUDRATE) (((F_CPU / (UART_BAUDRATE * 8UL))) - 1)

static volatile void (*g_callBackRX)(void) = NULL_PTR;

ISR(USART_RXC_vect){
	if(g_callBackRX != NULL_PTR){
		g_callBackRX();
	}
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	UART_init
 *
 * [DESCRIPTION]:	Function to initialize the UART
 * 					by set:
 * 					1.	RX enable.
 * 					2.	receiver enable.
 * 					3.	transmitter enable.
 * 					4.	parity.
 * 					5.	stop bit.
 * 					6.	baud rate.
 *
 * [Args]:			Pointer of type Config_Ptr.
 *
 * [IN]:			*config_ptr.
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void UART_init(UART_ConfigType *config_Ptr){

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);
	UCSRB = ((config_Ptr -> INTRXEnable) << RXCIE) | ((config_Ptr -> receiverEnable) << RXEN);
	UCSRB |= ((config_Ptr -> transmitterEnable) << TXEN);
	UCSRC = (1 << URSEL);
	UCSRC |= ((config_Ptr -> parity) << UPM0);
	UCSRC |= ((config_Ptr -> stopBitNo) << USBS);
	UCSRC |= (1 << UCSZ0) | (1 << UCSZ1);
	uint16 baudPrecale = (uint16)(BAUD_PRESCALE(config_Ptr -> UART_baudRate));
	UBRRH = baudPrecale >> 8;
	UBRRL = baudPrecale;
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	UART_sendByte
 *
 * [DESCRIPTION]:	Function to send a byte.
 *
 * [Args]:			data.
 *
 * [IN]:			data.
 * 					contains the data you want to send.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void UART_sendByte(const uint8 data){
	/*
	 * check if the UDR is ready to receive new data.
	 * if it`s ready pass the data.
	 * if no wait until it is ready.
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR = data;
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	UART_receiveByte
 *
 * [DESCRIPTION]:	Function to receive a byte.
 *
 * [Args]:			None.
 *
 * [IN]:			None.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		The data we read.
 * ---------------------------------------------------------------------------------------------------
 */
uint8 UART_receiveByte(void){
	/*
	 * check if it has been receive whole data
	 * if yes get the data back
	 * if no wait until the data be ready;
	 */
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	UART_sendString
 *
 * [DESCRIPTION]:	Function to send string
 *
 * [Args]:			string you want to send
 *
 * [IN]:			Pointer to uint8
 * 					containing the address of the fist byte of the string.
 *
 * [OUT]:			None
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void UART_sendString(const uint8 *str_Ptr){
	for(; *str_Ptr != '\0'; ){
		UART_sendByte(*str_Ptr);
		str_Ptr++;
	}
	UART_sendByte(END_CHARACTER);
}

/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]: UART_receiveString
 *
 * [DESCRIPTION]:	Function to receive string
 *
 * [Args]:			Pointer to receive the string
 *
 * [IN]:			None
 *
 * [OUT]:			* str_Ptr
 *
 * [IN / OUT]:		None
 *
 * [RETURNS]:		None
 * ---------------------------------------------------------------------------------------------------
 */
void UART_receiveString(uint8* str_Ptr){
	do{
		*str_Ptr = UART_receiveByte();
		str_Ptr++;
	}while(*(str_Ptr - 1) != END_CHARACTER);
	*(str_Ptr - 1) = '\0';
}


/* Function Comment
 * ---------------------------------------------------------------------------------------------------
 * [FUNCTION NAME]:	UART_setCallBack
 *
 * [DESCRIPTION]:	Function to set call back in case we use it with interrupt
 *
 * [Args]:			Pinter has the address of the function we will call
 *
 * [IN]:			a_Ptr Pointer to a function.
 *
 * [OUT]:			None.
 *
 * [IN / OUT]:		None.
 *
 * [RETURNS]:		None.
 * ---------------------------------------------------------------------------------------------------
 */
void UART_setCallBack(void(*a_Ptr)(void)){
	g_callBackRX = a_Ptr;
}
