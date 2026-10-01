	thumb_func_start sub_08065108
sub_08065108: @ 0x08065108
	push {r4, r5, r6, lr}
	mov r6, sl
	mov r5, r9
	mov r4, r8
	push {r4, r5, r6}
	sub sp, #0x10
	mov r8, r0
	mov r9, r1
	add r4, r2, #0
	add r5, r3, #0
	ldr r6, [sp, #0x30]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	add r0, r6, #0
	bl sub_08064F90
	mov sl, r0
	add r0, r6, #0
	bl sub_08064FF8
	add r3, r0, #0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r0, r8
	lsl r0, r0, #6
	mov r8, r0
	ldr r0, _08065188 @ =0x0822C720
	add r8, r0
	add r4, #4
	mov r0, #0x1F
	and r4, r0
	add r5, #1
	and r5, r0
	lsl r5, r5, #5
	add r4, r4, r5
	lsl r4, r4, #1
	add r9, r4
	mov r0, #2
	str r0, [sp, #0]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	str r0, [sp, #0xC]
	mov r0, r8
	mov r1, r9
	mov r2, sl
	bl sub_08079404
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08065188: .4byte gUnk_0822C720
	thumb_func_end sub_08065108

