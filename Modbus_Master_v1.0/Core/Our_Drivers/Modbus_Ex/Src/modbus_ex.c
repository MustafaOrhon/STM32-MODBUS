/*
 * modbus_ex.c
 *
 *  Created on: Jan 25, 2025
 *      Author: Mustafa
 */
#include <string.h>
#include <math.h>
#include "main.h"
#include "modbus_ex.h"

#define MSB(x)    ((uint8_t)(x >> 8))
#define LSB(x)	  ((uint8_t)(x & 0xFF))
#define BIT_READ(x, y)  ((uint8_t)(((x) >> (y)) & 0x01))

extern RingBuffer_t USART2_INrb;
extern UART_HandleTypeDef huart2;

uint8_t output_10thcoil = 0;
uint8_t input1,input2,input3,input4,input5;
uint16_t analog_value[10] = {0};

sTickTime_t idle_time;
sTickTime_t wait_response_time;
sTickTime_t error_time;

static bool are_elements_equal(uint8_t *tx_array,uint8_t *rx_array,uint8_t num_of_elements);

/*
 * Modbus Master Control function
 * */
bool Modbus_Master_Control(sModbus_t *modbus)
{
	if (modbus == NULL)
	{
		return false;
	}

	if (modbus->mode < eModbusMode_First || modbus->mode >= eModbusMode_Last)
	{
		return false;
	}

	if (modbus->state < eModbusState_First || modbus->state >= eModbusState_Last)
	{
		return false;
	}

	switch (modbus->state)
	{
	case eModbusState_Idle:  // Idle state : get ready to send a message
	{
		if (modbus->tx_message.new_send_message_status == true && TickTimer_CheckElapsedTime(&idle_time))
		{
			modbus->tx_message.new_send_message_status = false;
			modbus->tx_message.tx_index = 0;
			TickTimer_Reset(&idle_time);
			modbus->state = eModbusState_SendRequst;
		}
		break;
	}
	case eModbusState_SendRequst: // Send a request to slave
	{
		/* Fill the ADU part*/
		modbus->tx_message.tx_data[modbus->tx_message.tx_index] = modbus->tx_message.slave_address;
		modbus->tx_message.tx_index++;

		modbus->tx_message.tx_data[modbus->tx_message.tx_index] = modbus->tx_message.function_code;
		modbus->tx_message.tx_index++;

		switch (modbus->tx_message.function_code)
		{
		case eModbusFunctionCode_ReadCoilStatus:
		{

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			break;
		}
		case eModbusFunctionCode_ReadInputStatus:
		{
			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;
			break;
		}
		case eModbusFunctionCode_ReadHoldingRegisters:
		{
			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;
			break;
		}
		case eModbusFunctionCode_ReadInputRegisters:
		{
			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;
			break;
		}
		case eModbusFunctionCode_WriteSingleCoil:
		{
			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.write_data[0]);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.write_data[0]);
			modbus->tx_message.tx_index++;
			break;
		}
		case eModbusFunctionCode_WriteSingleRegister:
		{
			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.write_data[0]);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.write_data[0]);
			modbus->tx_message.tx_index++;

			break;
		}
		case eModbusFunctionCode_WriteMultipleCoils:
		{
			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.write_data[0]); // Number of bytes
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.write_data[1]); // Coil Status
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.write_data[1]); // Coil Status
			modbus->tx_message.tx_index++;
			break;
		}
		case eModbusFunctionCode_WriteMultipleRegisters:
		{
			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.start_address);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.num_of_registers);
			modbus->tx_message.tx_index++;

			modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.write_data[0]); // Number of bytes
			modbus->tx_message.tx_index++;

			for(int i = 1; i <= modbus->tx_message.num_of_registers; i++)
			{
				modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.write_data[i]); // Coil Status
				modbus->tx_message.tx_index++;

				modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.write_data[i]); // Coil Status
				modbus->tx_message.tx_index++;
			}
			break;
		}
		default:
		{
			break;
		}
		}
		modbus->tx_message.crc16 = 0xFF;
		modbus->tx_message.crc16 = CRC16_Calculate_With_LookUp_Table(modbus->tx_message.tx_data, modbus->tx_message.tx_index);
		modbus->tx_message.tx_data[modbus->tx_message.tx_index] = LSB(modbus->tx_message.crc16);
		modbus->tx_message.tx_index++;
		modbus->tx_message.tx_data[modbus->tx_message.tx_index] = MSB(modbus->tx_message.crc16);
		modbus->tx_message.tx_index++;

		RingBuffer_Clear(&USART2_INrb);

		Modbus_Master_Write(modbus->tx_message.tx_data, modbus->tx_message.tx_index);

		TickTimer_Set(&wait_response_time, 1000);
		modbus->state = eModbusState_WaitingForReply;

		break;
	}
	case eModbusState_WaitingForReply: // Wait for slave to send reply
	{
		if (USART2_INrb.head > 2)
		{
			modbus->rx_message.new_receive_msg_status = true;
		}

		if (modbus->rx_message.new_receive_msg_status == true)
		{

			/* Check the ID of the message to check if its true slave*/
			modbus->rx_message.slave_address = USART2_INrb.buffer[0]; // Slave address
			if (modbus->rx_message.slave_address == modbus->tx_message.slave_address)
			{
				modbus->state = eModbusState_ProccessingReply;
				modbus->rx_message.new_receive_msg_status = false;
			}
			else
			{
				RingBuffer_Clear(&USART2_INrb);
			}
		}

		else
		{
			if (TickTimer_CheckElapsedTime(&wait_response_time) == true)
			{
				modbus->error_code = eModbusErrorCode_Timeout;
				modbus->state = eModbusState_Error;
				TickTimer_Set(&error_time, 1000);
			}
		}

		break;
	}
	case eModbusState_ProccessingReply: // Process the reply if slave respond in time
	{
		/* Take the function code of the message */
		modbus->rx_message.function_code = USART2_INrb.buffer[1];
		/* Check the incoming message function code  and check error code*/
		if ((modbus->rx_message.function_code & 0x7F) != (modbus->tx_message.function_code))
		{
			modbus->error_code = eModbusErrorCode_InvalidFunction;
			modbus->state = eModbusState_Error;
			TickTimer_Set(&error_time, 1000);
		}

		if (BIT_READ(modbus->rx_message.function_code, 7))
		{
			modbus->error_code = USART2_INrb.buffer[2];
			modbus->state = eModbusState_Error;
			TickTimer_Set(&error_time, 1000);
		}

		/* Take the incoming data length */
		if (modbus->rx_message.message_length == 0 && modbus->rx_message.function_code <= eModbusFunctionCode_ReadInputRegisters)
		{
			modbus->rx_message.message_length = USART2_INrb.buffer[2] + 5;
		}
		else if(modbus->rx_message.function_code >= eModbusFunctionCode_WriteSingleCoil)
		{
			modbus->rx_message.message_length = modbus->tx_message.tx_index;
			if(modbus->rx_message.function_code == eModbusFunctionCode_WriteMultipleCoils
				|| modbus->rx_message.function_code == eModbusFunctionCode_WriteMultipleRegisters)
			{
				modbus->rx_message.message_length = 8;
			}
		}

		/* Wait all data */
		if (USART2_INrb.head >= modbus->rx_message.message_length && modbus->rx_message.message_length != 0)
		{
			modbus->rx_message.message_length = 0;
			/* CRC Control*/
			modbus->rx_message.crc16 = 0xFFFF;
			memcpy(modbus->rx_message.rx_data, USART2_INrb.buffer, USART2_INrb.head);
			modbus->rx_message.rx_index = USART2_INrb.head;
			modbus->rx_message.crc16 = CRC16_Calculate_With_LookUp_Table(modbus->rx_message.rx_data, modbus->rx_message.rx_index - 2);
			if (LSB(modbus->rx_message.crc16) != modbus->rx_message.rx_data[modbus->rx_message.rx_index - 2] ||
			MSB(modbus->rx_message.crc16) != modbus->rx_message.rx_data[modbus->rx_message.rx_index - 1])
			{
				/*CRC ERROR*/
				modbus->error_code = eModbusErrorCode_InvalidCRC;
				modbus->state = eModbusState_Error;
				TickTimer_Set(&error_time, 1000);
			}

			if (modbus->state != eModbusState_Error)
			{
				switch (modbus->rx_message.function_code)
				{
				case eModbusFunctionCode_ReadCoilStatus:
				{
					output_10thcoil = BIT_READ(modbus->rx_message.rx_data[3], 0);
					break;
				}
				case eModbusFunctionCode_ReadInputStatus:
				{
					input1 = BIT_READ(modbus->rx_message.rx_data[3],0);
					input2 = BIT_READ(modbus->rx_message.rx_data[3],1);
					input3 = BIT_READ(modbus->rx_message.rx_data[3],2);
					input4 = BIT_READ(modbus->rx_message.rx_data[3],3);
					input5 = BIT_READ(modbus->rx_message.rx_data[3],4);
					break;
				}
				case eModbusFunctionCode_ReadHoldingRegisters:
				{
					uint8_t data_length = modbus->rx_message.rx_data[2] / 2; /* Divide by two cause we are reading as 16bit*/
					uint8_t index = 3;

					for(int i = 0; i < data_length; i++)
					{		                 // LSB                                          // MSB
						analog_value[i] = (modbus->rx_message.rx_data[index + 1 + i]) | (modbus->rx_message.rx_data[index + i]  << 8 );
						index +=1;
					}

					break;
				}
				case eModbusFunctionCode_ReadInputRegisters:
				{
					uint8_t data_length = modbus->rx_message.rx_data[2] / 2; /* Divide by two cause we are reading as 16bit*/
					uint8_t index = 3;

					for(int i = 0; i < data_length; i++)
					{		                 // LSB                                          // MSB
						analog_value[i] = (modbus->rx_message.rx_data[index + 1 + i]) | (modbus->rx_message.rx_data[index + i]  << 8 );
						index +=1;
					}
					break;
				}
				case eModbusFunctionCode_WriteSingleCoil:
				{
					if(are_elements_equal(modbus->tx_message.tx_data,modbus->rx_message.rx_data,modbus->tx_message.tx_index) == false)
					{
						return false;
					}
					break;
				}
				case eModbusFunctionCode_WriteSingleRegister:
				{
					if(are_elements_equal(modbus->tx_message.tx_data,modbus->rx_message.rx_data,modbus->tx_message.tx_index) == false)
					{
						return false;
					}
					break;
				}
				case eModbusFunctionCode_WriteMultipleCoils:
				{
					if(modbus->rx_message.rx_data[5] != modbus->tx_message.num_of_registers)
					{
						return false;
					}
					break;
				}
				case eModbusFunctionCode_WriteMultipleRegisters:
				{
					if(modbus->rx_message.rx_data[5] != modbus->tx_message.num_of_registers)
					{
						return false;
					}
					break;
				}
				default:
					break;
				}
				modbus->state = eModbusState_Idle;
			}
		}

		break;
	}

	case eModbusState_Error: // Handle error
	{
		if(TickTimer_CheckElapsedTime(&error_time))
		{
			modbus->state = eModbusState_Idle;
		}
		break;
	}

	default:
	{
		break;
	}
	}

	return true;
}

