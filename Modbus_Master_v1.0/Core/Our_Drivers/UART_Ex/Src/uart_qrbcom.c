/*
 * uart_qrbcom.c
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#include "uart_qrbcom.h"

RingBuffer_t USART2_INrb;
uint8_t USART2_INRingBuffer[BUFFER_SIZE] = {0};
RingBuffer_t USART2_OUTrb;
uint8_t USART2_OUTRingBuffer[BUFFER_SIZE] = {0};

sUartComm_t uart_comm;

uint8_t print_buffer[PRINT_BUFFER_SIZE] = {0};
va_list arg;

extern UART_HandleTypeDef huart2;
bool UARTx_Cell_Comm_Init(void)
{
	RingBuffer_Init(&USART2_INrb, USART2_INRingBuffer, BUFFER_SIZE);
	RingBuffer_Init(&USART2_OUTrb, USART2_OUTRingBuffer, BUFFER_SIZE);

	uart_comm.huart = &huart2;
	uart_comm.ringbuffer_in = &USART2_INrb;
	uart_comm.ringbuffer_out = &USART2_OUTrb;

	UARTx_Init(&uart_comm);

	__HAL_UART_ENABLE_IT(uart_comm.huart,UART_IT_RXNE);
	return true;
}

int UARTx_Printf(const char *format,...)
{
	int status = 0;
	va_start(arg,format);

	status = vsnprintf((char *)print_buffer,PRINT_BUFFER_SIZE,format,arg);
	va_end(arg);

	UARTx_SendString(&uart_comm, print_buffer);

	return status;
}
