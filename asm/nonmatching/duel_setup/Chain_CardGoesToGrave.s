	thumb_func_start Chain_CardGoesToGrave
Chain_CardGoesToGrave: @ 0x0801FCA8
	add r2, r0, #0
	mov r3, #1
	mov r0, #0xE
	ldrb r1, [r2, #2]
	and r0, r1
	cmp r0, #6
	bne _0801FCD0
	ldr r0, _0801FCF0 @ =0x000007FF
	ldrh r1, [r2]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0801FCF4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0801FCEA
_0801FCD0:
	ldr r0, _0801FCF0 @ =0x000007FF
	ldrh r1, [r2]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0801FCF4 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0801FCF8
_0801FCEA:
	mov r0, #0
	b _0801FD5C
	.align 2, 0
_0801FCF0: .4byte 0x000007FF
_0801FCF4: .4byte gCardStats
_0801FCF8:
	cmp r0, #0x16
	bgt _0801FD0A
	cmp r0, #0x15
	blt _0801FD0A
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0801FD0C
_0801FD0A:
	mov r0, #0
_0801FD0C:
	cmp r0, #4
	bgt _0801FD14
	cmp r0, #2
	bge _0801FD54
_0801FD14:
	ldr r0, _0801FD38 @ =0x000007FF
	ldrh r1, [r2]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0801FD3C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0801FD40 @ =0x000004CE
	cmp r1, r0
	beq _0801FD54
	cmp r1, r0
	bgt _0801FD48
	cmp r1, #0x47
	beq _0801FD54
	ldr r0, _0801FD44 @ =0x0000015B
	cmp r1, r0
	beq _0801FD54
	b _0801FD5A
_0801FD38: .4byte 0x000007FF
_0801FD3C: .4byte gCardIdToNumber
_0801FD40: .4byte 0x000004CE
_0801FD44: .4byte 0x0000015B
_0801FD48:
	ldr r0, _0801FD60 @ =0x000004EA
	cmp r1, r0
	beq _0801FD54
	ldr r0, _0801FD64 @ =0x00000609
	cmp r1, r0
	bne _0801FD5A
_0801FD54:
	ldrb r2, [r2, #4]
	lsl r0, r2, #0x1C
	lsr r3, r0, #0x1F
_0801FD5A:
	add r0, r3, #0
_0801FD5C:
	bx lr
	.align 2, 0
_0801FD60: .4byte 0x000004EA
_0801FD64: .4byte 0x00000609
	thumb_func_end Chain_CardGoesToGrave

