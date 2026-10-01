	thumb_func_start sub_0800D498
sub_0800D498: @ 0x0800D498
	push {r4, lr}
	ldr r4, _0800D4C0 @ =0x020185C0
	ldrh r0, [r4, #2]
	ldrh r1, [r4, #4]
	ldrh r2, [r4, #6]
	bl sub_0805E788
	cmp r0, #0
	beq _0800D4B8
	ldr r0, _0800D4C4 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800D4B8:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800D4C0: .4byte 0x020185C0
_0800D4C4: .4byte 0x0000080D
	thumb_func_end sub_0800D498

