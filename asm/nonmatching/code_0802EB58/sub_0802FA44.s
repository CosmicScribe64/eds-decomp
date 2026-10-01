	thumb_func_start sub_0802FA44
sub_0802FA44: @ 0x0802FA44
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	mov r5, #0
	mov r7, #0
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	beq _0802FB04
	mov r4, #0
	ldr r3, _0802FABC @ =0x020192E4
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0802FAC0 @ =0x00000D64
	mul r0, r2
	add r0, r0, r3
	ldrb r0, [r0, #2]
	cmp r5, r0
	bge _0802FAE6
	mov r8, r3
_0802FA7E:
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r4, #2
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802FAC4 @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r0, _0802FAC8 @ =0x000007FF
	add r1, r0, #0
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802FACC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0802FAD0
	add r0, r2, #0
	bl sub_08007834
	cmp r0, #0
	bne _0802FAD2
	add r7, #1
	b _0802FAD2
_0802FABC: .4byte 0x020192E4
_0802FAC0: .4byte 0x00000D64
_0802FAC4: .4byte 0x02019968
_0802FAC8: .4byte 0x000007FF
_0802FACC: .4byte gUnk_08621DE0
_0802FAD0:
	add r5, #1
_0802FAD2:
	add r4, #1
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0802FB00 @ =0x00000D64
	mul r0, r2
	add r0, r8
	ldrb r0, [r0, #2]
	cmp r4, r0
	blt _0802FA7E
_0802FAE6:
	mov r0, r9
	cmp r0, #0
	beq _0802FAF2
	cmp r5, #0
	beq _0802FAF2
	sub r5, #1
_0802FAF2:
	cmp r7, #0
	beq _0802FB04
	cmp r5, #1
	ble _0802FB04
	mov r0, #1
	b _0802FB06
	.align 2, 0
_0802FB00: .4byte 0x00000D64
_0802FB04:
	mov r0, #0
_0802FB06:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802FA44
	.align 2, 0

