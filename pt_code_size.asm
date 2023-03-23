;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.2.0 #13081 (MINGW64)
;--------------------------------------------------------
	.module mb_process
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _mb_packet_response_write_multiple_registers
	.globl _mb_packet_response_write_multiple_coils
	.globl _mb_packet_response_write_single_register
	.globl _mb_packet_response_write_single_coil
	.globl _mb_packet_response_read_input_registers
	.globl _mb_packet_response_read_holding_registers
	.globl _mb_packet_response_read_discrete_inputs
	.globl _mb_packet_response_read_coil
	.globl _mb_table_read_bit
	.globl _mb_table_write_bit
	.globl _mb_table_read
	.globl _mb_table_write
	.globl _mb_tx_packet_handler
	.globl _MB_PROCESS_Buffer
	.globl _mb_slave_process_read_coils
	.globl _mb_slave_process_read_discrere_inputs
	.globl _mb_slave_process_read_holding_registers
	.globl _mb_slave_process_read_input_registers
	.globl _mb_slave_process_write_single_coil
	.globl _mb_slave_process_write_single_register
	.globl _mb_slave_process_write_multiple_coils
	.globl _mb_slave_process_write_multiple_register
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
_MB_PROCESS_Buffer::
	.ds 32
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	lib\modbus\mb-process.c: 19: mb_error_e mb_slave_process_read_coils(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_read_coils
;	-----------------------------------------
_mb_slave_process_read_coils:
	sub	sp, #31
;	lib\modbus\mb-process.c: 24: Start=Packet->u16_1;
	ldw	(0x1a, sp), x
	ldw	x, (0x3, x)
;	lib\modbus\mb-process.c: 25: End=(uint32_t)Packet->u16_2+Start;
	ldw	y, (0x1a, sp)
	ldw	y, (0x5, y)
	clr	a
	clr	(0x12, sp)
	ldw	(0x18, sp), x
	clr	(0x17, sp)
	clr	(0x16, sp)
	addw	y, (0x18, sp)
	adc	a, (0x17, sp)
	ld	(0x1d, sp), a
	ld	a, (0x12, sp)
	adc	a, (0x16, sp)
	ldw	(0x0e, sp), y
	ld	(0x0c, sp), a
	ld	a, (0x1d, sp)
	ld	(0x0d, sp), a
;	lib\modbus\mb-process.c: 27: for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
	ldw	(0x1c, sp), x
	clrw	x
	ldw	(0x1e, sp), x
00110$:
	ldw	y, (0x1c, sp)
	ldw	(0x12, sp), y
	clrw	x
	ldw	(0x10, sp), x
	ldw	y, (0x1e, sp)
	ldw	(0x14, sp), y
	ldw	x, y
	srlw	x
	srlw	x
	srlw	x
	ldw	(0x16, sp), x
;	lib\modbus\mb-process.c: 29: if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
	ld	a, (0x15, sp)
	and	a, #0x07
	ld	(0x19, sp), a
	clr	(0x18, sp)
;	lib\modbus\mb-process.c: 27: for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
	ldw	x, (0x12, sp)
	cpw	x, (0x0e, sp)
	ld	a, (0x11, sp)
	sbc	a, (0x0d, sp)
	ld	a, (0x10, sp)
	sbc	a, (0x0c, sp)
	jrnc	00103$
	ldw	x, (0x16, sp)
	cpw	x, #0x001e
	jrnc	00103$
;	lib\modbus\mb-process.c: 29: if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
	ldw	x, (0x18, sp)
	jrne	00102$
	ldw	x, (0x16, sp)
	clr	((_MB_PROCESS_Buffer+0), x)
00102$:
;	lib\modbus\mb-process.c: 30: MB_PROCESS_Buffer[i/8]|=mb_table_read_bit(TBALE_Coils,Start)<<(i%8);
	ldw	x, (0x16, sp)
	addw	x, #(_MB_PROCESS_Buffer+0)
	ld	a, (x)
	ld	(0x17, sp), a
	pushw	x
	ldw	y, (0x1e, sp)
	pushw	y
	ldw	x, #(_TBALE_Coils+0)
	call	_mb_table_read_bit
	popw	x
	exg	a, yl
	ld	a, (0x19, sp)
	exg	a, yl
	push	a
	ld	a, yl
	tnz	a
	jreq	00149$
00148$:
	sll	(1, sp)
	dec	a
	jrne	00148$
00149$:
	pop	a
	or	a, (0x17, sp)
	ld	(x), a
;	lib\modbus\mb-process.c: 27: for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
	ldw	x, (0x1c, sp)
	incw	x
	ldw	(0x1c, sp), x
	ldw	x, (0x1e, sp)
	incw	x
	ldw	(0x1e, sp), x
	jra	00110$
00103$:
;	lib\modbus\mb-process.c: 33: Size=i/8;
	ldw	x, (0x16, sp)
;	lib\modbus\mb-process.c: 34: if(i%8)Size++;
	ldw	y, (0x18, sp)
	jreq	00105$
	incw	x
00105$:
;	lib\modbus\mb-process.c: 37: mb_packet_response_read_coil(&tmpPacket,Size,MB_PROCESS_Buffer);
	push	#<(_MB_PROCESS_Buffer+0)
	push	#((_MB_PROCESS_Buffer+0) >> 8)
	ld	a, xl
	ldw	x, sp
	addw	x, #3
	call	_mb_packet_response_read_coil
;	lib\modbus\mb-process.c: 39: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x1a, sp)
	ld	a, (x)
	jreq	00107$