bool Modbus_ReadCoilStatus(sModbus_t *modbus, uint8_t slave_id, uint16_t start_address, uint16_t number_of_coils)
{
	if (modbus == NULL)
	{
		return false;
	}

	if ((slave_id < 1) || (slave_id > 247))
	{
		return false;
	}

	if (start_address > 0xFFFF)
	{
		return false;
	}

	if ((number_of_coils == 0) || (number_of_coils > 2000))
	{
		return false;
	}

	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.function_code = eModbusFunctionCode_ReadCoilStatus;
	modbus->tx_message.start_address = start_address;
	modbus->tx_message.num_of_registers = number_of_coils;
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}
	return true;
}

bool Modbus_ReadInputStatus(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address, uint16_t number_of_inputs)
{
	if (modbus == NULL)
	{
		return false;
	}

	if ((slave_id < 1) || (slave_id > 247))
	{
		return false;
	}

	if (start_address > 0xFFFF)
	{
		return false;
	}

	if ((number_of_inputs == 0) || (number_of_inputs > 2000))
	{
		return false;
	}


	modbus->tx_message.function_code = eModbusFunctionCode_ReadInputStatus;
	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.start_address = start_address;
	modbus->tx_message.num_of_registers = number_of_inputs;
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}

	return true;
}

bool Modbus_WriteSingleCoil(sModbus_t *modbus, uint8_t slave_id,uint16_t coil_number, eModbusCoilStatus_t coil_status)
{

    if (modbus == NULL)
    {
        return false;
    }

    if ((slave_id < 1) || (slave_id > 247))
    {
        return false;
    }

    if (coil_number > 0xFFFF)
    {
        return false;
    }

    if ((coil_status != eModbusCoilStatus_Off) && (coil_status != eModbusCoilStatus_On))
    {
        return false; // Invalid coil status
    }

	modbus->tx_message.function_code = eModbusFunctionCode_WriteSingleCoil;
	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.start_address = coil_number;

	modbus->tx_message.write_data[0] = coil_status;
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}

	return true;
}

