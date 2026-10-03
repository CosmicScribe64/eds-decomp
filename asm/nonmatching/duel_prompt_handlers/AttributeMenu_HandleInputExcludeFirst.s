	thumb_func_start AttributeMenu_HandleInputExcludeFirst
AttributeMenu_HandleInputExcludeFirst: @ 0x080524A8
	push {r4, r5, r6, lr}
	ldr r2, _080524C8 @ =0x0201AE60
	add r0, r2, #0
	add r0, #0x23
	ldrb r0, [r0]
	add r6, r2, #0
	cmp r0, #0
	beq _08052558
	add r1, r2, #0
	add r1, #0x22
	ldrb r0, [r1]
	cmp r0, #0
	bne _080524CC
	add r0, #1
	strb r0, [r1]
	b _08052558
_080524C8: .4byte 0x0201AE60
_080524CC:
	ldr r0, _080524F8 @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	beq _08052500
	add r4, r2, #0
	ldr r0, _080524FC @ =0x0201AE44
	ldrh r5, [r0]
_080524DE:
	ldrh r0, [r4, #0x14]
	add r0, #5
	strh r0, [r4, #0x14]
	ldrh r0, [r4, #0x14]
	mov r1, #6
	bl __umodsi3
	strh r0, [r4, #0x14]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, r5
	beq _080524DE
	b _08052526
_080524F8: .4byte 0x03000040
_080524FC: .4byte 0x0201AE44
_08052500:
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	beq _08052534
	add r4, r2, #0
	ldr r0, _08052530 @ =0x0201AE44
	ldrh r5, [r0]
_0805250E:
	ldrh r0, [r4, #0x14]
	add r0, #1
	strh r0, [r4, #0x14]
	ldrh r0, [r4, #0x14]
	mov r1, #6
	bl __umodsi3
	strh r0, [r4, #0x14]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, r5
	beq _0805250E
_08052526:
	add r1, r6, #0
	add r1, #0x23
	mov r0, #0
	strb r0, [r1]
	b _0805255A
_08052530: .4byte 0x0201AE44
_08052534:
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _08052558
	ldr r0, _08052550 @ =0x020192E0
	ldr r1, _08052554 @ =0x00001B64
	add r0, r0, r1
	ldrh r2, [r2, #0x14]
	ldrh r0, [r0]
	cmp r2, r0
	beq _08052558
	mov r0, #1
	b _0805255A
	.align 2, 0
_08052550: .4byte 0x020192E0
_08052554: .4byte 0x00001B64
_08052558:
	mov r0, #0
_0805255A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end AttributeMenu_HandleInputExcludeFirst

