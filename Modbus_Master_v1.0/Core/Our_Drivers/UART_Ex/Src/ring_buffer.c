/*
 * ring_buffer.c
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#include "ring_buffer.h"

bool RingBuffer_Init(RingBuffer_t *pRingBuffer,uint8_t *buffer,size_t capacity)
{
	if((pRingBuffer == NULL) || (buffer == NULL) || (capacity == 0))
	{
		return false;
	}
	pRingBuffer->head = 0;
	pRingBuffer->tail = 0;
	pRingBuffer->capacity = capacity;
	pRingBuffer->buffer = buffer;
	return true;
}

bool RingBuffer_IsEmpty(RingBuffer_t *pRingBuffer)
{
	if(pRingBuffer == NULL)
	{
		return false;
	}
	return (pRingBuffer->head == pRingBuffer->tail);
}

bool RingBuffer_IsFully(RingBuffer_t *pRingBuffer)
{
	if(pRingBuffer == NULL)
	{
		return false;
	}

	int32_t diff = (pRingBuffer->head - pRingBuffer->tail);

	if(diff < 0)
	{
		diff = pRingBuffer->capacity + diff;
	}

	return (diff == (pRingBuffer->capacity - 1));
}

bool RingBuffer_Write(RingBuffer_t *pRingBuffer,uint8_t data)
{
	if(RingBuffer_IsFully(pRingBuffer) == true)
	{
		return false;
	}

	pRingBuffer->buffer[pRingBuffer->head] = data;
	pRingBuffer->head++;
	if(pRingBuffer->head == pRingBuffer->capacity)
	{
		pRingBuffer->head = 0;
	}

	return true;
}

bool RingBuffer_Read(RingBuffer_t *pRingBuffer,uint8_t *address)
{
	if((pRingBuffer == NULL) || (address == NULL))
	{
		return false;
	}

	if(RingBuffer_IsEmpty(pRingBuffer) == true)
	{
		return false;
	}

	*address = pRingBuffer->buffer[pRingBuffer->tail];
	pRingBuffer->tail++;

	if(pRingBuffer->tail == pRingBuffer->capacity)
	{
		pRingBuffer->tail = 0;
	}

	return true;
}

bool RingBuffer_Clear(RingBuffer_t *pRingBuffer)
{
	if(pRingBuffer == NULL)
	{
		return false;
	}
	pRingBuffer->head = 0;
	pRingBuffer->tail = 0;
	return true;
}
