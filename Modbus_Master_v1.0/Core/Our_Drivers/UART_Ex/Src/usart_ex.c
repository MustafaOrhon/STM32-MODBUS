/*
 * usart_ex.c
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#include "usart_ex.h"

RingBuffer_t *usart2_in;
RingBuffer_t *usart2_out;

extern UART_HandleTypeDef huart2;

bool UARTx_Init(sUartComm_t *uart)
{
	if(uart == NULL)
	{
		return false;
	}

	if(uart->huart != &huart2)
	{
		return false;
	}

	usart2_in = uart->ringbuffer_in;
	usart2_out = uart->ringbuffer_out;
	return true;
}

bool UARTx_SendChar(sUartComm_t *uart,uint8_t data)
{
	if(uart == NULL)
	{
		return false;
	}

	if(RingBuffer_IsFully(uart->ringbuffer_out))
	{
		if((uart->huart->Instance->CR1 & UART_TX_INT) == false)
		{
			uint8_t ch = 0;
			RingBuffer_Read(uart->ringbuffer_out,&ch);
			HAL_UART_Transmit_IT(uart->huart, &ch, 1);
		}
		while(RingBuffer_IsFully(uart->ringbuffer_out));
	}

	if(RingBuffer_Write(uart->ringbuffer_out, data) == false)
	{
		return false;
	}
	return true;
}



bool UARTx_SendString(sUartComm_t *uart, const uint8_t *string)
{
	if((uart == NULL) || (string == NULL))
	{
		return false;
	}

	const uint8_t *ptr = string;

    while (*ptr)
    {
        if (UARTx_SendChar(uart, *ptr) == false)
        {
            return false;
        }
        ptr++;
    }

    if ((uart->huart->Instance->CR1 & UART_TX_INT) == false)
    {
        uint8_t ch = 0;
        bool status = RingBuffer_Read(uart->ringbuffer_out, &ch);
        if (status == true)
        {
            uart->huart->Instance->DR = ch;
            __HAL_UART_ENABLE_IT(uart->huart, UART_IT_TXE);
        }
    }

	return true;
}
