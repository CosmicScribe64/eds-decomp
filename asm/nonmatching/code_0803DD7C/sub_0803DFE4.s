	thumb_func_start sub_0803DFE4
sub_0803DFE4: @ 0x0803DFE4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	mov r2, #1
	ldrb r0, [r7, #2]
	and r2, r0
	cmp r2, #0
	beq _0803E0C0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r7, #0xA]
	and r0, r1
	strb r0, [r7, #0xA]
	mov r5, #0
	mov r2, #1
	mov r9, r2
	ldr r3, _0803E0B0 @ =0x00000D64
	mov r8, r3
_0803E00C:
	mov r4, #5
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r6, r8
	mul r6, r0
_0803E018:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0803E0B4 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0803E060
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803E060
	ldr r3, _0803E0B8 @ =0x000007FF
	add r0, r3, #0
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0803E0BC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0803E060
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803E0AA
_0803E060:
	add r4, #1
	cmp r4, #9
	ble _0803E018
	mov r4, #5
	mov r0, #1
	and r0, r5
	ldr r1, _0803E0B0 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_0803E072:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0803E0B4 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803E09E
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803E09E
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803E0AA
_0803E09E:
	add r4, #1
	cmp r4, #9
	ble _0803E072
	add r5, #1
	cmp r5, #1
	ble _0803E00C
_0803E0AA:
	mov r0, #1
	b _0803E154
	.align 2, 0
_0803E0B0: .4byte 0x00000D64
_0803E0B4: .4byte 0x0201930C
_0803E0B8: .4byte 0x000007FF
_0803E0BC: .4byte gUnk_08621DE0
_0803E0C0:
	ldr r0, _0803E0EC @ =0x02017A40
	ldr r3, _0803E0F0 @ =0x000003E5
	add r4, r0, r3
	ldrb r0, [r4]
	cmp r0, #0
	bne _0803E100
	ldr r0, _0803E0F4 @ =0x00000206
	ldr r1, _0803E0F8 @ =0x00000712
	ldr r3, _0803E0FC @ =0x08083C94
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r1, [r7, #0xA]
	and r0, r1
	strb r0, [r7, #0xA]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0803E152
	.align 2, 0
_0803E0EC: .4byte 0x02017A40
_0803E0F0: .4byte 0x000003E5
_0803E0F4: .4byte 0x00000206
_0803E0F8: .4byte 0x00000712
_0803E0FC: .4byte gUnk_08083C94
_0803E100:
	ldr r1, _0803E13C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803E150
	ldr r0, _0803E140 @ =0x000A000A
	bl sub_08052F38
	cmp r0, #0
	beq _0803E152
	ldr r0, _0803E144 @ =0x0201CFB0
	ldr r2, _0803E148 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803E14C @ =0x00000828
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r7, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803E152
	b _0803E0AA
	.align 2, 0
_0803E13C: .4byte 0x03000040
_0803E140: .4byte 0x000A000A
_0803E144: .4byte 0x0201CFB0
_0803E148: .4byte 0x00000824
_0803E14C: .4byte 0x00000828
_0803E150:
	strb r2, [r4]
_0803E152:
	mov r0, #0
_0803E154:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803DFE4