;	lib\modbus\mb-process.c: 41: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	incw	x
	call	_mb_tx_packet_handler
00107$:
;	lib\modbus\mb-process.c: 42: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 43: }
	addw	sp, #31
	ret
;	lib\modbus\mb-process.c: 45: mb_error_e mb_slave_process_read_discrere_inputs(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_read_discrere_inputs
;	-----------------------------------------
_mb_slave_process_read_discrere_inputs:
	sub	sp, #31
;	lib\modbus\mb-process.c: 50: Start=Packet->u16_1;
	ldw	(0x1a, sp), x
	ldw	x, (0x3, x)
;	lib\modbus\mb-process.c: 51: End=(uint32_t)Packet->u16_2+Start;
	ldw	y, (0x1a, sp)
	ldw	y, (0x5, y)
	clr	a
	clr	(0x12, sp)
	ldw	(0x18, sp), x
	clr	(0x17, sp)
	clr	(0x16, sp)
	addw	y, (0x18, sp)
	adc	a, (0x17, sp)
	ld	(0x1d, sp), a
	ld	a, (0x12, sp)
	adc	a, (0x16, sp)
	ldw	(0x0e, sp), y
	ld	(0x0c, sp), a
	ld	a, (0x1d, sp)
	ld	(0x0d, sp), a
;	lib\modbus\mb-process.c: 53: for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
	ldw	(0x1c, sp), x
	clrw	x
	ldw	(0x1e, sp), x
00110$:
	ldw	y, (0x1c, sp)
	ldw	(0x12, sp), y
	clrw	x
	ldw	(0x10, sp), x
	ldw	y, (0x1e, sp)
	ldw	(0x14, sp), y
	ldw	x, y
	srlw	x
	srlw	x
	srlw	x
	ldw	(0x16, sp), x
;	lib\modbus\mb-process.c: 55: if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
	ld	a, (0x15, sp)
	and	a, #0x07
	ld	(0x19, sp), a
	clr	(0x18, sp)
;	lib\modbus\mb-process.c: 53: for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
	ldw	x, (0x12, sp)
	cpw	x, (0x0e, sp)
	ld	a, (0x11, sp)
	sbc	a, (0x0d, sp)
	ld	a, (0x10, sp)
	sbc	a, (0x0c, sp)
	jrnc	00103$
	ldw	x, (0x16, sp)
	cpw	x, #0x001e
	jrnc	00103$
;	lib\modbus\mb-process.c: 55: if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
	ldw	x, (0x18, sp)
	jrne	00102$
	ldw	x, (0x16, sp)
	clr	((_MB_PROCESS_Buffer+0), x)
00102$:
;	lib\modbus\mb-process.c: 56: MB_PROCESS_Buffer[i/8]|=mb_table_read_bit(TBALE_Discretes_Input,Start)<<(i%8);
	ldw	x, (0x16, sp)
	addw	x, #(_MB_PROCESS_Buffer+0)
	ld	a, (x)
	ld	(0x17, sp), a
	pushw	x
	ldw	y, (0x1e, sp)
	pushw	y
	ldw	x, #(_TBALE_Discretes_Input+0)
	call	_mb_table_read_bit
	popw	x
	exg	a, yl
	ld	a, (0x19, sp)
	exg	a, yl
	push	a
	ld	a, yl
	tnz	a
	jreq	00149$
00148$:
	sll	(1, sp)
	dec	a
	jrne	00148$
00149$:
	pop	a
	or	a, (0x17, sp)
	ld	(x), a
;	lib\modbus\mb-process.c: 53: for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
	ldw	x, (0x1c, sp)
	incw	x
	ldw	(0x1c, sp), x
	ldw	x, (0x1e, sp)
	incw	x
	ldw	(0x1e, sp), x
	jra	00110$
00103$:
;	lib\modbus\mb-process.c: 59: Size=i/8;
	ldw	x, (0x16, sp)
;	lib\modbus\mb-process.c: 60: if(i%8)Size++;
	ldw	y, (0x18, sp)
	jreq	00105$
	incw	x
00105$:
;	lib\modbus\mb-process.c: 63: mb_packet_response_read_discrete_inputs(&tmpPacket,Size,MB_PROCESS_Buffer);
	push	#<(_MB_PROCESS_Buffer+0)
	push	#((_MB_PROCESS_Buffer+0) >> 8)
	ld	a, xl
	ldw	x, sp
	addw	x, #3
	call	_mb_packet_response_read_discrete_inputs
;	lib\modbus\mb-process.c: 65: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x1a, sp)
	ld	a, (x)
	jreq	00107$
