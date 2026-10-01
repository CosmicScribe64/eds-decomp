	thumb_func_start sub_0802DAD4
sub_0802DAD4: @ 0x0802DAD4
	push {r4, r5, lr}
	lsl r2, r2, #0x10
	ldrb r3, [r0, #2]
	lsl r1, r3, #0x1F
	lsr r4, r1, #0x1F
	ldrh r0, [r0, #2]
	lsl r0, r0, #0x16
	lsr r5, r0, #0x1A
	cmp r2, #0
	bne _0802DB28
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0800A430
	ldr r1, _0802DB1C @ =0x0000FFFF
	cmp r0, r1
	bne _0802DB28
	add r0, r4, #0
	bl sub_08008C6C
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _0802DB28
	mov r0, #0x94
	mul r0, r5
	ldr r1, _0802DB20 @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	ldr r1, _0802DB24 @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #7]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1F
	b _0802DB2A
	.align 2, 0
_0802DB1C: .4byte 0x0000FFFF
_0802DB20: .4byte 0x00000D64
_0802DB24: .4byte 0x0201930C
_0802DB28:
	mov r0, #0
_0802DB2A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802DAD4

