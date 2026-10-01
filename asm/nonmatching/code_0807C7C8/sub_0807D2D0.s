	thumb_func_start sub_0807D2D0
sub_0807D2D0: @ 0x0807D2D0
	push {r4, lr}
	ldr r3, _0807D30C @ =0x03000040
	ldr r0, _0807D310 @ =0x0000485A
	add r4, r3, r0
	ldrb r0, [r4]
	cmp r0, #0
	beq _0807D31C
	cmp r0, #1
	beq _0807D330
	ldr r2, _0807D314 @ =0x0201F780
	mov r0, #0
	strh r0, [r2]
	sub r0, #3
	ldrb r1, [r2, #2]
	and r0, r1
	mov r1, #2
	neg r1, r1
	and r0, r1
	strb r0, [r2, #2]
	mov r0, #0x1F
	neg r0, r0
	ldrb r1, [r2, #3]
	and r0, r1
	strb r0, [r2, #3]
	ldr r0, _0807D318 @ =0x00004859
	add r1, r3, r0
	mov r0, #1
	strb r0, [r1]
	b _0807D340
	.align 2, 0
_0807D30C: .4byte 0x03000040
_0807D310: .4byte 0x0000485A
_0807D314: .4byte 0x0201F780
_0807D318: .4byte 0x00004859
_0807D31C:
	ldr r0, _0807D32C @ =0x0201F780
	ldrh r0, [r0, #0x20]
	mov r1, #0
	mov r2, #0
	bl sub_0800688C
	b _0807D33A
	.align 2, 0
_0807D32C: .4byte 0x0201F780
_0807D330:
	bl sub_08006D08
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807D340
_0807D33A:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0807D340:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0807D2D0

