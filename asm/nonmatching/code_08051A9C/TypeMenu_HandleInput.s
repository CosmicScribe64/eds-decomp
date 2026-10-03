	thumb_func_start TypeMenu_HandleInput
TypeMenu_HandleInput: @ 0x080522C0
	push {r4, r5, r6, lr}
	ldr r5, _080522E0 @ =0x0201AE60
	add r6, r5, #0
	add r6, #0x23
	ldrb r0, [r6]
	cmp r0, #0
	beq _080522DC
	add r1, r5, #0
	add r1, #0x22
	ldrb r0, [r1]
	cmp r0, #0
	bne _080522E4
	add r0, #1
	strb r0, [r1]
_080522DC:
	mov r0, #0
	b _08052324
_080522E0: .4byte 0x0201AE60
_080522E4:
	ldr r0, _080522F8 @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	beq _080522FC
	ldrh r0, [r5, #0x14]
	add r0, #0x13
	b _08052308
	.align 2, 0
_080522F8: .4byte 0x03000040
_080522FC:
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	beq _0805231A
	ldrh r0, [r5, #0x14]
	add r0, #1
_08052308:
	mov r4, #0
	strh r0, [r5, #0x14]
	ldrh r0, [r5, #0x14]
	mov r1, #0x14
	bl __umodsi3
	strh r0, [r5, #0x14]
	strb r4, [r6]
	b _080522DC
_0805231A:
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _080522DC
	mov r0, #1
_08052324:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end TypeMenu_HandleInput
	.align 2, 0

