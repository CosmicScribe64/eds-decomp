	thumb_func_start sub_0800D824
sub_0800D824: @ 0x0800D824
	push {r4, lr}
	sub sp, #4
	ldr r4, _0800D85C @ =0x020185C0
	ldrh r1, [r4, #4]
	lsl r0, r1, #0x10
	ldrh r2, [r4, #2]
	orr r0, r2
	str r0, [sp, #0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800D840
	mov r0, sp
	bl sub_080096F4
_0800D840:
	bl sub_080611AC
	ldr r0, _0800D860 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800D85C: .4byte 0x020185C0
_0800D860: .4byte 0x0000080D
	thumb_func_end sub_0800D824

