	thumb_func_start sub_08078ED4
sub_08078ED4: @ 0x08078ED4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r4, r1, #0
	ldr r1, [sp, #0x20]
	lsl r0, r0, #0x18
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x10
	lsr r6, r3, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov sl, r1
	lsr r0, r0, #0x15
	ldr r1, _08078F54 @ =0x08228D00
	add r5, r0, r1
	mov r0, #0xF
	mov r8, r0
	mov r1, #3
	mov r9, r1
_08078F00:
	mov r0, sl
	cmp r0, #0
	bne _08078F58
	ldrh r1, [r5]
	lsr r0, r1, #4
	mov r1, r8
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	strh r0, [r4]
	add r4, #2
	mov r0, #0xF
	ldrh r1, [r5]
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #0xC
	mov r1, r8
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #8
	mov r1, r8
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	b _08078FB2
	.align 2, 0
_08078F54: .4byte gUnk_08228D00
_08078F58:
	mov r0, #0xF
	ldrh r1, [r5]
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	ldrh r1, [r4]
	orr r0, r1
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #4
	mov r1, r8
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	ldrh r1, [r4]
	orr r0, r1
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #8
	mov r1, r8
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	ldrh r1, [r4]
	orr r0, r1
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #0xC
	mov r1, r8
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl sub_080725B0
	ldrh r1, [r4]
	orr r0, r1
_08078FB2:
	strh r0, [r4]
	add r4, #2
	add r5, #2
	mov r0, #1
	neg r0, r0
	add r9, r0
	mov r1, r9
	cmp r1, #0
	bge _08078F00
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08078ED4
	.align 2, 0

