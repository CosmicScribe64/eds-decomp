	thumb_func_start sub_0802FB8C
sub_0802FB8C: @ 0x0802FB8C
	push {r4, lr}
	add r4, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802FBEC
	cmp r1, #0
	bne _0802FBEC
	mov r0, #0xFC
	ldrb r1, [r4, #3]
	and r0, r1
	cmp r0, #0x40
	bne _0802FBEC
	ldrb r3, [r4, #8]
	ldrh r0, [r4, #8]
	lsr r1, r0, #8
	mov r2, #1
	and r2, r3
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0802FBE0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802FBE4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802FBEC
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	cmp r3, r0
	bne _0802FBEC
	ldr r1, _0802FBE8 @ =0x0000057D
	add r0, r3, #0
	bl sub_080086CC
	cmp r0, #0
	ble _0802FBEC
	mov r0, #1
	b _0802FBEE
	.align 2, 0
_0802FBE0: .4byte 0x00000D64
_0802FBE4: .4byte 0x0201930C
_0802FBE8: .4byte 0x0000057D
_0802FBEC:
	mov r0, #0
_0802FBEE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802FB8C

