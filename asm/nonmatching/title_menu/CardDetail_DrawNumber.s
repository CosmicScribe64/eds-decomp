	thumb_func_start CardDetail_DrawNumber
CardDetail_DrawNumber: @ 0x080063D0
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r5, r2, #0
	add r6, #0x14
	cmp r5, #0
	bne _080063F0
	lsl r0, r1, #0x10
	orr r0, r6
	ldr r2, _080063EC @ =0x00003030
	mov r1, #0
	bl AddSprite
	b _08006420
	.align 2, 0
_080063EC: .4byte 0x00003030
_080063F0:
	lsl r7, r1, #0x10
_080063F2:
	add r4, r6, #0
	orr r4, r7
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	ldr r0, _08006428 @ =0x00003030
	add r2, r2, r0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0
	bl AddSprite
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	sub r6, #4
	cmp r5, #0
	bne _080063F2
_08006420:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08006428: .4byte 0x00003030
	thumb_func_end CardDetail_DrawNumber

