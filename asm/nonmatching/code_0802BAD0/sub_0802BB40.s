	thumb_func_start sub_0802BB40
sub_0802BB40: @ 0x0802BB40
	push {r4, r5, lr}
	add r2, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r5, r0, #0x18
	lsr r4, r1, #0x18
	cmp r4, #4
	bgt _0802BB60
	ldrh r0, [r2]
	add r1, r5, #0
	add r2, r4, #0
	bl sub_0802B1B8
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802BB64
_0802BB60:
	mov r0, #0
	b _0802BB84
_0802BB64:
	mov r3, #0
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	mul r0, r4
	ldr r1, _0802BB8C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802BB90 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BB82
	mov r3, #1
_0802BB82:
	add r0, r3, #0
_0802BB84:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802BB8C: .4byte 0x00000D64
_0802BB90: .4byte 0x0201930C
	thumb_func_end sub_0802BB40

