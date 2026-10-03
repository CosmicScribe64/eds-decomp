	thumb_func_start TextBoxDrawChoiceCursor
TextBoxDrawChoiceCursor: @ 0x0805FBA4
	push {r4, r5, lr}
	ldr r3, _0805FBF8 @ =0x0201AE60
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
	bne _0805FC00
	add r1, r3, #0
	add r1, #0x23
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805FC0C
	lsl r0, r2, #0x10
	orr r0, r4
	ldr r2, _0805FBFC @ =0x0000431F
	mov r1, #0
	bl AddSprite
	b _0805FC0C
_0805FBF8: .4byte 0x0201AE60
_0805FBFC: .4byte 0x0000431F
_0805FC00:
	lsl r0, r2, #0x10
	orr r0, r4
	ldr r2, _0805FC14 @ =0x0000431F
	mov r1, #0
	bl AddSprite
_0805FC0C:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0805FC14: .4byte 0x0000431F
	thumb_func_end TextBoxDrawChoiceCursor