;	lib\modbus\mb-process.c: 67: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	incw	x
	call	_mb_tx_packet_handler
00107$:
;	lib\modbus\mb-process.c: 68: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 69: }
	addw	sp, #31
	ret
;	lib\modbus\mb-process.c: 71: mb_error_e mb_slave_process_read_holding_registers(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_read_holding_registers
;	-----------------------------------------
_mb_slave_process_read_holding_registers:
	sub	sp, #30
;	lib\modbus\mb-process.c: 76: Start=Packet->u16_1;
	ldw	(0x1b, sp), x
	ldw	y, x
	ldw	y, (0x3, y)
;	lib\modbus\mb-process.c: 77: End=(uint32_t)Packet->u16_2+Start;
	ldw	x, (0x1b, sp)
	ldw	x, (0x5, x)
	clr	a
	clr	(0x01, sp)
	ldw	(0x15, sp), y
	clr	(0x14, sp)
	clr	(0x13, sp)
	addw	x, (0x15, sp)
	adc	a, (0x14, sp)
	ld	(0x18, sp), a
	ld	a, (0x01, sp)
	adc	a, (0x13, sp)
	ldw	(0x12, sp), x
	ld	(0x10, sp), a
	ld	a, (0x18, sp)
	ld	(0x11, sp), a
;	lib\modbus\mb-process.c: 79: for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
	clrw	x
	ldw	(0x14, sp), x
	ldw	(0x1d, sp), y
00106$:
	ldw	y, (0x1d, sp)
	ldw	(0x18, sp), y
	clrw	x
	ldw	(0x16, sp), x
;	lib\modbus\mb-process.c: 84: MB_PROCESS_Buffer[i+1]=Temp&0xff;
	ld	a, (0x15, sp)
	ld	(0x1a, sp), a
;	lib\modbus\mb-process.c: 79: for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
	ldw	x, (0x18, sp)
	cpw	x, (0x12, sp)
	ld	a, (0x17, sp)
	sbc	a, (0x11, sp)
	ld	a, (0x16, sp)
	sbc	a, (0x10, sp)
	jrnc	00101$
	ldw	x, (0x14, sp)
	cpw	x, #0x001e
	jrnc	00101$
;	lib\modbus\mb-process.c: 81: Temp=mb_table_read(TABLE_Holding_Registers,Start);
	ldw	x, (0x1d, sp)
	pushw	x
	ldw	x, #(_TABLE_Holding_Registers+0)
	call	_mb_table_read
	exgw	x, y
;	lib\modbus\mb-process.c: 83: MB_PROCESS_Buffer[i]=(Temp>>8)&0xff;
	ldw	x, (0x14, sp)
	addw	x, #(_MB_PROCESS_Buffer+0)
	ld	a, yh
	ld	(x), a
;	lib\modbus\mb-process.c: 84: MB_PROCESS_Buffer[i+1]=Temp&0xff;
	ld	a, (0x1a, sp)
	inc	a
	ld	xl, a
	rlc	a
	clr	a
	sbc	a, #0x00
	ld	xh, a
	ld	a, yl
	ld	((_MB_PROCESS_Buffer+0), x), a
;	lib\modbus\mb-process.c: 79: for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
	ldw	x, (0x1d, sp)
	incw	x
	ldw	(0x1d, sp), x
	ldw	x, (0x14, sp)
	incw	x
	incw	x
	ldw	(0x14, sp), x
	jra	00106$
00101$:
;	lib\modbus\mb-process.c: 87: mb_packet_s tmpPacket = {0};
	clr	(0x05, sp)
	clr	(0x06, sp)
	clr	(0x07, sp)
	clrw	x
	ldw	(0x08, sp), x
	clrw	x
	ldw	(0x0a, sp), x
	clr	(0x0c, sp)
	clrw	x
	ldw	(0x0d, sp), x
	clr	(0x0f, sp)
;	lib\modbus\mb-process.c: 88: mb_packet_response_read_holding_registers(&tmpPacket,i,MB_PROCESS_Buffer);
	push	#<(_MB_PROCESS_Buffer+0)
	push	#((_MB_PROCESS_Buffer+0) >> 8)
	ld	a, (0x1c, sp)
	ldw	x, sp
	addw	x, #7
	call	_mb_packet_response_read_holding_registers
;	lib\modbus\mb-process.c: 90: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x1b, sp)
	ld	a, (x)
	jreq	00103$
