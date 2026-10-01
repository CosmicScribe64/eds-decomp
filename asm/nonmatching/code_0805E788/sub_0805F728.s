	thumb_func_start sub_0805F728
sub_0805F728: @ 0x0805F728
	push {r4, lr}
	sub sp, #0xC
	mov r2, sp
	bl sub_0800ABC8
	mov r0, sp
	ldrh r0, [r0]
	mov r1, #0
	bl sub_0805F074
	ldr r1, _0805F760 @ =0x02011C20
	mov r0, #0x7F
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805F76C
	ldr r1, _0805F764 @ =0x0808649C
	mov r0, #0x18
	mov r2, #5
	bl sub_0805EE30
	ldr r1, _0805F768 @ =0x080864A4
	mov r0, #0x38
	mov r2, #4
	bl sub_0805EE30
	b _0805F780
	.align 2, 0
_0805F760: .4byte 0x02011C20
_0805F764: .4byte gUnk_0808649C
_0805F768: .4byte gUnk_080864A4
_0805F76C:
	ldr r1, _0805F7AC @ =0x080864AC
	mov r0, #0x18
	mov r2, #5
	bl sub_0805EE30
	ldr r1, _0805F7B0 @ =0x080864B4
	mov r0, #0x38
	mov r2, #4
	bl sub_0805EE30
_0805F780:
	ldr r4, [sp, #4]
	mov r0, sp
	ldrh r2, [r0]
	ldr r0, _0805F7B4 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0805F7B8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r3, r4, #0
	cmp r0, #0x15
	blt _0805F7C6
	cmp r0, #0x17
	ble _0805F7BC
	cmp r0, #0x18
	beq _0805F7C0
	b _0805F7C6
	.align 2, 0
_0805F7AC: .4byte gUnk_080864AC
_0805F7B0: .4byte gUnk_080864B4
_0805F7B4: .4byte 0x000007FF
_0805F7B8: .4byte gUnk_08621DE0
_0805F7BC:
	mov r0, #0
	b _0805F7DC
_0805F7C0:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805F7DC
_0805F7C6:
	ldr r0, _0805F818 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0805F81C @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805F7DC:
	mov r2, #7
	cmp r0, r3
	beq _0805F7E4
	mov r2, #6
_0805F7E4:
	mov r0, #0x19
	add r1, r4, #0
	mov r3, #5
	bl sub_0805EE78
	ldr r3, [sp, #8]
	mov r0, sp
	ldrh r2, [r0]
	ldr r0, _0805F818 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0805F81C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F82A
	cmp r0, #0x17
	ble _0805F820
	cmp r0, #0x18
	beq _0805F824
	b _0805F82A
	.align 2, 0
_0805F818: .4byte 0x000007FF
_0805F81C: .4byte gUnk_08621DE0
_0805F820:
	mov r0, #0
	b _0805F840
_0805F824:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805F840
_0805F82A:
	ldr r0, _0805F884 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0805F888 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _0805F88C @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805F840:
	add r1, r0, #0
	ldr r0, [sp, #8]
	mov r2, #7
	cmp r1, r0
	beq _0805F84C
	mov r2, #6
_0805F84C:
	mov r0, #0x39
	add r1, r3, #0
	mov r3, #5
	bl sub_0805EE78
	mov r0, #0x17
	mov r1, #3
	bl sub_0805EEDC
	mov r0, sp
	ldrh r2, [r0]
	ldr r0, _0805F884 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0805F888 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F898
	cmp r0, #0x17
	ble _0805F890
	cmp r0, #0x18
	beq _0805F894
	b _0805F898
_0805F884: .4byte 0x000007FF
_0805F888: .4byte gUnk_08621DE0
_0805F88C: .4byte 0x000001FF
_0805F890:
	mov r0, #0
	b _0805F8AC
_0805F894:
	mov r0, #0xA
	b _0805F8AC
_0805F898:
	ldr r0, _0805F8E0 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0805F8E4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805F8AC:
	add r1, r0, #0
	mov r0, #0x36
	mov r2, #7
	mov r3, #2
	bl sub_0805EE78
	mov r4, #0x13
	mov r0, sp
	ldrh r2, [r0]
	ldr r0, _0805F8E0 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0805F8E4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F8F0
	cmp r0, #0x17
	ble _0805F8E8
	cmp r0, #0x18
	beq _0805F8EC
	b _0805F8F0
_0805F8E0: .4byte 0x000007FF
_0805F8E4: .4byte gUnk_08621DE0
_0805F8E8:
	mov r0, #0
	b _0805F904
_0805F8EC:
	mov r0, #0xA
	b _0805F904
_0805F8F0:
	ldr r0, _0805F95C @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0805F960 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805F904:
	cmp r0, #9
	bls _0805F90A
	sub r4, #1
_0805F90A:
	mov r0, sp
	ldrb r1, [r0, #2]
	mov r0, #0x1F
	and r0, r1
	cmp r0, #0
	beq _0805F92E
	lsl r1, r1, #0x1B
	lsr r0, r1, #0x1B
	cmp r0, #0x14
	bhi _0805F92E
	ldr r0, _0805F964 @ =0x081A41A4
	lsr r1, r1, #0x19
	add r1, r1, r0
	ldr r2, [r1]
	add r0, r4, #0
	mov r1, #9
	bl sub_0805EF00
_0805F92E:
	mov r0, sp
	ldrb r1, [r0, #2]
	mov r0, #0xE0
	and r0, r1
	cmp r0, #0
	beq _0805F954
	lsl r1, r1, #0x18
	lsr r0, r1, #0x1D
	cmp r0, #6
	bhi _0805F954
	add r0, r4, #2
	ldr r2, _0805F968 @ =0x081A41F8
	lsr r1, r1, #0x1D
	lsl r1, r1, #2
	add r1, r1, r2
	ldr r2, [r1]
	mov r1, #0xA
	bl sub_0805EF00
_0805F954:
	add sp, #0xC
	pop {r4}
	pop {r0}
	bx r0
_0805F95C: .4byte 0x000007FF
_0805F960: .4byte gUnk_08621DE0
_0805F964: .4byte gUnk_081A41A4
_0805F968: .4byte gUnk_081A41F8
	thumb_func_end sub_0805F728

