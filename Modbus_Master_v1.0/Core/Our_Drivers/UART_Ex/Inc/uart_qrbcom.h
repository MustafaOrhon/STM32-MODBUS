/*
 * uart_qrbcom.h
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#ifndef OUR_DRIVERS_UART_EX_INC_UART_QRBCOM_H_
#define OUR_DRIVERS_UART_EX_INC_UART_QRBCOM_H_

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "usart_ex.h"



#define BUFFER_SIZE       1024
#define PRINT_BUFFER_SIZE 1024

bool UARTx_Cell_Comm_Init(void);
int UARTx_Printf(const char *format,...);

#endif /* OUR_DRIVERS_UART_EX_INC_UART_QRBCOM_H_ */
