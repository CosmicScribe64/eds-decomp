	thumb_func_start sub_0805F074
sub_0805F074: @ 0x0805F074
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	lsl r0, r7, #6
	ldr r1, _0805F114 @ =0x0822C720
	add r6, r0, r1
	add r0, r6, #0
	bl sub_080753E0
	mov r4, #0xC
	cmp r0, #0xC
	ble _0805F098
	mov r4, #0xA
_0805F098:
	mov r0, #0x20
	mov r1, #2
	bl sub_08074B08
	lsr r5, r4, #1
	mov r1, #9
	sub r1, r1, r5
	lsl r4, r4, #8
	mov r0, #8
	add r2, r4, #0
	orr r2, r0
	mov r0, #3
	add r3, r6, #0
	bl sub_0807501C
	mov r1, #8
	sub r1, r1, r5
	mov r0, #7
	orr r4, r0
	mov r0, #2
	add r2, r4, #0
	add r3, r6, #0
	bl sub_0807501C
	ldr r0, _0805F118 @ =0x0201CFB8
	mov r1, #9
	bl sub_08075114
	ldr r0, _0805F11C @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0805F120 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0805F0EA
	b _0805F25C
_0805F0EA:
	mov r0, r8
	cmp r0, #0
	bne _0805F0F2
	b _0805F25C
_0805F0F2:
	ldr r1, _0805F124 @ =0x02011C20
	mov r0, #0x7F
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805F130
	ldr r1, _0805F128 @ =0x08086470
	mov r0, #0x17
	mov r2, #5
	bl sub_0805EE30
	ldr r1, _0805F12C @ =0x08086478
	mov r0, #0x37
	mov r2, #4
	bl sub_0805EE30
	b _0805F144
_0805F114: .4byte gUnk_0822C720
_0805F118: .4byte 0x0201CFB8
_0805F11C: .4byte 0x000007FF
_0805F120: .4byte gUnk_08621DE0
_0805F124: .4byte 0x02011C20
_0805F128: .4byte gUnk_08086470
_0805F12C: .4byte gUnk_08086478
_0805F130:
	ldr r1, _0805F168 @ =0x08086480
	mov r0, #0x17
	mov r2, #5
	bl sub_0805EE30
	ldr r1, _0805F16C @ =0x08086488
	mov r0, #0x37
	mov r2, #4
	bl sub_0805EE30
_0805F144:
	ldr r0, _0805F170 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0805F174 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F182
	cmp r0, #0x17
	ble _0805F178
	cmp r0, #0x18
	beq _0805F17C
	b _0805F182
	.align 2, 0
_0805F168: .4byte gUnk_08086480
_0805F16C: .4byte gUnk_08086488
_0805F170: .4byte 0x000007FF
_0805F174: .4byte gUnk_08621DE0
_0805F178:
	mov r0, #0
	b _0805F198
_0805F17C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805F198
_0805F182:
	ldr r0, _0805F1C8 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0805F1CC @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805F198:
	add r1, r0, #0
	mov r0, #0x1A
	mov r2, #7
	mov r3, #4
	bl sub_0805EE78
	ldr r0, _0805F1C8 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0805F1CC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F1DA
	cmp r0, #0x17
	ble _0805F1D0
	cmp r0, #0x18
	beq _0805F1D4
	b _0805F1DA
	.align 2, 0
_0805F1C8: .4byte 0x000007FF
_0805F1CC: .4byte gUnk_08621DE0
_0805F1D0:
	mov r0, #0
	b _0805F1F0
_0805F1D4:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805F1F0
_0805F1DA:
	ldr r0, _0805F228 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0805F22C @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _0805F230 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805F1F0:
	add r1, r0, #0
	mov r0, #0x3A
	mov r2, #7
	mov r3, #4
	bl sub_0805EE78
	mov r0, #0x18
	mov r1, #3
	bl sub_0805EEDC
	ldr r0, _0805F228 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0805F22C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F23C
	cmp r0, #0x17
	ble _0805F234
	cmp r0, #0x18
	beq _0805F238
	b _0805F23C
	.align 2, 0
_0805F228: .4byte 0x000007FF
_0805F22C: .4byte gUnk_08621DE0
_0805F230: .4byte 0x000001FF
_0805F234:
	mov r0, #0
	b _0805F250
_0805F238:
	mov r0, #0xA
	b _0805F250
_0805F23C:
	ldr r0, _0805F268 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0805F26C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805F250:
	add r1, r0, #0
	mov r0, #0x37
	mov r2, #7
	mov r3, #2
	bl sub_0805EE78
_0805F25C:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805F268: .4byte 0x000007FF
_0805F26C: .4byte gUnk_08621DE0
	thumb_func_end sub_0805F074

