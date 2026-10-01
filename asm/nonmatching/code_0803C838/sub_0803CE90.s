	thumb_func_start sub_0803CE90
sub_0803CE90: @ 0x0803CE90
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x10
	lsr r6, r3, #0x10
	mov r4, #0
	ldr r0, _0803D028 @ =0x0201930C
	mov ip, r0
	mov r0, #1
	mov r1, r8
	and r0, r1
	ldr r1, _0803D02C @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	ldr r2, _0803D030 @ =0x000007FF
	mov r9, r2
	mov r0, #0xFA
	lsl r0, r0, #3
	add r1, r5, r0
_0803CEC2:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r3
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0803CEFE
	mov r0, r9
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _0803D034 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	cmp r2, r5
	beq _0803CEEA
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803CEFE
_0803CEEA:
	mov r2, #0x80
	lsl r2, r2, #7
	add r0, r4, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r7, r0
	beq _0803CEFE
	cmp r6, r0
	beq _0803CEFE
	b _0803D01C
_0803CEFE:
	add r4, #1
	cmp r4, #4
	ble _0803CEC2
	mov r4, #0
	ldr r0, _0803D038 @ =0x020192E4
	mov r1, #1
	mov r2, r8
	and r1, r2
	ldr r2, _0803D02C @ =0x00000D64
	mul r1, r2
	add r0, r1, r0
	ldrb r0, [r0, #2]
	cmp r4, r0
	bge _0803CF5E
	ldr r2, _0803D030 @ =0x000007FF
	mov r9, r2
	sub r2, #0x2F
	add r2, r2, r5
	mov ip, r2
	add r3, r0, #0
_0803CF26:
	ldr r0, _0803D03C @ =0x02019968
	add r0, r1, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	mov r0, r9
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _0803D034 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	cmp r2, r5
	beq _0803CF46
	ldrh r0, [r0]
	cmp r0, ip
	bne _0803CF56
_0803CF46:
	ldr r2, _0803D040 @ =0xFFFF8000
	add r0, r4, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r7, r0
	beq _0803CF56
	cmp r6, r0
	bne _0803D01C
_0803CF56:
	add r1, #4
	add r4, #1
	cmp r4, r3
	blt _0803CF26
_0803CF5E:
	mov r4, #0
	mov r0, #1
	mov r1, r8
	and r0, r1
	ldr r1, _0803D02C @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
_0803CF6C:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r5
	ldr r1, _0803D028 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0803CFAA
	ldr r1, _0803D030 @ =0x000007FF
	add r0, r1, #0
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _0803D034 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CFAA
	mov r1, #0x80
	lsl r1, r1, #7
	add r0, r4, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r7, r0
	beq _0803CFAA
	cmp r6, r0
	bne _0803D01C
_0803CFAA:
	add r4, #1
	cmp r4, #4
	ble _0803CF6C
	mov r4, #0
	ldr r1, _0803D038 @ =0x020192E4
	mov r2, #1
	mov r0, r8
	and r2, r0
	ldr r3, _0803D02C @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r4, r0
	bge _0803D01A
	add r5, r2, #0
_0803CFCA:
	lsl r1, r4, #2
	add r0, r5, #0
	mul r0, r3
	add r1, r1, r0
	ldr r0, _0803D03C @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0803D008
	ldr r1, _0803D030 @ =0x000007FF
	add r0, r1, #0
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _0803D034 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D008
	ldr r1, _0803D040 @ =0xFFFF8000
	add r0, r4, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r7, r0
	beq _0803D008
	cmp r6, r0
	bne _0803D01C
_0803D008:
	add r4, #1
	ldr r1, _0803D038 @ =0x020192E4
	ldr r3, _0803D02C @ =0x00000D64
	add r0, r5, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r4, r0
	blt _0803CFCA
_0803D01A:
	ldr r0, _0803D044 @ =0x0000FFFF
_0803D01C:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803D028: .4byte 0x0201930C
_0803D02C: .4byte 0x00000D64
_0803D030: .4byte 0x000007FF
_0803D034: .4byte gUnk_08622AB4
_0803D038: .4byte 0x020192E4
_0803D03C: .4byte 0x02019968
_0803D040: .4byte 0xFFFF8000
_0803D044: .4byte 0x0000FFFF
	thumb_func_end sub_0803CE90