;	lib\modbus\mb-process.c: 92: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	addw	x, #5
	call	_mb_tx_packet_handler
00103$:
;	lib\modbus\mb-process.c: 93: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 94: }
	addw	sp, #30
	ret
;	lib\modbus\mb-process.c: 96: mb_error_e mb_slave_process_read_input_registers(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_read_input_registers
;	-----------------------------------------
_mb_slave_process_read_input_registers:
	sub	sp, #30
;	lib\modbus\mb-process.c: 101: Start=Packet->u16_1;
	ldw	(0x1b, sp), x
	ldw	y, x
	ldw	y, (0x3, y)
;	lib\modbus\mb-process.c: 102: End=(uint32_t)Packet->u16_2+Start;
	ldw	x, (0x1b, sp)
	ldw	x, (0x5, x)
	clr	a
	clr	(0x01, sp)
	ldw	(0x15, sp), y
	clr	(0x14, sp)
	clr	(0x13, sp)
	addw	x, (0x15, sp)
	adc	a, (0x14, sp)
	ld	(0x18, sp), a
	ld	a, (0x01, sp)
	adc	a, (0x13, sp)
	ldw	(0x12, sp), x
	ld	(0x10, sp), a
	ld	a, (0x18, sp)
	ld	(0x11, sp), a
;	lib\modbus\mb-process.c: 104: for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
	clrw	x
	ldw	(0x14, sp), x
	ldw	(0x1d, sp), y
00106$:
	ldw	y, (0x1d, sp)
	ldw	(0x18, sp), y
	clrw	x
	ldw	(0x16, sp), x
;	lib\modbus\mb-process.c: 109: MB_PROCESS_Buffer[i+1]=Temp&0xff;
	ld	a, (0x15, sp)
	ld	(0x1a, sp), a
;	lib\modbus\mb-process.c: 104: for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
	ldw	x, (0x18, sp)
	cpw	x, (0x12, sp)
	ld	a, (0x17, sp)
	sbc	a, (0x11, sp)
	ld	a, (0x16, sp)
	sbc	a, (0x10, sp)
	jrnc	00101$
	ldw	x, (0x14, sp)
	cpw	x, #0x001e
	jrnc	00101$
;	lib\modbus\mb-process.c: 106: Temp=mb_table_read(TBALE_Input_Registers,Start);
	ldw	x, (0x1d, sp)
	pushw	x
	ldw	x, #(_TBALE_Input_Registers+0)
	call	_mb_table_read
	exgw	x, y
;	lib\modbus\mb-process.c: 108: MB_PROCESS_Buffer[i]=(Temp>>8)&0xff;
	ldw	x, (0x14, sp)
	addw	x, #(_MB_PROCESS_Buffer+0)
	ld	a, yh
	ld	(x), a
;	lib\modbus\mb-process.c: 109: MB_PROCESS_Buffer[i+1]=Temp&0xff;
	ld	a, (0x1a, sp)
	inc	a
	ld	xl, a
	rlc	a
	clr	a
	sbc	a, #0x00
	ld	xh, a
	ld	a, yl
	ld	((_MB_PROCESS_Buffer+0), x), a
;	lib\modbus\mb-process.c: 104: for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
	ldw	x, (0x1d, sp)
	incw	x
	ldw	(0x1d, sp), x
	ldw	x, (0x14, sp)
	incw	x
	incw	x
	ldw	(0x14, sp), x
	jra	00106$
00101$:
;	lib\modbus\mb-process.c: 112: mb_packet_s tmpPacket = {0};
	clr	(0x05, sp)
	clr	(0x06, sp)
	clr	(0x07, sp)
	clrw	x
	ldw	(0x08, sp), x
	clrw	x
	ldw	(0x0a, sp), x
	clr	(0x0c, sp)
	clrw	x
	ldw	(0x0d, sp), x
	clr	(0x0f, sp)
;	lib\modbus\mb-process.c: 113: mb_packet_response_read_input_registers(&tmpPacket,i,MB_PROCESS_Buffer);
	push	#<(_MB_PROCESS_Buffer+0)
	push	#((_MB_PROCESS_Buffer+0) >> 8)
	ld	a, (0x1c, sp)
	ldw	x, sp
	addw	x, #7
	call	_mb_packet_response_read_input_registers
;	lib\modbus\mb-process.c: 115: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x1b, sp)
	ld	a, (x)
	jreq	00103$
