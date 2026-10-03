	thumb_func_start AttributeMenu_HandleInput
AttributeMenu_HandleInput: @ 0x0805243C
	push {r4, r5, r6, lr}
	ldr r5, _0805245C @ =0x0201AE60
	add r6, r5, #0
	add r6, #0x23
	ldrb r0, [r6]
	cmp r0, #0
	beq _08052458
	add r1, r5, #0
	add r1, #0x22
	ldrb r0, [r1]
	cmp r0, #0
	bne _08052460
	add r0, #1
	strb r0, [r1]
_08052458:
	mov r0, #0
	b _080524A0
_0805245C: .4byte 0x0201AE60
_08052460:
	ldr r0, _08052474 @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	beq _08052478
	ldrh r0, [r5, #0x14]
	add r0, #5
	b _08052484
	.align 2, 0
_08052474: .4byte 0x03000040
_08052478:
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	beq _08052496
	ldrh r0, [r5, #0x14]
	add r0, #1
_08052484:
	mov r4, #0
	strh r0, [r5, #0x14]
	ldrh r0, [r5, #0x14]
	mov r1, #6
	bl __umodsi3
	strh r0, [r5, #0x14]
	strb r4, [r6]
	b _08052458
_08052496:
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _08052458
	mov r0, #1
_080524A0:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end AttributeMenu_HandleInput
	.align 2, 0

