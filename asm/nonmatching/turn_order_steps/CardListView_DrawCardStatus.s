	thumb_func_start CardListView_DrawCardStatus
CardListView_DrawCardStatus: @ 0x0802A4CC
	push {r4, r5, lr}
	add r5, r0, #0
	mov r4, #8
	ldr r0, [r5]
	lsl r0, r0, #0xC
	cmp r0, #0
	bge _0802A4EE
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r4, r0
	mov r2, #0x88
	lsl r2, r2, #5
	add r0, r4, #0
	mov r1, #0x40
	bl AddSprite
	mov r4, #0x18
_0802A4EE:
	ldr r1, _0802A520 @ =0x0201D810
	mov r0, #0xE0
	ldrb r2, [r1]
	and r0, r2
	cmp r0, #0x80
	bne _0802A558
	ldrb r2, [r1, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r2, [r1, #6]
	add r0, r2, r0
	lsl r0, r0, #1
	mov r2, #0x83
	lsl r2, r2, #2
	add r1, r1, r2
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #2
	beq _0802A538
	cmp r1, #2
	bgt _0802A524
	cmp r1, #1
	beq _0802A52A
	b _0802A558
	.align 2, 0
_0802A520: .4byte 0x0201D810
_0802A524:
	cmp r1, #4
	beq _0802A548
	b _0802A558
_0802A52A:
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r0, r4
	ldr r2, _0802A534 @ =0x00001106
	b _0802A550
_0802A534: .4byte 0x00001106
_0802A538:
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r0, r4
	ldr r2, _0802A544 @ =0x00001108
	b _0802A550
	.align 2, 0
_0802A544: .4byte 0x00001108
_0802A548:
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r0, r4
	ldr r2, _0802A604 @ =0x0000110A
_0802A550:
	mov r1, #0x40
	bl AddSprite
	add r4, #0x10
_0802A558:
	ldr r0, [r5]
	lsl r2, r0, #0x14
	lsl r0, r2, #1
	lsr r0, r0, #0x13
	ldr r1, _0802A608 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0802A5CE
	ldr r1, _0802A60C @ =0x0201D810
	mov r0, #0xE0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0x40
	beq _0802A5CE
	lsr r0, r2, #0x14
	bl IsSpecialSummonOnly
	cmp r0, #0
	beq _0802A5A2
	mov r0, #0xC0
	ldrb r2, [r5, #1]
	and r0, r2
	cmp r0, #0
	bne _0802A5A2
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r0, r4
	ldr r2, _0802A610 @ =0x00001102
	mov r1, #0x40
	bl AddSprite
	add r4, #0x10
_0802A5A2:
	ldr r0, _0802A60C @ =0x0201D810
	mov r1, #0xE0
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0x60
	beq _0802A5B6
	cmp r1, #0
	beq _0802A5B6
	cmp r1, #0x20
	bne _0802A5CE
_0802A5B6:
	ldr r0, [r5]
	lsl r0, r0, #0xB
	cmp r0, #0
	bge _0802A5CE
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r0, r4
	ldr r2, _0802A614 @ =0x0000110C
	mov r1, #0x40
	bl AddSprite
	add r4, #0x10
_0802A5CE:
	ldr r1, _0802A60C @ =0x0201D810
	ldrb r3, [r1]
	mov r0, #0xE0
	and r0, r3
	cmp r0, #0x60
	bne _0802A64C
	lsl r3, r3, #0x1E
	ldrb r2, [r1, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r1, [r1, #6]
	add r0, r1, r0
	ldr r2, _0802A618 @ =0x020192E4
	lsl r0, r0, #1
	lsr r3, r3, #0x1F
	ldr r1, _0802A61C @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r1, _0802A620 @ =0x00000CC4
	add r2, r2, r1
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #1
	beq _0802A624
	cmp r0, #2
	beq _0802A63C
	b _0802A64C
_0802A604: .4byte 0x0000110A
_0802A608: .4byte gCardStats
_0802A60C: .4byte 0x0201D810
_0802A610: .4byte 0x00001102
_0802A614: .4byte 0x0000110C
_0802A618: .4byte 0x020192E4
_0802A61C: .4byte 0x00000D64
_0802A620: .4byte 0x00000CC4
_0802A624:
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r4, r0
	ldr r2, _0802A638 @ =0x00001110
	add r0, r4, #0
	mov r1, #0x40
	bl AddSprite
	b _0802A64C
	.align 2, 0
_0802A638: .4byte 0x00001110
_0802A63C:
	mov r0, #0x88
	lsl r0, r0, #0x10
	orr r4, r0
	ldr r2, _0802A654 @ =0x0000110E
	add r0, r4, #0
	mov r1, #0x40
	bl AddSprite
_0802A64C:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802A654: .4byte 0x0000110E
	thumb_func_end CardListView_DrawCardStatus