;	lib\modbus\mb-process.c: 117: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	addw	x, #5
	call	_mb_tx_packet_handler
00103$:
;	lib\modbus\mb-process.c: 118: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 119: }
	addw	sp, #30
	ret
;	lib\modbus\mb-process.c: 121: mb_error_e mb_slave_process_write_single_coil(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_write_single_coil
;	-----------------------------------------
_mb_slave_process_write_single_coil:
	sub	sp, #15
;	lib\modbus\mb-process.c: 123: mb_table_write_bit(TBALE_Coils,Packet->u16_1,Packet->u16_2>>8);
	ldw	(0x0e, sp), x
	ldw	y, x
	addw	y, #0x0005
	ldw	x, y
	ldw	x, (x)
	ld	a, xh
	ldw	x, (0x0e, sp)
	addw	x, #0x0003
	ldw	(0x0c, sp), x
	ldw	x, (x)
	pushw	y
	push	a
	pushw	x
	ldw	x, #(_TBALE_Coils+0)
	call	_mb_table_write_bit
	popw	y
;	lib\modbus\mb-process.c: 125: mb_packet_s tmpPacket = {0};
	clr	(0x01, sp)
	clr	(0x02, sp)
	clr	(0x03, sp)
	clrw	x
	ldw	(0x04, sp), x
	clrw	x
	ldw	(0x06, sp), x
	clr	(0x08, sp)
	clrw	x
	ldw	(0x09, sp), x
	clr	(0x0b, sp)
;	lib\modbus\mb-process.c: 126: mb_packet_response_write_single_coil(&tmpPacket,Packet->u16_1,Packet->u16_2);
	ldw	y, (y)
	ldw	x, (0x0c, sp)
	ldw	x, (x)
	pushw	y
	pushw	x
	ldw	x, sp
	addw	x, #5
	call	_mb_packet_response_write_single_coil
;	lib\modbus\mb-process.c: 128: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x0e, sp)
	ld	a, (x)
	jreq	00102$
