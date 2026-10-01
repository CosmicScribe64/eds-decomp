	thumb_func_start sub_08016460
sub_08016460: @ 0x08016460
	push {lr}
	bl sub_08060B4C
	cmp r0, #0
	beq _0801647A
	ldr r1, _08016480 @ =0x020185C0
	ldr r0, _08016484 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801647A:
	pop {r0}
	bx r0
	.align 2, 0
_08016480: .4byte 0x020185C0
_08016484: .4byte 0x0000080D
	thumb_func_end sub_08016460

