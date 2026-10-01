	thumb_func_start sub_0804BAAC
sub_0804BAAC: @ 0x0804BAAC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	ldr r0, _0804BAD0 @ =0x020192E0
	ldr r1, _0804BAD4 @ =0x00001B16
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x17
	lsr r0, r0, #0x18
	cmp r0, #4
	bls _0804BAC6
	b _0804BC6A
_0804BAC6:
	lsl r0, r0, #2
	ldr r1, _0804BAD8 @ =0x0804BADC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0804BAD0: .4byte 0x020192E0
_0804BAD4: .4byte 0x00001B16
_0804BAD8: .4byte 0x0804BADC
_0804BADC:
	.4byte _0804BAF0
	.4byte _0804BB78
	.4byte _0804BBF0
	.4byte _0804BBFE
	.4byte _0804BC5C
_0804BAF0:
	mov r6, #0
	ldr r5, _0804BB08 @ =0x02018450
	mov r3, #0xA6
	lsl r3, r3, #1
	add r3, r3, r5
	mov r8, r3
_0804BAFC:
	cmp r6, r7
	bne _0804BB0C
	ldrh r4, [r5]
	lsl r0, r4, #0x17
	b _0804BB10
	.align 2, 0
_0804BB08: .4byte 0x02018450
_0804BB0C:
	ldrb r1, [r5, #1]
	lsl r0, r1, #0x1C
_0804BB10:
	lsr r4, r0, #0x1D
	add r0, r6, #0
	bl sub_08008860
	lsl r1, r6, #1
	mov r3, #0xA4
	lsl r3, r3, #1
	add r2, r5, r3
	add r1, r1, r2
	strh r0, [r1]
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _0804BB68 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r3, _0804BB6C @ =0x0201930C
	add r1, r1, r3
	ldrh r0, [r1, #4]
	mov r4, r8
	strh r0, [r4]
	mov r0, #2
	add r8, r0
	add r6, #1
	cmp r6, #1
	ble _0804BAFC
	ldr r1, _0804BB70 @ =0x00001AEA
	add r3, r3, r1
	ldrh r2, [r3]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804BB74 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r3]
_0804BB62:
	mov r0, #0
	b _0804BC6C
	.align 2, 0
_0804BB68: .4byte 0x00000D64
_0804BB6C: .4byte 0x0201930C
_0804BB70: .4byte 0x00001AEA
_0804BB74: .4byte 0xFFFFFE01
_0804BB78:
	mov r6, #1
	sub r4, r6, r7
	mov r5, #0xB2
	lsl r5, r5, #3
	add r0, r4, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	beq _0804BBE0
	mov r2, #1
	neg r2, r2
	add r0, r4, #0
	add r1, r5, #0
	bl sub_080083BC
	add r2, r0, #0
	lsl r3, r5, #1
	ldr r0, _0804BBCC @ =0x08623DF4
	add r3, r3, r0
	and r4, r6
	lsl r0, r4, #0x1F
	mov r1, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	ldr r2, _0804BBD0 @ =0x20200000
	orr r1, r2
	orr r0, r1
	ldrh r3, [r3]
	orr r0, r3
	mov r1, #0
	bl sub_0801FBCC
	ldr r2, _0804BBD4 @ =0x020192E0
	ldr r1, _0804BBD8 @ =0x00001B16
	add r2, r2, r1
	ldr r0, _0804BBDC @ =0xFFFFFE01
	ldrh r3, [r2]
	and r0, r3
	mov r1, #8
	b _0804BC46
	.align 2, 0
_0804BBCC: .4byte gUnk_08623DF4
_0804BBD0: .4byte 0x20200000
_0804BBD4: .4byte 0x020192E0
_0804BBD8: .4byte 0x00001B16
_0804BBDC: .4byte 0xFFFFFE01
_0804BBE0:
	ldr r2, _0804BBE8 @ =0x020192E0
	ldr r4, _0804BBEC @ =0x00001B16
	add r2, r2, r4
	b _0804BC34
_0804BBE8: .4byte 0x020192E0
_0804BBEC: .4byte 0x00001B16
_0804BBF0:
	add r0, r7, #0
	bl sub_0804A99C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804BB62
	b _0804BC2E
_0804BBFE:
	mov r0, #1
	sub r0, r0, r7
	lsl r2, r7, #0x18
	lsr r2, r2, #0x18
	ldr r3, _0804BC4C @ =0x02018450
	ldrh r4, [r3]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x1D
	lsl r1, r1, #8
	orr r2, r1
	mov r1, #1
	sub r1, r1, r7
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldrb r3, [r3, #1]
	lsl r3, r3, #0x1C
	lsr r3, r3, #0x1D
	lsl r3, r3, #8
	orr r1, r3
	lsl r1, r1, #0x10
	orr r2, r1
	mov r1, #0x10
	bl sub_08042AB0
_0804BC2E:
	ldr r2, _0804BC50 @ =0x020192E0
	ldr r0, _0804BC54 @ =0x00001B16
	add r2, r2, r0
_0804BC34:
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804BC58 @ =0xFFFFFE01
	and r0, r3
_0804BC46:
	orr r0, r1
	strh r0, [r2]
	b _0804BB62
_0804BC4C: .4byte 0x02018450
_0804BC50: .4byte 0x020192E0
_0804BC54: .4byte 0x00001B16
_0804BC58: .4byte 0xFFFFFE01
_0804BC5C:
	add r0, r7, #0
	bl sub_0804A99C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804BC6A
	b _0804BB62
_0804BC6A:
	mov r0, #1
_0804BC6C:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0804BAAC
	.align 2, 0

