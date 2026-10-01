	thumb_func_start sub_080149D8
sub_080149D8: @ 0x080149D8
	push {r4, lr}
	ldr r1, _08014A14 @ =0x020192E4
	ldr r3, _08014A18 @ =0x020185C0
	mov r0, #0x80
	lsl r0, r0, #8
	ldrh r2, [r3]
	and r0, r2
	mov r2, #0
	cmp r0, #0
	beq _080149EE
	ldr r2, _08014A1C @ =0x00000D64
_080149EE:
	add r2, r2, r1
	ldrb r4, [r3, #2]
	lsl r1, r4, #6
	mov r0, #0x3F
	ldrb r4, [r2, #7]
	and r0, r4
	orr r0, r1
	strb r0, [r2, #7]
	ldr r0, _08014A20 @ =0x0000080D
	add r1, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08014A14: .4byte 0x020192E4
_08014A18: .4byte 0x020185C0
_08014A1C: .4byte 0x00000D64
_08014A20: .4byte 0x0000080D
	thumb_func_end sub_080149D8

