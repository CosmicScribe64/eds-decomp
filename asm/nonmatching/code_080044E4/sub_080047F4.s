	thumb_func_start sub_080047F4
sub_080047F4: @ 0x080047F4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r7, #0x1F
	ldr r0, _08004850 @ =0x00008EAB
	cmp r5, r0
	bls _08004810
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
_08004810:
	ldr r4, _08004854 @ =0x000005B5
	add r0, r5, #0
	add r1, r4, #0
	bl __udivsi3
	lsl r0, r0, #0x12
	lsr r6, r0, #0x10
	add r0, r5, #0
	add r1, r4, #0
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldr r0, _08004858 @ =0x000007D1
	add r4, r6, r0
	ldr r1, _0800485C @ =0x0000016D
	mov r9, r1
	add r0, r5, #0
	bl __udivsi3
	add r4, r4, r0
	lsl r4, r4, #0x10
	lsr r6, r4, #0x10
	ldr r0, _08004860 @ =0x000005B4
	cmp r5, r0
	bne _08004864
	mov r5, r9
	sub r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	b _08004870
	.align 2, 0
_08004850: .4byte 0x00008EAB
_08004854: .4byte 0x000005B5
_08004858: .4byte 0x000007D1
_0800485C: .4byte 0x0000016D
_08004860: .4byte 0x000005B4
_08004864:
	add r0, r5, #0
	mov r1, r9
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
_08004870:
	mov r4, #0
_08004872:
	cmp r5, r7
	blt _0800489C
	sub r0, r5, r7
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r0, _08004898 @ =0x08198628
	add r0, r4, r0
	ldrb r7, [r0]
	cmp r4, #1
	bne _08004872
	add r0, r6, #0
	bl sub_08004280
	add r7, r7, r0
	b _08004872
	.align 2, 0
_08004898: .4byte gUnk_08198628
_0800489C:
	ldr r2, _0800490C @ =0x00000FFF
	add r0, r2, #0
	add r1, r6, #0
	and r1, r0
	ldr r0, _08004910 @ =0xFFFFF000
	mov r3, r8
	ldrh r3, [r3]
	and r0, r3
	orr r0, r1
	mov r7, r8
	strh r0, [r7]
	add r1, r4, #1
	lsl r2, r1, #4
	mov r0, #0xF
	ldrb r3, [r7, #1]
	and r0, r3
	orr r0, r2
	strb r0, [r7, #1]
	add r2, r5, #1
	mov r0, #0x1F
	add r3, r2, #0
	and r3, r0
	mov r4, #0x20
	neg r4, r4
	add r0, r4, #0
	ldrb r5, [r7, #2]
	and r0, r5
	orr r0, r3
	strb r0, [r7, #2]
	add r0, r6, #0
	bl sub_080042D8
	lsl r0, r0, #5
	mov r1, #0x1F
	ldrb r7, [r7, #2]
	and r1, r7
	orr r1, r0
	mov r0, r8
	strb r1, [r0, #2]
	mov r0, #0x1F
	mov r2, r8
	ldrh r2, [r2, #2]
	and r0, r2
	cmp r0, #0
	bne _08004900
	mov r0, #1
	and r1, r4
	orr r1, r0
	mov r3, r8
	strb r1, [r3, #2]
_08004900:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800490C: .4byte 0x00000FFF
_08004910: .4byte 0xFFFFF000
	thumb_func_end sub_080047F4

