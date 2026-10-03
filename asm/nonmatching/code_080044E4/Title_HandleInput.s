	thumb_func_start Title_HandleInput
Title_HandleInput: @ 0x0800548C
	push {lr}
	bl Title_DrawMenu
	ldr r1, _080054CC @ =0x03000040
	mov r0, #0x30
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080054DA
	ldr r2, _080054D0 @ =0x0201527C
	ldrb r3, [r2, #2]
	mov r0, #1
	and r0, r3
	cmp r0, #0
	beq _080054D4
	ldr r0, [r2]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2, #2]
	mov r0, #0
	bl PlaySE
	b _080054DA
_080054CC: .4byte 0x03000040
_080054D0: .4byte 0x0201527C
_080054D4:
	mov r0, #3
	bl PlaySE
_080054DA:
	ldr r1, _080054EC @ =0x03000040
	mov r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _080054F0
	mov r0, #0
	b _080054FC
	.align 2, 0
_080054EC: .4byte 0x03000040
_080054F0:
	mov r0, #1
	bl PlaySE
	bl FadeOutBGM
	mov r0, #1
_080054FC:
	pop {r1}
	bx r1
	thumb_func_end Title_HandleInput

