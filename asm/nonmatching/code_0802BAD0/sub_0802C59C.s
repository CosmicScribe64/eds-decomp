	thumb_func_start sub_0802C59C
sub_0802C59C: @ 0x0802C59C
	push {r4, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r3, r0, #0x18
	lsr r1, r1, #0x18
	cmp r1, #4
	ble _0802C5DC
	mov r2, #1
	and r2, r3
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0802C5D4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802C5D8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802C5DC
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r3
	beq _0802C5DC
	mov r0, #1
	b _0802C5DE
_0802C5D4: .4byte 0x00000D64
_0802C5D8: .4byte 0x0201930C
_0802C5DC:
	mov r0, #0
_0802C5DE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802C59C

