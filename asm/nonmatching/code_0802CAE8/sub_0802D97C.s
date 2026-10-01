	thumb_func_start sub_0802D97C
sub_0802D97C: @ 0x0802D97C
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r3, r1, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	beq _0802D98A
	b _0802DABC
_0802D98A:
	cmp r3, #0
	bne _0802D990
	b _0802DABC
_0802D990:
	mov r0, #1
	ldrb r2, [r5, #2]
	add r1, r0, #0
	ldrb r4, [r3, #2]
	and r1, r4
	and r0, r2
	add r6, r2, #0
	cmp r1, r0
	bne _0802D9A4
	b _0802DABC
_0802D9A4:
	ldr r0, _0802D9CC @ =0x000007FF
	ldrh r1, [r3]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0802D9D0 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0802D9D4 @ =0x00000426
	cmp r1, r0
	bgt _0802D9EC
	sub r0, #1
	cmp r1, r0
	bge _0802DA74
	cmp r1, #0xDF
	beq _0802DA10
	cmp r1, #0xDF
	bgt _0802D9D8
	cmp r1, #0x53
	beq _0802DA10
	b _0802DABC
_0802D9CC: .4byte 0x000007FF
_0802D9D0: .4byte gUnk_08622AB4
_0802D9D4: .4byte 0x00000426
_0802D9D8:
	ldr r0, _0802D9E8 @ =0x0000029F
	cmp r1, r0
	beq _0802DA74
	mov r0, #0xFB
	lsl r0, r0, #2
	cmp r1, r0
	beq _0802DA10
	b _0802DABC
_0802D9E8: .4byte 0x0000029F
_0802D9EC:
	ldr r0, _0802DA00 @ =0x00000437
	cmp r1, r0
	beq _0802DA10
	cmp r1, r0
	bgt _0802DA04
	sub r0, #0xC
	cmp r1, r0
	beq _0802DA74
	b _0802DABC
	.align 2, 0
_0802DA00: .4byte 0x00000437
_0802DA04:
	ldr r0, _0802DA60 @ =0x0000046F
	cmp r1, r0
	bgt _0802DABC
	sub r0, #1
	cmp r1, r0
	blt _0802DABC
_0802DA10:
	ldrb r4, [r3, #0xC]
	ldrh r3, [r3, #0xC]
	lsr r3, r3, #8
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _0802DA64 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802DA68 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0802DABC
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	cmp r4, r0
	bne _0802DA44
	ldrh r5, [r5, #2]
	lsl r0, r5, #0x16
	lsr r0, r0, #0x1A
	cmp r3, r0
	beq _0802DABC
_0802DA44:
	ldr r0, _0802DA6C @ =0x000007FF
	and r1, r0
	lsl r0, r1, #2
	ldr r4, _0802DA70 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0802DABC
_0802DA5C:
	mov r0, #1
	b _0802DABE
_0802DA60: .4byte 0x0000046F
_0802DA64: .4byte 0x00000D64
_0802DA68: .4byte 0x0201930C
_0802DA6C: .4byte 0x000007FF
_0802DA70: .4byte gUnk_08621DE0
_0802DA74:
	mov r3, #5
	ldr r7, _0802DAC4 @ =0x0201930C
	lsl r4, r6, #0x1F
	ldr r6, _0802DAC8 @ =0x000007FF
_0802DA7C:
	lsr r2, r4, #0x1F
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0802DACC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r1, r1, r7
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0802DAB6
	ldrh r2, [r5, #2]
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1A
	cmp r3, r0
	beq _0802DAB6
	and r1, r6
	lsl r0, r1, #2
	ldr r1, _0802DAD0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _0802DA5C
_0802DAB6:
	add r3, #1
	cmp r3, #9
	ble _0802DA7C
_0802DABC:
	mov r0, #0
_0802DABE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802DAC4: .4byte 0x0201930C
_0802DAC8: .4byte 0x000007FF
_0802DACC: .4byte 0x00000D64
_0802DAD0: .4byte gUnk_08621DE0
	thumb_func_end sub_0802D97C