bool Modbus_WriteMultipleCoils(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address,uint16_t number_of_coils,uint16_t coil_status)
{
    if (modbus == NULL)
    {
        return false;
    }

    if ((slave_id < 1) || (slave_id > 247))
    {
        return false;
    }

    if (start_address > 0xFFFF)
    {
        return false;
    }

    if ((number_of_coils == 0) || (number_of_coils > 2000))
    {
        return false;
    }

	modbus->tx_message.function_code = eModbusFunctionCode_WriteMultipleCoils;
	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.start_address = start_address;
	modbus->tx_message.num_of_registers = number_of_coils;

	modbus->tx_message.write_data[0] = (uint8_t)(ceil((double)number_of_coils / 8));
	modbus->tx_message.write_data[1] = coil_status;
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}
    return true;
}
bool Modbus_ReadHoldingRegisters(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address, uint16_t number_of_registers)
{
	if (modbus == NULL)
	{
		return false;
	}

	if ((slave_id < 1) || (slave_id > 247))
	{
		return false;
	}

	if (start_address > 0xFFFF)
	{
		return false;
	}

	if ((number_of_registers == 0) || (number_of_registers > 2000))
	{
		return false;
	}


	modbus->tx_message.function_code = eModbusFunctionCode_ReadHoldingRegisters;
	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.start_address = start_address;
	modbus->tx_message.num_of_registers = number_of_registers;
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}

	return true;
}

