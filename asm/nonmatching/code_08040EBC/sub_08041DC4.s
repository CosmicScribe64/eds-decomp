	thumb_func_start sub_08041DC4
sub_08041DC4: @ 0x08041DC4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	add r7, r1, #0
	add r6, r2, #0
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	add r2, r6, #0
	mul r2, r0
	ldr r0, _08041E44 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r3, _08041E48 @ =0x0201930C
	add r2, r2, r3
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldrb r2, [r2, #6]
	lsl r0, r2, #0x1E
	lsr r5, r0, #0x1F
	cmp r4, #0
	bne _08041DF6
	b _08041EF4
_08041DF6:
	ldr r0, _08041E4C @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08041E50 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08041EF4
	ldr r2, _08041E54 @ =0x00001AE6
	add r0, r3, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	cmp r0, r7
	beq _08041E26
	add r0, r4, #0
	bl sub_0802CD28
	cmp r0, #1
	ble _08041EF4
_08041E26:
	ldr r0, _08041E4C @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r3, _08041E58 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08041E5C @ =0x0000052C
	cmp r1, r0
	beq _08041E70
	cmp r1, r0
	bgt _08041E64
	ldr r0, _08041E60 @ =0x000003F9
	cmp r1, r0
	beq _08041E70
	b _08041E72
_08041E44: .4byte 0x00000D64
_08041E48: .4byte 0x0201930C
_08041E4C: .4byte 0x000007FF
_08041E50: .4byte gUnk_08621DE0
_08041E54: .4byte 0x00001AE6
_08041E58: .4byte gUnk_08622AB4
_08041E5C: .4byte 0x0000052C
_08041E60: .4byte 0x000003F9
_08041E64:
	ldr r0, _08041EF8 @ =0x00000594
	cmp r1, r0
	beq _08041E70
	add r0, #0x68
	cmp r1, r0
	bne _08041E72
_08041E70:
	mov r5, #0
_08041E72:
	cmp r5, #0
	bne _08041EF4
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	mul r0, r6
	ldr r1, _08041EFC @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08041F00 @ =0x0201930C
	add r0, r0, r1
	add r0, #0x91
	ldrb r1, [r0]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _08041EF4
	mov r0, #8
	and r0, r1
	cmp r0, #0
	bne _08041EF4
	ldr r0, _08041F04 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08041F08 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08041ECE
	ldr r5, _08041F0C @ =0x000002EF
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bne _08041EF4
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bne _08041EF4
_08041ECE:
	add r0, r4, #0
	bl sub_0802CD28
	cmp r0, #1
	bgt _08041F18
	ldr r0, _08041F10 @ =0x020192E0
	ldr r2, _08041F14 @ =0x00001B12
	add r0, r0, r2
	ldrb r1, [r0]
	lsl r0, r1, #0x1B
	lsr r0, r0, #0x1D
	cmp r0, #2
	beq _08041EEC
	cmp r0, #4
	bne _08041EF4
_08041EEC:
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	cmp r0, r7
	beq _08041F18
_08041EF4:
	mov r0, #0
	b _08041F7E
_08041EF8: .4byte 0x00000594
_08041EFC: .4byte 0x00000D64
_08041F00: .4byte 0x0201930C
_08041F04: .4byte 0x000007FF
_08041F08: .4byte gUnk_08621DE0
_08041F0C: .4byte 0x000002EF
_08041F10: .4byte 0x020192E0
_08041F14: .4byte 0x00001B12
_08041F18:
	mov r3, r8
	strh r4, [r3]
	mov r0, #1
	add r1, r7, #0
	and r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r3, #2]
	and r0, r2
	orr r0, r1
	strb r0, [r3, #2]
	mov r0, #0x3F
	and r6, r0
	lsl r1, r6, #4
	ldr r0, _08041F88 @ =0xFFFFFC0F
	ldrh r3, [r3, #2]
	and r0, r3
	orr r0, r1
	mov r1, r8
	strh r0, [r1, #2]
	ldr r0, _08041F8C @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r2, _08041F90 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08041F70
	cmp r0, #0x15
	blt _08041F70
	ldr r2, _08041F94 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _08041F98 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #7]
	lsr r0, r0, #6
	cmp r0, #0
	bne _08041EF4
_08041F70:
	mov r0, r8
	mov r1, #0
	mov r2, #0
	bl sub_0802CE38
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_08041F7E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08041F88: .4byte 0xFFFFFC0F
_08041F8C: .4byte 0x000007FF
_08041F90: .4byte gUnk_08621DE0
_08041F94: .4byte 0x020192E4
_08041F98: .4byte 0x00000D64
	thumb_func_end sub_08041DC4

