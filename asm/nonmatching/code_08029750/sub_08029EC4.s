	thumb_func_start sub_08029EC4
sub_08029EC4: @ 0x08029EC4
	push {r4, lr}
	ldr r1, _08029EF0 @ =0x0819A73C
	ldr r0, _08029EF4 @ =0x03000040
	ldr r2, _08029EF8 @ =0x00004859
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08029EFC
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08029EEA
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08029EEA:
	mov r0, #0
	b _08029EFE
	.align 2, 0
_08029EF0: .4byte gUnk_0819A73C
_08029EF4: .4byte 0x03000040
_08029EF8: .4byte 0x00004859
_08029EFC:
	mov r0, #1
_08029EFE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08029EC4

