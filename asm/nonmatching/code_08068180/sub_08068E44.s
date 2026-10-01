	thumb_func_start sub_08068E44
sub_08068E44: @ 0x08068E44
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _08068E70 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08068E74 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08068E82
	cmp r0, #0x17
	ble _08068E78
	cmp r0, #0x18
	beq _08068E7C
	b _08068E82
	.align 2, 0
_08068E70: .4byte 0x000007FF
_08068E74: .4byte gUnk_08621DE0
_08068E78:
	mov r2, #0
	b _08068E98
_08068E7C:
	mov r2, #0xFA
	lsl r2, r2, #4
	b _08068E98
_08068E82:
	ldr r0, _08068EBC @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08068EC0 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r0, #1
_08068E98:
	ldr r0, _08068EBC @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08068EC0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08068ECE
	cmp r0, #0x17
	ble _08068EC4
	cmp r0, #0x18
	beq _08068EC8
	b _08068ECE
	.align 2, 0
_08068EBC: .4byte 0x000007FF
_08068EC0: .4byte gUnk_08621DE0
_08068EC4:
	mov r0, #0
	b _08068EE4
_08068EC8:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08068EE4
_08068ECE:
	ldr r0, _08068EF4 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08068EF8 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08068EE4:
	mov r1, #0
	sub r0, r2, r0
	cmp r0, #0
	ble _08068EEE
	mov r1, #1
_08068EEE:
	add r0, r1, #0
	bx lr
	.align 2, 0
_08068EF4: .4byte 0x000007FF
_08068EF8: .4byte gUnk_08621DE0
	thumb_func_end sub_08068E44

