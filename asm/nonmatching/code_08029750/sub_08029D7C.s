	thumb_func_start sub_08029D7C
sub_08029D7C: @ 0x08029D7C
	push {r4, lr}
	ldr r0, _08029DB0 @ =0x02020310
	ldr r1, _08029DB4 @ =0x00000B0E
	add r0, r0, r1
	mov r1, #0
	strb r1, [r0]
	ldr r1, _08029DB8 @ =0x0819A718
	ldr r0, _08029DBC @ =0x03000040
	ldr r2, _08029DC0 @ =0x00004859
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08029DC4
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08029DAC
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08029DAC:
	mov r0, #0
	b _08029DC6
_08029DB0: .4byte 0x02020310
_08029DB4: .4byte 0x00000B0E
_08029DB8: .4byte gUnk_0819A718
_08029DBC: .4byte 0x03000040
_08029DC0: .4byte 0x00004859
_08029DC4:
	mov r0, #1
_08029DC6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08029D7C

