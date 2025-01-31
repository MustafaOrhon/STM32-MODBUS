/*
 * modbus_crc16.h
 *
 *  Created on: Jan 25, 2025
 *      Author: Mustafa
 */

#ifndef OUR_DRIVERS_MODBUS_EX_INC_MODBUS_CRC16_H_
#define OUR_DRIVERS_MODBUS_EX_INC_MODBUS_CRC16_H_

#include <stdint.h>

uint16_t calculate_crc(uint16_t crc_value,uint8_t data);
uint16_t CRC16_Calculate_With_LookUp_Table(uint8_t *buffer, uint16_t length);
#endif /* OUR_DRIVERS_MODBUS_EX_INC_MODBUS_CRC16_H_ */
