	thumb_func_start sub_08029DCC
sub_08029DCC: @ 0x08029DCC
	push {r4, lr}
	ldr r0, _08029E00 @ =0x02020310
	ldr r1, _08029E04 @ =0x00000B0E
	add r0, r0, r1
	mov r1, #1
	strb r1, [r0]
	ldr r1, _08029E08 @ =0x0819A718
	ldr r2, _08029E0C @ =0x03000040
	ldr r3, _08029E10 @ =0x00004859
	add r4, r2, r3
	ldrb r3, [r4]
	lsl r0, r3, #2
	add r0, r0, r1
	ldr r1, [r0]
	cmp r1, #0
	beq _08029E2A
	mov r0, #2
	ldrh r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08029E18
	ldr r0, _08029E14 @ =0x08003AA5
	bl sub_080754F8
_08029DFC:
	mov r0, #0
	b _08029E2C
_08029E00: .4byte 0x02020310
_08029E04: .4byte 0x00000B0E
_08029E08: .4byte gUnk_0819A718
_08029E0C: .4byte 0x03000040
_08029E10: .4byte 0x00004859
_08029E14: .4byte sub_08003AA4
_08029E18:
	bl _call_via_r1
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08029DFC
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08029DFC
_08029E2A:
	mov r0, #1
_08029E2C:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08029DCC
	.align 2, 0

