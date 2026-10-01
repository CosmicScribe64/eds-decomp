	thumb_func_start sub_0802E6F8
sub_0802E6F8: @ 0x0802E6F8
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08047170
	cmp r0, #0
	beq _0802E74C
	ldr r2, _0802E740 @ =0x020192E4
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	lsr r1, r3, #0x1F
	ldr r0, _0802E744 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #0xB]
	lsl r0, r0, #0x1C
	cmp r0, #0
	bge _0802E74C
	lsr r0, r3, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	beq _0802E74C
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802E748 @ =0x0000041E
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	beq _0802E74C
	mov r0, #1
	b _0802E74E
_0802E740: .4byte 0x020192E4
_0802E744: .4byte 0x00000D64
_0802E748: .4byte 0x0000041E
_0802E74C:
	mov r0, #0
_0802E74E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802E6F8

