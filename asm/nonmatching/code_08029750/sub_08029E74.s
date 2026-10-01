	thumb_func_start sub_08029E74
sub_08029E74: @ 0x08029E74
	push {r4, lr}
	ldr r0, _08029EA8 @ =0x02020310
	ldr r1, _08029EAC @ =0x00000B0E
	add r0, r0, r1
	mov r1, #1
	strb r1, [r0]
	ldr r1, _08029EB0 @ =0x0819A72C
	ldr r0, _08029EB4 @ =0x03000040
	ldr r2, _08029EB8 @ =0x00004859
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08029EBC
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08029EA4
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08029EA4:
	mov r0, #0
	b _08029EBE
_08029EA8: .4byte 0x02020310
_08029EAC: .4byte 0x00000B0E
_08029EB0: .4byte gUnk_0819A72C
_08029EB4: .4byte 0x03000040
_08029EB8: .4byte 0x00004859
_08029EBC:
	mov r0, #1
_08029EBE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08029E74