bool Modbus_ReadInputRegisters(sModbus_t *modbus, uint8_t slave_id, uint16_t start_address, uint16_t number_of_registers)
{

	if (modbus == NULL)
	{
		return false;
	}

	if ((slave_id < 1) || (slave_id > 247))
	{
		return false;
	}

	if (start_address > 0xFFFF)
	{
		return false;
	}

	if ((number_of_registers == 0) || (number_of_registers > 125))
	{
		return false;
	}

	modbus->tx_message.function_code = eModbusFunctionCode_ReadInputRegisters;
	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.start_address = start_address;
	modbus->tx_message.num_of_registers = number_of_registers;
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}

	return true;

}
uint8_t Modbus_Master_Write(uint8_t *data, size_t data_length)
{
	HAL_GPIO_WritePin(TX_RX_EN1_GPIO_Port, TX_RX_EN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TX_RX_EN2_GPIO_Port, TX_RX_EN2_Pin, GPIO_PIN_SET);

	if (HAL_UART_Transmit(&huart2, data, data_length, HAL_MAX_DELAY) == HAL_OK)
	{
		HAL_GPIO_WritePin(TX_RX_EN1_GPIO_Port, TX_RX_EN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(TX_RX_EN2_GPIO_Port, TX_RX_EN2_Pin, GPIO_PIN_RESET);
		return HAL_OK;
	}
	return HAL_ERROR;
}

bool Modbus_WriteSingleRegister(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address,uint16_t value)
{

    if (modbus == NULL)
    {
        return false;
    }

    if (slave_id < 1 || slave_id > 247)
    {
        return false;
    }

    if (start_address > 0xFFFF)
    {
        return false;
    }

	modbus->tx_message.function_code = eModbusFunctionCode_WriteSingleRegister;
	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.start_address = start_address;

	modbus->tx_message.write_data[0] = value;
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}
    return true;
}

bool Modbus_WriteMultipleRegisters(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address,uint16_t number_of_registers,const uint16_t *data)
{

    if (modbus == NULL)
    {
        return false;
    }

    if (slave_id < 1 || slave_id > 247)
    {
        return false;
    }

    if (start_address > 0xFFFF)
    {
        return false;
    }

    if (number_of_registers == 0 || number_of_registers > 123)
    {
        return false;
    }

    if (data == NULL)
    {
        return false;
    }

	modbus->tx_message.function_code = eModbusFunctionCode_WriteMultipleRegisters;
	modbus->tx_message.slave_address = slave_id;
	modbus->tx_message.start_address = start_address;
	modbus->tx_message.num_of_registers = number_of_registers;

	modbus->tx_message.write_data[0] = number_of_registers * 2;

	for(int i = 0; i < number_of_registers; i++)
	{
		modbus->tx_message.write_data[i + 1] = data[i];
	}
	modbus->state = eModbusState_Idle;

	if ((modbus->state == eModbusState_Idle) && (modbus->tx_message.new_send_message_status == false))
	{

		modbus->tx_message.new_send_message_status = true;
		TickTimer_Set(&idle_time, 100);
	}
    return true;
}
static bool are_elements_equal(uint8_t *tx_array,uint8_t *rx_array,uint8_t num_of_elements)
{

	for(int i = 0; i < num_of_elements ; i++)
	{
		if(tx_array[i] != rx_array[i])
		{
			return false;
		}

	}
	return true;
}