;	lib\modbus\mb-process.c: 130: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	incw	x
	call	_mb_tx_packet_handler
00102$:
;	lib\modbus\mb-process.c: 131: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 132: }
	addw	sp, #15
	ret
;	lib\modbus\mb-process.c: 134: mb_error_e mb_slave_process_write_single_register(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_write_single_register
;	-----------------------------------------
_mb_slave_process_write_single_register:
	sub	sp, #17
;	lib\modbus\mb-process.c: 136: mb_table_write(TABLE_Holding_Registers,Packet->u16_1,Packet->u16_2);
	ldw	(0x10, sp), x
	addw	x, #0x0005
	ldw	(0x0c, sp), x
	ldw	x, (x)
	ldw	y, (0x10, sp)
	addw	y, #0x0003
	ldw	(0x0e, sp), y
	ldw	y, (y)
	pushw	x
	pushw	y
	ldw	x, #(_TABLE_Holding_Registers+0)
	call	_mb_table_write
;	lib\modbus\mb-process.c: 138: mb_packet_s tmpPacket = {0};
	clr	(0x01, sp)
	clr	(0x02, sp)
	clr	(0x03, sp)
	clrw	x
	ldw	(0x04, sp), x
	clrw	x
	ldw	(0x06, sp), x
	clr	(0x08, sp)
	clrw	x
	ldw	(0x09, sp), x
	clr	(0x0b, sp)
;	lib\modbus\mb-process.c: 139: mb_packet_response_write_single_register(&tmpPacket,Packet->u16_1,Packet->u16_2);
	ldw	x, (0x0c, sp)
	ldw	x, (x)
	ldw	y, (0x0e, sp)
	ldw	y, (y)
	pushw	x
	pushw	y
	ldw	x, sp
	addw	x, #5
	call	_mb_packet_response_write_single_register
;	lib\modbus\mb-process.c: 141: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x10, sp)
	ld	a, (x)
	jreq	00102$
;	lib\modbus\mb-process.c: 143: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	incw	x
	call	_mb_tx_packet_handler
00102$:
;	lib\modbus\mb-process.c: 144: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 145: }
	addw	sp, #17
	ret
