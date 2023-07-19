#ifndef _MB_PACKET_C
#define _MB_PACKET_C

#ifdef __cplusplus
extern "C" {
#endif

#include "mb.h"

#if (MB_MODE==MB_MODE_MASTER) || defined MB_DEBUG

void mb_packet_request_read_coil(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
void mb_packet_request_read_discrete_inputs(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
void mb_packet_request_read_holding_registers(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
void mb_packet_request_read_input_registers(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
void mb_packet_request_write_single_coil(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t CAddress,uint16_t Data);
void mb_packet_request_write_single_register(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t RAddress,uint16_t Data);
void mb_packet_request_write_multiple_coils(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);
void mb_packet_request_write_multiple_registers(mb_packet_s* Packet, uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);

#endif
#if (MB_MODE==MB_MODE_SLAVE) || defined MB_DEBUG

void mb_packet_response_read_coil(mb_packet_s* Packet, uint8_t len,uint8_t *Data);
void mb_packet_response_read_discrete_inputs(mb_packet_s* Packet, uint8_t len,uint8_t *Data);
void mb_packet_response_read_holding_registers(mb_packet_s* Packet, uint8_t len,uint8_t *Data);
void mb_packet_response_read_input_registers(mb_packet_s* Packet, uint8_t len,uint8_t *Data);
void mb_packet_response_write_single_coil(mb_packet_s* Packet, uint16_t CAddress,uint16_t Data);
void mb_packet_response_write_single_register(mb_packet_s* Packet, uint16_t RAddress,uint16_t Data);
void mb_packet_response_write_multiple_coils(mb_packet_s* Packet, uint16_t StartAd,uint16_t Quantity);
void mb_packet_response_write_multiple_registers(mb_packet_s* Packet, uint16_t StartAd,uint16_t Quantity);

void mb_packet_error(mb_packet_s* Packet, mb_functions_e Func,mb_error_e Exeption_Code);

#endif

#ifdef __cplusplus
}
#endif

#endif
