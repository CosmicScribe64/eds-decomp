	thumb_func_start sub_080073BC
sub_080073BC: @ 0x080073BC
	push {r4, r5, lr}
	ldr r5, _080073F8 @ =0x03000040
	ldr r0, _080073FC @ =0x0000485B
	add r2, r5, r0
	mov r0, #1
	strb r0, [r2]
	ldr r1, _08007400 @ =0x08198D5C
	ldr r3, _08007404 @ =0x00004859
	add r4, r5, r3
	ldrb r3, [r4]
	lsl r0, r3, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0800740C
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080073F2
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	ldr r0, _08007408 @ =0x0000485A
	add r1, r5, r0
	mov r0, #0
	strb r0, [r1]
_080073F2:
	mov r0, #0
	b _08007410
	.align 2, 0
_080073F8: .4byte 0x03000040
_080073FC: .4byte 0x0000485B
_08007400: .4byte gUnk_08198D5C
_08007404: .4byte 0x00004859
_08007408: .4byte 0x0000485A
_0800740C:
	strb r0, [r2]
	mov r0, #1
_08007410:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_080073BC
	.align 2, 0

