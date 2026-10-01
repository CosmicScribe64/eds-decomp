	thumb_func_start sub_08010FAC
sub_08010FAC: @ 0x08010FAC
	push {r4, lr}
	sub sp, #4
	ldr r4, _08010FDC @ =0x020185C0
	ldrh r1, [r4]
	lsr r0, r1, #0xF
	ldrh r2, [r4, #4]
	lsl r1, r2, #0x10
	ldrh r2, [r4, #2]
	orr r1, r2
	str r1, [sp, #0]
	mov r1, sp
	bl sub_08009EAC
	ldr r0, _08010FE0 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
_08010FDC: .4byte 0x020185C0
_08010FE0: .4byte 0x0000080D
	thumb_func_end sub_08010FAC

