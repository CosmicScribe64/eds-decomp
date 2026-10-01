	thumb_func_start sub_08006E94
sub_08006E94: @ 0x08006E94
	push {r4, lr}
	ldr r0, _08006EB0 @ =0x03000040
	ldr r1, _08006EB4 @ =0x0000485A
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	beq _08006EB8
	cmp r0, #1
	beq _08006F90
	bl sub_08006A98
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08006FA6
_08006EB0: .4byte 0x03000040
_08006EB4: .4byte 0x0000485A
_08006EB8:
	ldr r4, _08006EF0 @ =0x02013D90
	add r0, r4, #0
	mov r1, #0x44
	bl sub_08075278
	add r1, r4, #0
	add r1, #0x40
	mov r0, #0xC8
	lsl r0, r0, #2
	strh r0, [r1]
	strh r0, [r4, #2]
	add r2, r0, #0
	lsl r0, r2, #2
	ldr r1, _08006EF4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08006F02
	cmp r0, #0x17
	ble _08006EF8
	cmp r0, #0x18
	beq _08006EFC
	b _08006F02
	.align 2, 0
_08006EF0: .4byte 0x02013D90
_08006EF4: .4byte gUnk_08621DE0
_08006EF8:
	mov r0, #0
	b _08006F14
_08006EFC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08006F14
_08006F02:
	lsl r0, r2, #2
	ldr r1, _08006F40 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08006F14:
	ldr r2, _08006F44 @ =0x02013D90
	str r0, [r2, #0x2C]
	add r0, r2, #0
	add r0, #0x40
	ldrh r3, [r0]
	ldr r0, _08006F48 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08006F40 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08006F56
	cmp r0, #0x17
	ble _08006F4C
	cmp r0, #0x18
	beq _08006F50
	b _08006F56
_08006F40: .4byte gUnk_08621DE0
_08006F44: .4byte 0x02013D90
_08006F48: .4byte 0x000007FF
_08006F4C:
	mov r0, #0
	b _08006F6C
_08006F50:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08006F6C
_08006F56:
	ldr r0, _08006F7C @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _08006F80 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08006F84 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08006F6C:
	str r0, [r2, #0x30]
	ldr r0, _08006F88 @ =0x03000040
	ldr r1, _08006F8C @ =0x0000485A
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _08006FA4
_08006F7C: .4byte 0x000007FF
_08006F80: .4byte gUnk_08621DE0
_08006F84: .4byte 0x000001FF
_08006F88: .4byte 0x03000040
_08006F8C: .4byte 0x0000485A
_08006F90:
	bl sub_0800696C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08006FA4
	bl sub_08006B80
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08006FA4:
	mov r0, #0
_08006FA6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08006E94

