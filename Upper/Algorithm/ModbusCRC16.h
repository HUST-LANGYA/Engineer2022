#ifndef __MODBUS_CRC16_H
#define __MODBUS_CRC16_H
#include "main.h"


uint32_t Verify_modbusCRC16_Check_Sum(uint8_t *pchMessage, uint16_t dwLength);
uint16_t crc16bitbybit(uint8_t *ptr, uint16_t len);
uint16_t crc16table(uint8_t *ptr, uint16_t len);
uint16_t crc16tablefast(uint8_t *ptr, uint16_t len);






#endif
