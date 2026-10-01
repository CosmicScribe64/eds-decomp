	thumb_func_start sub_0802DF8C
sub_0802DF8C: @ 0x0802DF8C
	push {r4, r5, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	beq _0802DFC6
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08047170
	cmp r0, #0
	beq _0802DFC6
	ldr r5, _0802DFCC @ =0x00000402
	mov r0, #0
	add r1, r5, #0
	bl sub_080090C8
	cmp r0, #0
	bgt _0802DFC6
	mov r0, #1
	add r1, r5, #0
	bl sub_080090C8
	cmp r0, #0
	ble _0802DFD0
_0802DFC6:
	mov r0, #0
	b _0802DFF4
	.align 2, 0
_0802DFCC: .4byte 0x00000402
_0802DFD0:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802DFFC @ =0x000007FF
	ldrh r4, [r4]
	and r1, r4
	lsl r1, r1, #1
	ldr r2, _0802E000 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	mov r1, #0
	cmp r0, #0
	ble _0802DFF2
	mov r1, #1
_0802DFF2:
	add r0, r1, #0
_0802DFF4:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802DFFC: .4byte 0x000007FF
_0802E000: .4byte gUnk_08622AB4
	thumb_func_end sub_0802DF8C