;	lib\modbus\mb-process.c: 147: mb_error_e mb_slave_process_write_multiple_coils(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_write_multiple_coils
;	-----------------------------------------
_mb_slave_process_write_multiple_coils:
	sub	sp, #33
;	lib\modbus\mb-process.c: 152: Start=Packet->u16_1;
	ldw	(0x1e, sp), x
	addw	x, #0x0003
	ldw	(0x10, sp), x
	ldw	x, (x)
	exgw	x, y
;	lib\modbus\mb-process.c: 153: End=(uint32_t)Packet->u16_2+Start;
	ldw	x, (0x1e, sp)
	addw	x, #0x0005
	ldw	(0x12, sp), x
	ldw	x, (x)
	clr	a
	clr	(0x01, sp)
	ldw	(0x18, sp), y
	clr	(0x17, sp)
	clr	(0x16, sp)
	addw	x, (0x18, sp)
	adc	a, (0x17, sp)
	ld	(0x1b, sp), a
	ld	a, (0x01, sp)
	adc	a, (0x16, sp)
	ldw	(0x16, sp), x
	ld	(0x14, sp), a
	ld	a, (0x1b, sp)
	ld	(0x15, sp), a
;	lib\modbus\mb-process.c: 155: for(i=0;Start<End;Start++,i++)
	ldw	x, (0x1e, sp)
	ldw	(0x18, sp), x
	clrw	x
	ldw	(0x20, sp), x
00105$:
	ldw	x, y
	clr	a
	clr	(0x1a, sp)
	push	a
	cpw	x, (0x17, sp)
	pop	a
	sbc	a, (0x15, sp)
	ld	a, (0x1a, sp)
	sbc	a, (0x14, sp)
	jrnc	00101$
;	lib\modbus\mb-process.c: 157: mb_table_write_bit(TBALE_Coils,Start,((Packet->Data[i/8])>>(i%8))&0x01);
	ldw	x, (0x18, sp)
	ldw	x, (0x8, x)
	ldw	(0x1a, sp), x
	ldw	x, (0x20, sp)
	ldw	(0x1c, sp), x
	srlw	x
	srlw	x
	srlw	x
	addw	x, (0x1a, sp)
	ld	a, (x)
	ld	xl, a
	ld	a, (0x1d, sp)
	and	a, #0x07
	ld	xh, a
	tnz	a
	jreq	00127$
00126$:
	exg	a, xl
	srl	a
	exg	a, xl
	dec	a
	jrne	00126$
00127$:
	ld	a, xl
	and	a, #0x01
	pushw	y
	push	a
	pushw	y
	ldw	x, #(_TBALE_Coils+0)
	call	_mb_table_write_bit
	popw	y
;	lib\modbus\mb-process.c: 155: for(i=0;Start<End;Start++,i++)
	incw	y
	ldw	x, (0x20, sp)
	incw	x
	ldw	(0x20, sp), x
	jra	00105$
00101$:
;	lib\modbus\mb-process.c: 160: mb_packet_s tmpPacket = {0};
	clr	(0x05, sp)
	clr	(0x06, sp)
	clr	(0x07, sp)
	clrw	x
	ldw	(0x08, sp), x
	clrw	x
	ldw	(0x0a, sp), x
	clr	(0x0c, sp)
	clrw	x
	ldw	(0x0d, sp), x
	clr	(0x0f, sp)
;	lib\modbus\mb-process.c: 161: mb_packet_response_write_multiple_coils(&tmpPacket,Packet->u16_1,Packet->u16_2);
	ldw	x, (0x12, sp)
	ldw	x, (x)
	ldw	y, (0x10, sp)
	ldw	y, (y)
	pushw	x
	pushw	y
	ldw	x, sp
	addw	x, #9
	call	_mb_packet_response_write_multiple_coils
;	lib\modbus\mb-process.c: 163: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x1e, sp)
	ld	a, (x)
	jreq	00103$
;	lib\modbus\mb-process.c: 165: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	addw	x, #5
	call	_mb_tx_packet_handler
00103$:
;	lib\modbus\mb-process.c: 166: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 167: }
	addw	sp, #33
	ret
;	lib\modbus\mb-process.c: 169: mb_error_e mb_slave_process_write_multiple_register(mb_packet_s* Packet)
;	-----------------------------------------
;	 function mb_slave_process_write_multiple_register
;	-----------------------------------------
_mb_slave_process_write_multiple_register:
	sub	sp, #29
;	lib\modbus\mb-process.c: 174: Start=Packet->u16_1;
	ldw	(0x1c, sp), x
	addw	x, #0x0003
	ldw	(0x0c, sp), x
	ldw	x, (x)
	exgw	x, y
