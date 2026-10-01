	thumb_func_start sub_080657F8
sub_080657F8: @ 0x080657F8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r6, _08065854 @ =0x0201DB20
	ldr r1, _08065858 @ =0x00001C1C
	add r0, r6, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r6, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r5, #0xC4
	lsl r5, r5, #3
	add r3, r6, r5
	add r2, r2, r3
	ldrh r2, [r2]
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	ldr r0, _0806585C @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08065860 @ =0x08621DE0
	add r5, r0, r1
	ldr r0, [r5]
	mov r2, #0xF8
	lsl r2, r2, #0x11
	mov r8, r2
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080658B4
	cmp r0, #0x16
	bgt _08065864
	cmp r0, #0x15
	beq _08065870
	b _0806596C
	.align 2, 0
_08065854: .4byte 0x0201DB20
_08065858: .4byte 0x00001C1C
_0806585C: .4byte 0x000007FF
_08065860: .4byte gUnk_08621DE0
_08065864:
	cmp r0, #0x17
	bne _0806586A
	b _08065A8E
_0806586A:
	cmp r0, #0x18
	beq _08065938
	b _0806596C
_08065870:
	ldr r2, _080658AC @ =0x0600C000
	add r4, #7
	lsl r0, r4, #3
	ldr r3, _080658B0 @ =0x0000063E
	add r1, r6, r3
	ldrh r1, [r1]
	add r0, r1, r0
	mov r1, #0xFF
	and r0, r1
	lsr r0, r0, #3
	str r0, [sp, #0]
	mov r0, #6
	str r0, [sp, #4]
	add r0, #0xFA
	str r0, [sp, #8]
	mov r0, #0
	mov r1, #9
	mov r3, #4
	bl sub_080656B4
	ldr r1, [r5]
	add r0, r1, #0
	mov r5, r8
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08065900
	cmp r0, #0x15
	bge _080658EE
	b _08065900
_080658AC: .4byte 0x0600C000
_080658B0: .4byte 0x0000063E
_080658B4:
	ldr r2, _080658F8 @ =0x0600C000
	add r4, #7
	lsl r0, r4, #3
	ldr r3, _080658FC @ =0x0000063E
	add r1, r6, r3
	ldrh r1, [r1]
	add r0, r1, r0
	mov r1, #0xFF
	and r0, r1
	lsr r0, r0, #3
	str r0, [sp, #0]
	mov r0, #6
	str r0, [sp, #4]
	add r0, #0xFA
	str r0, [sp, #8]
	mov r0, #0
	mov r1, #8
	mov r3, #4
	bl sub_080656B4
	ldr r1, [r5]
	add r0, r1, #0
	mov r5, r8
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08065900
	cmp r0, #0x15
	blt _08065900
_080658EE:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r1, r1, #0x11
	b _08065902
_080658F8: .4byte 0x0600C000
_080658FC: .4byte 0x0000063E
_08065900:
	mov r1, #0
_08065902:
	ldr r2, _0806592C @ =0x0600C000
	lsl r3, r4, #3
	ldr r0, _08065930 @ =0x0201DB20
	ldr r4, _08065934 @ =0x0000063E
	add r0, r0, r4
	ldrh r0, [r0]
	add r3, r0, r3
	mov r0, #0xFF
	and r3, r0
	lsr r3, r3, #3
	str r3, [sp, #0]
	mov r0, #7
	str r0, [sp, #4]
	add r0, #0xF9
	str r0, [sp, #8]
	mov r0, #2
	mov r3, #6
	bl sub_080656B4
	b _08065A8E
	.align 2, 0
_0806592C: .4byte 0x0600C000
_08065930: .4byte 0x0201DB20
_08065934: .4byte 0x0000063E
_08065938:
	ldr r2, _08065964 @ =0x0600C000
	add r0, r4, #7
	lsl r0, r0, #3
	ldr r5, _08065968 @ =0x0000063E
	add r1, r6, r5
	ldrh r1, [r1]
	add r0, r1, r0
	mov r1, #0xFF
	and r0, r1
	lsr r0, r0, #3
	str r0, [sp, #0]
	mov r0, #6
	str r0, [sp, #4]
	add r0, #0xFA
	str r0, [sp, #8]
	mov r0, #0
	mov r1, #0xA
	mov r3, #4
	bl sub_080656B4
	b _08065A8E
	.align 2, 0
_08065964: .4byte 0x0600C000
_08065968: .4byte 0x0000063E
_0806596C:
	ldr r5, _080659E8 @ =0x000007FF
	and r5, r7
	lsl r0, r5, #2
	mov r8, r0
	ldr r1, _080659EC @ =0x08621DE0
	add r8, r1
	mov r2, r8
	ldr r1, [r2]
	lsr r1, r1, #0x1D
	add r4, #7
	mov sl, r4
	lsl r4, r4, #3
	ldr r6, _080659F0 @ =0x0201DB20
	ldr r3, _080659F4 @ =0x0000063E
	add r6, r6, r3
	ldrh r2, [r6]
	add r0, r2, r4
	mov r3, #0xFF
	and r0, r3
	lsr r0, r0, #3
	str r0, [sp, #0]
	mov r0, #6
	str r0, [sp, #4]
	add r0, #0xFA
	mov r9, r0
	str r0, [sp, #8]
	mov r0, #0
	ldr r2, _080659F8 @ =0x0600C000
	mov r3, #4
	bl sub_080656B4
	mov r2, r8
	ldr r1, [r2]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r1, r0
	lsr r1, r1, #0x14
	ldrh r6, [r6]
	add r4, r6, r4
	mov r3, #0xFF
	and r4, r3
	lsr r4, r4, #3
	str r4, [sp, #0]
	mov r0, #7
	str r0, [sp, #4]
	mov r4, r9
	str r4, [sp, #8]
	mov r0, #1
	ldr r2, _080659F8 @ =0x0600C000
	mov r3, #6
	bl sub_080656B4
	lsl r5, r5, #1
	ldr r0, _080659FC @ =0x08622AB4
	add r5, r5, r0
	ldrh r1, [r5]
	ldr r0, _08065A00 @ =0x00000776
	mov r4, sl
	cmp r1, r0
	bne _08065A04
	mov r0, #3
	b _08065A66
_080659E8: .4byte 0x000007FF
_080659EC: .4byte gUnk_08621DE0
_080659F0: .4byte 0x0201DB20
_080659F4: .4byte 0x0000063E
_080659F8: .4byte 0x0600C000
_080659FC: .4byte gUnk_08622AB4
_08065A00: .4byte 0x00000776
_08065A04:
	cmp r1, r0
	blt _08065A14
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08065A14
	mov r0, #1
	b _08065A66
_08065A14:
	ldr r0, _08065A38 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08065A3C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08065A46
	cmp r0, #0x16
	bgt _08065A40
	cmp r0, #0x15
	beq _08065A4A
	b _08065A52
	.align 2, 0
_08065A38: .4byte 0x000007FF
_08065A3C: .4byte gUnk_08621DE0
_08065A40:
	cmp r0, #0x17
	beq _08065A4E
	b _08065A52
_08065A46:
	mov r0, #7
	b _08065A66
_08065A4A:
	mov r0, #8
	b _08065A66
_08065A4E:
	mov r0, #9
	b _08065A66
_08065A52:
	ldr r0, _08065AA0 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r2, _08065AA4 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08065A66:
	add r1, r0, #0
	ldr r2, _08065AA8 @ =0x0600C000
	lsl r3, r4, #3
	ldr r0, _08065AAC @ =0x0201DB20
	ldr r4, _08065AB0 @ =0x0000063E
	add r0, r0, r4
	ldrh r0, [r0]
	add r3, r0, r3
	mov r0, #0xFF
	and r3, r0
	lsr r3, r3, #3
	str r3, [sp, #0]
	mov r0, #1
	str r0, [sp, #4]
	add r0, #0xFF
	str r0, [sp, #8]
	mov r0, #3
	mov r3, #8
	bl sub_080656B4
_08065A8E:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08065AA0: .4byte 0x000007FF
_08065AA4: .4byte gUnk_08621DE0
_08065AA8: .4byte 0x0600C000
_08065AAC: .4byte 0x0201DB20
_08065AB0: .4byte 0x0000063E
	thumb_func_end sub_080657F8

