/*
 * usart_ex.h
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#ifndef OUR_DRIVERS_UART_EX_INC_USART_EX_H_
#define OUR_DRIVERS_UART_EX_INC_USART_EX_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define UART_TX_EMPTY       (1 << 7)
#define UART_TX_INT         (1 << 7)

#include "ring_buffer.h"
#include "stm32f4xx.h"

typedef struct
{
	UART_HandleTypeDef *huart;
	RingBuffer_t *ringbuffer_in;
	RingBuffer_t *ringbuffer_out;
}sUartComm_t;

/**
 * Initializes the UART communication with given ring buffers.
 * @param uart Pointer to the UART communication structure.
 * @return true if initialization is successful, false otherwise.
 */
bool UARTx_Init(sUartComm_t *uart);

/**
 * Sends a single character through UART.
 * @param uart Pointer to the UART communication structure.
 * @param data Data byte to be sent.
 * @return true if the character is successfully sent, false otherwise.
 */
bool UARTx_SendChar(sUartComm_t *uart, uint8_t data);
/**
 * Sends a string of characters through UART.
 * @param uart Pointer to the UART communication structure.
 * @param string Null-terminated string to be sent.
 * @return true if the string is successfully sent, false otherwise.
 */
bool UARTx_SendString(sUartComm_t *uart, const uint8_t *string);

#endif /* OUR_DRIVERS_UART_EX_INC_USART_EX_H_ */
