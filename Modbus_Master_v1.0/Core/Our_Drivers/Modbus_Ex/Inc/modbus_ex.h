/*
 * modbus_ex.h
 *
 *  Created on: Jan 25, 2025
 *      Author: Mustafa
 */

#ifndef OUR_DRIVERS_MODBUS_EX_INC_MODBUS_EX_H_
#define OUR_DRIVERS_MODBUS_EX_INC_MODBUS_EX_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "modbus_crc16.h"
#include "timer_ex.h"
#include "ring_buffer.h"

#define MODBUS_TX_MAX_SIZE 252
#define MODBUS_RX_MAX_SIZE 64

typedef enum
{
	eModbusFunctionCode__First = 0x01,
	eModbusFunctionCode_ReadCoilStatus = eModbusFunctionCode__First,
	eModbusFunctionCode_ReadInputStatus = 0x02,
	eModbusFunctionCode_ReadHoldingRegisters = 0x03,
	eModbusFunctionCode_ReadInputRegisters = 0x04,
	eModbusFunctionCode_WriteSingleCoil = 0x05,
	eModbusFunctionCode_WriteSingleRegister = 0x06,
	eModbusFunctionCode_WriteMultipleCoils = 0x0F,
	eModbusFunctionCode_WriteMultipleRegisters = 0x10,
	eModbusFunctionCode__Last,
} eModbusFunctionCode_t;

typedef enum
{
	eModbusMode_First = 0,
	eModbusMode_ASCII = eModbusMode_First,
	eModbusMode_RTU,
	eModbusMode_Last
} eModbusMode_t;

typedef struct
{
	uint8_t slave_address;
	eModbusFunctionCode_t function_code;
	uint16_t start_address;
	uint16_t num_of_registers;
	uint16_t write_data[MODBUS_TX_MAX_SIZE];
	uint8_t message_length;
	uint8_t tx_data[MODBUS_TX_MAX_SIZE];
	uint8_t tx_index;
	uint16_t crc16;

	bool new_send_message_status;
} sModbusTXMessage_t;

typedef struct
{
	uint8_t rx_data[MODBUS_RX_MAX_SIZE];
	uint8_t rx_index;
	uint8_t message_length;
	uint8_t slave_address;
	uint8_t function_code;
	sTickTime_t rx_time;
	uint16_t crc16;
	bool new_receive_msg_status;
} sModbusRXMessage_t;

typedef enum
{
	eModbusState_First = 0,
	eModbusState_Idle = eModbusState_First,
	eModbusState_SendRequst,
	eModbusState_WaitingForReply,
	eModbusState_ProccessingReply,
	eModbusState_Error,
	eModbusState_Last
} eModbusState_t;

typedef enum
{
	eModbusErrorCode_First = 0,
	eModbusErrorCode_NoError = eModbusErrorCode_First,
	eModbusErrorCode_InvalidFunction,
	eModbusErrorCode_DataAddress,
	eModbusErrorCode_DataValue,
	eModbusErrorCode_SlaveDeviceError,
	eModbusErrorCode_InfoMessage,
	eModbusErrorCode_InvalidCRC,
	eModbusErrorCode_Timeout,
	eModbusErrorCode_Last
}
eModbusErrorCode_t;

typedef enum
{
	eModbusCoilStatus_Off = 0x0000,
	eModbusCoilStatus_On = 0xFF00
}eModbusCoilStatus_t;

typedef struct
{
	sModbusTXMessage_t tx_message;
	sModbusRXMessage_t rx_message;
	eModbusMode_t mode;
	eModbusState_t state;
	eModbusErrorCode_t error_code;
} sModbus_t;

bool Modbus_Master_Control(sModbus_t *modbus);
bool Modbus_ReadCoilStatus(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address, uint16_t number_of_coils);
bool Modbus_ReadInputStatus(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address, uint16_t number_of_inputs);
bool Modbus_ReadInputRegisters(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address, uint16_t number_of_registers);
bool Modbus_ReadHoldingRegisters(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address, uint16_t number_of_registers);
bool Modbus_WriteSingleCoil(sModbus_t *modbus, uint8_t slave_id,uint16_t coil_number, eModbusCoilStatus_t coil_status);
bool Modbus_WriteSingleRegister(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address,uint16_t value);
bool Modbus_WriteMultipleRegisters(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address,uint16_t number_of_registers,const uint16_t *data);
bool Modbus_WriteMultipleCoils(sModbus_t *modbus, uint8_t slave_id,uint16_t start_address,uint16_t number_of_coils,uint16_t coil_status);
uint8_t Modbus_Master_Write(uint8_t *data,size_t data_length);
#endif /* OUR_DRIVERS_MODBUS_EX_INC_MODBUS_EX_H_ */
