	thumb_func_start sub_0800D468
sub_0800D468: @ 0x0800D468
	push {r4, lr}
	bl sub_08060B4C
	cmp r0, #0
	beq _0800D48A
	ldr r4, _0800D490 @ =0x020185C0
	ldrh r0, [r4, #2]
	ldrh r1, [r4, #4]
	bl sub_0805E3B8
	ldr r0, _0800D494 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
_0800D48A:
	pop {r4}
	pop {r0}
	bx r0
_0800D490: .4byte 0x020185C0
_0800D494: .4byte 0x0000080D
	thumb_func_end sub_0800D468

