	thumb_func_start RenderShadowedGlyph
RenderShadowedGlyph: @ 0x080788AC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	mov r9, r0
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r8, r2
	lsl r3, r3, #0x18
	lsr r7, r3, #0x18
	mov r5, #1
	mov r0, r8
	lsl r0, r0, #4
	str r0, [sp, #0x10]
	mov r1, r8
	lsl r1, r1, #8
	str r1, [sp, #0x14]
	lsl r2, r2, #0xC
	str r2, [sp, #0x18]
	mov r0, r8
	lsl r0, r0, #0x10
	str r0, [sp, #0x1C]
	mov r1, r8
	lsl r1, r1, #0x14
	str r1, [sp, #0x20]
	mov r2, r8
	lsl r2, r2, #0x18
	str r2, [sp, #0x24]
	mov r0, r8
	lsl r0, r0, #0x1C
	mov sl, r0
	mov r0, #0
	str r0, [sp, #0xC]
	add r0, sp, #0xC
	ldr r2, _08078A74 @ =0x05000003
	mov r1, sp
	bl CpuSet
	ldr r6, _08078A78 @ =0x0822BB00
	cmp r4, #0x9F
	bls _08078922
	add r0, r4, #0
	sub r0, #0x20
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	add r0, r5, #0
	ldr r1, [sp, #0x48]
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08078922
	add r0, r4, #0
	add r0, #0x40
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_08078922:
	lsl r0, r5, #3
	mul r0, r4
	add r6, r6, r0
	mov r1, r9
	ldr r3, [r1]
	mov r0, #1
	ldrb r2, [r6]
	and r0, r2
	cmp r0, #0
	beq _0807894C
	mov r0, #0x10
	neg r0, r0
	and r3, r0
	mov r0, r8
	orr r3, r0
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #2
	orr r0, r1
	strb r0, [r2, #1]
_0807894C:
	mov r0, #2
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _0807896C
	mov r0, #0xF1
	neg r0, r0
	and r3, r0
	ldr r2, [sp, #0x10]
	orr r3, r2
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #4
	orr r0, r1
	strb r0, [r2, #1]
_0807896C:
	mov r0, #4
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _0807898A
	ldr r0, _08078A7C @ =0xFFFFF0FF
	and r3, r0
	ldr r2, [sp, #0x14]
	orr r3, r2
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #8
	orr r0, r1
	strb r0, [r2, #1]
_0807898A:
	mov r0, #8
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _080789A8
	ldr r0, _08078A80 @ =0xFFFF0FFF
	and r3, r0
	ldr r2, [sp, #0x18]
	orr r3, r2
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #0x10
	orr r0, r1
	strb r0, [r2, #1]
_080789A8:
	mov r0, #0x10
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _080789C6
	ldr r0, _08078A84 @ =0xFFF0FFFF
	and r3, r0
	ldr r2, [sp, #0x1C]
	orr r3, r2
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #0x20
	orr r0, r1
	strb r0, [r2, #1]
_080789C6:
	mov r0, #0x20
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _080789E4
	ldr r0, _08078A88 @ =0xFF0FFFFF
	and r3, r0
	ldr r2, [sp, #0x20]
	orr r3, r2
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #0x40
	orr r0, r1
	strb r0, [r2, #1]
_080789E4:
	mov r0, #0x40
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078A02
	ldr r0, _08078A8C @ =0xF0FFFFFF
	and r3, r0
	ldr r2, [sp, #0x24]
	orr r3, r2
	mov r2, sp
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #0x80
	orr r0, r1
	strb r0, [r2, #1]
_08078A02:
	mov r0, #0x80
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078A14
	ldr r0, _08078A90 @ =0x0FFFFFFF
	and r3, r0
	mov r2, sl
	orr r3, r2
_08078A14:
	mov r0, r9
	add r0, #4
	mov r9, r0
	sub r0, #4
	stmia r0!, {r3}
	add r6, #1
	mov r5, #1
	cmp r5, #8
	bcc _08078A28
	b _08078C34
_08078A28:
	mov r2, r9
	ldr r3, [r2]
	mov r0, #1
	ldrb r1, [r6]
	and r0, r1
	add r4, r5, #1
	cmp r0, #0
	beq _08078A4E
	mov r0, #0x10
	neg r0, r0
	and r3, r0
	mov r2, r8
	orr r3, r2
	mov r0, sp
	add r1, r0, r4
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_08078A4E:
	mov r2, #2
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078A94
	mov r0, #0xF1
	neg r0, r0
	and r3, r0
	ldr r2, [sp, #0x10]
	orr r3, r2
	mov r0, sp
	add r1, r0, r4
	mov r0, #4
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _08078AAC
	.align 2, 0
_08078A74: .4byte 0x05000003
_08078A78: .4byte gFontLatin8x8Bold
_08078A7C: .4byte 0xFFFFF0FF
_08078A80: .4byte 0xFFFF0FFF
_08078A84: .4byte 0xFFF0FFFF
_08078A88: .4byte 0xFF0FFFFF
_08078A8C: .4byte 0xF0FFFFFF
_08078A90: .4byte 0x0FFFFFFF
_08078A94:
	mov r0, sp
	add r1, r0, r5
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08078AAC
	mov r0, #0xF1
	neg r0, r0
	and r3, r0
	lsl r0, r7, #4
	orr r3, r0
_08078AAC:
	mov r2, #4
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078AD4
	ldr r0, _08078AD0 @ =0xFFFFF0FF
	and r3, r0
	ldr r2, [sp, #0x14]
	orr r3, r2
	mov r0, sp
	add r1, r0, r4
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _08078AEA
	.align 2, 0
_08078AD0: .4byte 0xFFFFF0FF
_08078AD4:
	mov r0, sp
	add r1, r0, r5
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08078AEA
	ldr r0, _08078B0C @ =0xFFFFF0FF
	and r3, r0
	lsl r0, r7, #8
	orr r3, r0
_08078AEA:
	mov r2, #8
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078B14
	ldr r0, _08078B10 @ =0xFFFF0FFF
	and r3, r0
	ldr r2, [sp, #0x18]
	orr r3, r2
	mov r0, sp
	add r1, r0, r4
	mov r0, #0x10
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _08078B2A
_08078B0C: .4byte 0xFFFFF0FF
_08078B10: .4byte 0xFFFF0FFF
_08078B14:
	mov r0, sp
	add r1, r0, r5
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08078B2A
	ldr r0, _08078B4C @ =0xFFFF0FFF
	and r3, r0
	lsl r0, r7, #0xC
	orr r3, r0
_08078B2A:
	mov r2, #0x10
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078B54
	ldr r0, _08078B50 @ =0xFFF0FFFF
	and r3, r0
	ldr r2, [sp, #0x1C]
	orr r3, r2
	mov r0, sp
	add r1, r0, r4
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _08078B6A
_08078B4C: .4byte 0xFFFF0FFF
_08078B50: .4byte 0xFFF0FFFF
_08078B54:
	mov r0, sp
	add r1, r0, r5
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08078B6A
	ldr r0, _08078B8C @ =0xFFF0FFFF
	and r3, r0
	lsl r0, r7, #0x10
	orr r3, r0
_08078B6A:
	mov r2, #0x20
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078B94
	ldr r0, _08078B90 @ =0xFF0FFFFF
	and r3, r0
	ldr r2, [sp, #0x20]
	orr r3, r2
	mov r0, sp
	add r1, r0, r4
	mov r0, #0x40
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _08078BAA
_08078B8C: .4byte 0xFFF0FFFF
_08078B90: .4byte 0xFF0FFFFF
_08078B94:
	mov r0, sp
	add r1, r0, r5
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08078BAA
	ldr r0, _08078BCC @ =0xFF0FFFFF
	and r3, r0
	lsl r0, r7, #0x14
	orr r3, r0
_08078BAA:
	mov r2, #0x40
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078BD4
	ldr r0, _08078BD0 @ =0xF0FFFFFF
	and r3, r0
	ldr r2, [sp, #0x24]
	orr r3, r2
	mov r0, sp
	add r1, r0, r4
	mov r0, #0x80
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _08078BEA
_08078BCC: .4byte 0xFF0FFFFF
_08078BD0: .4byte 0xF0FFFFFF
_08078BD4:
	mov r0, sp
	add r1, r0, r5
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08078BEA
	ldr r0, _08078C00 @ =0xF0FFFFFF
	and r3, r0
	lsl r0, r7, #0x18
	orr r3, r0
_08078BEA:
	mov r2, #0x80
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _08078C08
	ldr r0, _08078C04 @ =0x0FFFFFFF
	and r3, r0
	mov r2, sl
	orr r3, r2
	b _08078C1E
_08078C00: .4byte 0xF0FFFFFF
_08078C04: .4byte 0x0FFFFFFF
_08078C08:
	mov r0, sp
	add r1, r0, r5
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08078C1E
	ldr r0, _08078C44 @ =0x0FFFFFFF
	and r3, r0
	lsl r0, r7, #0x1C
	orr r3, r0
_08078C1E:
	mov r1, r9
	add r1, #4
	mov r9, r1
	sub r1, #4
	stmia r1!, {r3}
	add r6, #1
	lsl r0, r4, #0x10
	lsr r5, r0, #0x10
	cmp r5, #8
	bcs _08078C34
	b _08078A28
_08078C34:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08078C44: .4byte 0x0FFFFFFF
	thumb_func_end RenderShadowedGlyph

