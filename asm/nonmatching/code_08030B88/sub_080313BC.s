	thumb_func_start sub_080313BC
sub_080313BC: @ 0x080313BC
	push {r4, lr}
	add r3, r0, #0
	mov r2, #7
	ldrb r0, [r3, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08031400
	ldrh r0, [r3, #0xC]
	and r2, r0
	lsr r0, r0, #8
	mov r1, #0x94
	mul r0, r1
	ldr r1, _08031408 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803140C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08031400
	ldrb r4, [r3, #2]
	lsl r1, r4, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	ldrh r4, [r3, #2]
	lsl r2, r4, #0x16
	lsr r2, r2, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	ldrh r2, [r3, #0xC]
	mov r3, #2
	bl sub_08017AB4
_08031400:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08031408: .4byte 0x00000D64
_0803140C: .4byte 0x0201930C
	thumb_func_end sub_080313BC

