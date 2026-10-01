	thumb_func_start sub_08012ED4
sub_08012ED4: @ 0x08012ED4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	ldr r4, _08012F14 @ =0x020185C0
	ldrh r0, [r4]
	lsr r6, r0, #0xF
	ldrh r7, [r4, #2]
	ldr r1, _08012F18 @ =0x0000080A
	add r1, r1, r4
	mov r8, r1
	ldrb r2, [r1]
	lsl r0, r2, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _08012F20
	cmp r0, #1
	beq _08012F2A
	add r0, r6, #0
	add r1, r7, #0
	bl sub_080098C0
	bl sub_080611AC
	ldr r0, _08012F1C @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08012F84
	.align 2, 0
_08012F14: .4byte 0x020185C0
_08012F18: .4byte 0x0000080A
_08012F1C: .4byte 0x0000080D
_08012F20:
	add r0, r6, #0
	mov r1, #0xB
	bl sub_080240A8
	b _08012F6C
_08012F2A:
	and r6, r0
	mov r2, #2
	neg r2, r2
	ldr r0, [sp, #0]
	and r0, r2
	orr r0, r6
	mov r1, #0x1E
	orr r0, r1
	ldr r4, _08012F94 @ =0xFFFFC01F
	and r0, r4
	ldr r5, _08012F98 @ =0xFFFFBFFF
	and r0, r5
	ldr r3, _08012F9C @ =0xFFFF7FFF
	and r0, r3
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r2
	orr r0, r6
	sub r1, #0x3D
	and r0, r1
	ldr r1, _08012FA0 @ =0x000001FF
	and r7, r1
	lsl r1, r7, #5
	and r0, r4
	orr r0, r1
	and r0, r5
	and r0, r3
	str r0, [sp, #4]
	add r2, sp, #4
	mov r0, #1
	mov r1, sp
	bl sub_080242C4
_08012F6C:
	mov r0, r8
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r8
_08012F84:
	strb r0, [r1]
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08012F94: .4byte 0xFFFFC01F
_08012F98: .4byte 0xFFFFBFFF
_08012F9C: .4byte 0xFFFF7FFF
_08012FA0: .4byte 0x000001FF
	thumb_func_end sub_08012ED4

