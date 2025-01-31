/*
 * ring_buffer.h
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#ifndef OUR_DRIVERS_UART_EX_INC_RING_BUFFER_H_
#define OUR_DRIVERS_UART_EX_INC_RING_BUFFER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t *buffer;           // Buffer for storing data
    size_t capacity;           // Maximum capacity of the buffer
    volatile size_t head;      // Pointer to the head of the buffer (write position)
    volatile size_t tail;      // Pointer to the tail of the buffer (read position)
} RingBuffer_t;

bool RingBuffer_Init(RingBuffer_t *pRingBuffer,uint8_t *buffer,size_t capacity);
bool RingBuffer_IsEmpty(RingBuffer_t *pRingBuffer);
bool RingBuffer_IsFully(RingBuffer_t *pRingBuffer);
bool RingBuffer_Write(RingBuffer_t *pRingBuffer,uint8_t data);
bool RingBuffer_Read(RingBuffer_t *pRingBuffer,uint8_t *address);
bool RingBuffer_Clear(RingBuffer_t *pRingBuffer);
#endif /* OUR_DRIVERS_UART_EX_INC_RING_BUFFER_H_ */
