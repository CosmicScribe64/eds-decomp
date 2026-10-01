	thumb_func_start sub_0804F310
sub_0804F310: @ 0x0804F310
	push {r4, r5, lr}
	ldr r3, _0804F364 @ =0x0201AE60
	ldrh r0, [r3, #8]
	add r0, #1
	lsl r4, r0, #3
	ldrh r1, [r3, #0xA]
	add r2, r1, #2
	lsl r2, r2, #3
	ldrh r5, [r3, #0x14]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #2
	add r2, r2, r0
	sub r2, #2
	ldrh r0, [r3, #0xE]
	add r1, r0, r1
	add r0, r3, #0
	add r0, #0x21
	ldrb r0, [r0]
	sub r1, r1, r0
	add r1, #2
	lsl r1, r1, #3
	sub r2, r2, r1
	add r0, r3, #0
	add r0, #0x22
	ldrb r0, [r0]
	cmp r0, #1
	bne _0804F36C
	add r1, r3, #0
	add r1, #0x23
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0804F378
	lsl r0, r2, #0x10
	orr r0, r4
	ldr r2, _0804F368 @ =0x0000431F
	mov r1, #0
	bl sub_080761F0
	b _0804F378
_0804F364: .4byte 0x0201AE60
_0804F368: .4byte 0x0000431F
_0804F36C:
	lsl r0, r2, #0x10
	orr r0, r4
	ldr r2, _0804F380 @ =0x0000431F
	mov r1, #0
	bl sub_080761F0
_0804F378:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0804F380: .4byte 0x0000431F
	thumb_func_end sub_0804F310

