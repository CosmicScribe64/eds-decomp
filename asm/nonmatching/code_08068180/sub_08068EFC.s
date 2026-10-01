	thumb_func_start sub_08068EFC
sub_08068EFC: @ 0x08068EFC
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _08068F28 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08068F2C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08068F3A
	cmp r0, #0x17
	ble _08068F30
	cmp r0, #0x18
	beq _08068F34
	b _08068F3A
	.align 2, 0
_08068F28: .4byte 0x000007FF
_08068F2C: .4byte gUnk_08621DE0
_08068F30:
	mov r2, #0
	b _08068F50
_08068F34:
	mov r2, #0xFA
	lsl r2, r2, #4
	b _08068F50
_08068F3A:
	ldr r0, _08068F74 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08068F78 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08068F7C @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r0, #1
_08068F50:
	ldr r0, _08068F74 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08068F78 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08068F8A
	cmp r0, #0x17
	ble _08068F80
	cmp r0, #0x18
	beq _08068F84
	b _08068F8A
	.align 2, 0
_08068F74: .4byte 0x000007FF
_08068F78: .4byte gUnk_08621DE0
_08068F7C: .4byte 0x000001FF
_08068F80:
	mov r0, #0
	b _08068FA0
_08068F84:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08068FA0
_08068F8A:
	ldr r0, _08068FB0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08068FB4 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08068FB8 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08068FA0:
	mov r1, #0
	sub r0, r2, r0
	cmp r0, #0
	ble _08068FAA
	mov r1, #1
_08068FAA:
	add r0, r1, #0
	bx lr
	.align 2, 0
_08068FB0: .4byte 0x000007FF
_08068FB4: .4byte gUnk_08621DE0
_08068FB8: .4byte 0x000001FF
	thumb_func_end sub_08068EFC

