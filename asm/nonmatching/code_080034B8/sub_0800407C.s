	thumb_func_start sub_0800407C
sub_0800407C: @ 0x0800407C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r0
	add r5, r1, #0
	add r6, r3, #0
	add r5, #2
	ldr r0, _080040DC @ =0x03000040
	mov sl, r0
	lsl r2, r2, #0x10
	lsr r2, r2, #0xB
	mov r8, r2
	mov r7, #2
_0800409A:
	lsl r4, r5, #0x10
	lsr r4, r4, #0x10
	add r4, r8
	lsl r4, r4, #1
	mov r1, r9
	lsl r0, r1, #0xB
	add r4, r4, r0
	ldr r0, _080040E0 @ =0x0000041C
	add r0, sl
	add r4, r4, r0
	add r0, r6, #0
	mov r1, #0xA
	bl __modsi3
	add r0, #4
	strh r0, [r4]
	add r0, r6, #0
	mov r1, #0xA
	bl __divsi3
	add r6, r0, #0
	sub r5, #1
	sub r7, #1
	cmp r7, #0
	bge _0800409A
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080040DC: .4byte 0x03000040
_080040E0: .4byte 0x0000041C
	thumb_func_end sub_0800407C