;	lib\modbus\mb-process.c: 175: End=(uint32_t)Packet->u16_2+Start;
	ldw	x, (0x1c, sp)
	addw	x, #0x0005
	ldw	(0x0e, sp), x
	ldw	x, (x)
	clr	a
	clr	(0x10, sp)
	ldw	(0x16, sp), y
	clr	(0x15, sp)
	clr	(0x14, sp)
	addw	x, (0x16, sp)
	adc	a, (0x15, sp)
	ld	(0x19, sp), a
	ld	a, (0x10, sp)
	adc	a, (0x14, sp)
	ldw	(0x12, sp), x
	ld	(0x10, sp), a
	ld	a, (0x19, sp)
	ld	(0x11, sp), a
;	lib\modbus\mb-process.c: 177: for(i=0;Start<End;Start++,i+=2)
	clrw	x
	ldw	(0x1a, sp), x
	ldw	x, (0x1c, sp)
	ldw	(0x14, sp), x
00105$:
	ldw	x, y
	clr	a
	clr	(0x16, sp)
	push	a
	cpw	x, (0x13, sp)
	pop	a
	sbc	a, (0x11, sp)
	ld	a, (0x16, sp)
	sbc	a, (0x10, sp)
	jrnc	00101$
;	lib\modbus\mb-process.c: 179: Temp=(Packet->Data[i]<<8)|(Packet->Data[i+1]);
	ldw	x, (0x14, sp)
	ldw	x, (0x8, x)
	ldw	(0x16, sp), x
	addw	x, (0x1a, sp)
	ld	a, (x)
	ld	(0x18, sp), a
	clr	(0x19, sp)
	ldw	x, (0x1a, sp)
	incw	x
	addw	x, (0x16, sp)
	ld	a, (x)
	clrw	x
	or	a, (0x19, sp)
	rlwa	x
	or	a, (0x18, sp)
	ld	xh, a
;	lib\modbus\mb-process.c: 180: mb_table_write(TABLE_Holding_Registers,Start,Temp);
	pushw	y
	pushw	x
	pushw	y
	ldw	x, #(_TABLE_Holding_Registers+0)
	call	_mb_table_write
	popw	y
;	lib\modbus\mb-process.c: 177: for(i=0;Start<End;Start++,i+=2)
	incw	y
	ldw	x, (0x1a, sp)
	incw	x
	incw	x
	ldw	(0x1a, sp), x
	jra	00105$
00101$:
;	lib\modbus\mb-process.c: 183: mb_packet_s tmpPacket = {0};
	clr	(0x01, sp)
	clr	(0x02, sp)
	clr	(0x03, sp)
	clrw	x
	ldw	(0x04, sp), x
	clrw	x
	ldw	(0x06, sp), x
	clr	(0x08, sp)
	clrw	x
	ldw	(0x09, sp), x
	clr	(0x0b, sp)
;	lib\modbus\mb-process.c: 184: mb_packet_response_write_multiple_registers(&tmpPacket,Packet->u16_1,Packet->u16_2);
	ldw	x, (0x0e, sp)
	ldw	x, (x)
	ldw	y, (0x0c, sp)
	ldw	y, (y)
	pushw	x
	pushw	y
	ldw	x, sp
	addw	x, #5
	call	_mb_packet_response_write_multiple_registers
;	lib\modbus\mb-process.c: 186: if(Packet->device_address != MB_BROADCAST_ADDRESS)
	ldw	x, (0x1c, sp)
	ld	a, (x)
	jreq	00103$
;	lib\modbus\mb-process.c: 188: mb_tx_packet_handler(&tmpPacket);
	ldw	x, sp
	incw	x
	call	_mb_tx_packet_handler
00103$:
;	lib\modbus\mb-process.c: 189: return MB_OK;
	clr	a
;	lib\modbus\mb-process.c: 190: }
	addw	sp, #29
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
