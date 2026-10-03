	thumb_func_start CanRedirectEffectToZone
CanRedirectEffectToZone: @ 0x0802F5F8
	push {r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r6, r2, #0
	add r7, r3, #0
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	cmp r5, #0
	bne _0802F60A
	b _0802F7DC
_0802F60A:
	ldr r0, _0802F680 @ =0x00000431
	cmp r1, r0
	bne _0802F62C
	ldr r0, _0802F684 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802F688 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0802F62C
	b _0802F7DC
_0802F62C:
	ldr r0, _0802F684 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0802F68C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802F690 @ =0x00000434
	cmp r1, r0
	bgt _0802F716
	sub r0, #1
	cmp r1, r0
	blt _0802F648
	b _0802F7BC
_0802F648:
	sub r0, #0x3E
	cmp r1, r0
	bgt _0802F6C0
	sub r0, #1
	cmp r1, r0
	blt _0802F656
	b _0802F7BC
_0802F656:
	ldr r0, _0802F694 @ =0x0000028B
	cmp r1, r0
	bne _0802F65E
	b _0802F7BC
_0802F65E:
	cmp r1, r0
	bgt _0802F698
	mov r0, #0x96
	lsl r0, r0, #1
	cmp r1, r0
	bge _0802F66C
	b _0802F7DC
_0802F66C:
	add r0, #0x10
	cmp r1, r0
	bgt _0802F674
	b _0802F7BC
_0802F674:
	add r0, #0xB
	cmp r1, r0
	ble _0802F67C
	b _0802F7DC
_0802F67C:
	sub r0, #9
	b _0802F6EE
_0802F680: .4byte 0x00000431
_0802F684: .4byte 0x000007FF
_0802F688: .4byte gCardStats
_0802F68C: .4byte gCardIdToNumber
_0802F690: .4byte 0x00000434
_0802F694: .4byte 0x0000028B
_0802F698:
	mov r0, #0xA4
	lsl r0, r0, #2
	cmp r1, r0
	bne _0802F6A2
	b _0802F7BC
_0802F6A2:
	cmp r1, r0
	bgt _0802F6AA
	sub r0, #3
	b _0802F7A6
_0802F6AA:
	ldr r0, _0802F6B8 @ =0x0000029B
	cmp r1, r0
	bne _0802F6B2
	b _0802F7BC
_0802F6B2:
	ldr r0, _0802F6BC @ =0x000003C2
	b _0802F7A6
	.align 2, 0
_0802F6B8: .4byte 0x0000029B
_0802F6BC: .4byte 0x000003C2
_0802F6C0:
	ldr r0, _0802F6E0 @ =0x00000417
	cmp r1, r0
	bgt _0802F6FC
	sub r0, #1
	cmp r1, r0
	blt _0802F6CE
	b _0802F7BC
_0802F6CE:
	sub r0, #0x13
	cmp r1, r0
	bne _0802F6D6
	b _0802F7BC
_0802F6D6:
	cmp r1, r0
	bgt _0802F6E4
	sub r0, #4
	b _0802F7A6
	.align 2, 0
_0802F6E0: .4byte 0x00000417
_0802F6E4:
	ldr r0, _0802F6F8 @ =0x00000413
	cmp r1, r0
	ble _0802F6EC
	b _0802F7DC
_0802F6EC:
	sub r0, #1
_0802F6EE:
	cmp r1, r0
	bge _0802F6F4
	b _0802F7DC
_0802F6F4:
	b _0802F7BC
	.align 2, 0
_0802F6F8: .4byte 0x00000413
_0802F6FC:
	ldr r0, _0802F70C @ =0x00000424
	cmp r1, r0
	beq _0802F7BC
	cmp r1, r0
	bgt _0802F710
	sub r0, #2
	b _0802F7A6
	.align 2, 0
_0802F70C: .4byte 0x00000424
_0802F710:
	mov r0, #0x86
	lsl r0, r0, #3
	b _0802F7A6
_0802F716:
	ldr r0, _0802F73C @ =0x0000058C
	cmp r1, r0
	bgt _0802F76C
	sub r0, #1
	cmp r1, r0
	bge _0802F7BC
	sub r0, #0xC7
	cmp r1, r0
	beq _0802F7BC
	cmp r1, r0
	bgt _0802F748
	sub r0, #0x26
	cmp r1, r0
	beq _0802F7BC
	cmp r1, r0
	bgt _0802F740
	sub r0, #0x19
	b _0802F7A6
	.align 2, 0
_0802F73C: .4byte 0x0000058C
_0802F740:
	ldr r0, _0802F744 @ =0x000004BB
	b _0802F7A6
_0802F744: .4byte 0x000004BB
_0802F748:
	ldr r0, _0802F758 @ =0x00000515
	cmp r1, r0
	beq _0802F7BC
	cmp r1, r0
	bgt _0802F75C
	sub r0, #0x39
	b _0802F7A6
	.align 2, 0
_0802F758: .4byte 0x00000515
_0802F75C:
	ldr r0, _0802F768 @ =0x00000521
	cmp r1, r0
	beq _0802F7BC
	add r0, #0x66
	b _0802F7A6
	.align 2, 0
_0802F768: .4byte 0x00000521
_0802F76C:
	ldr r0, _0802F788 @ =0x000005F9
	cmp r1, r0
	beq _0802F7BC
	cmp r1, r0
	bgt _0802F794
	sub r0, #0x4E
	cmp r1, r0
	bgt _0802F78C
	sub r0, #3
	cmp r1, r0
	bge _0802F7BC
	sub r0, #0x1A
	b _0802F7A6
	.align 2, 0
_0802F788: .4byte 0x000005F9
_0802F78C:
	ldr r0, _0802F790 @ =0x000005F7
	b _0802F7A6
_0802F790: .4byte 0x000005F7
_0802F794:
	ldr r0, _0802F7AC @ =0x0000060A
	cmp r1, r0
	beq _0802F7BC
	cmp r1, r0
	bgt _0802F7B0
	sub r0, #0xE
	cmp r1, r0
	beq _0802F7BC
	add r0, #8
_0802F7A6:
	cmp r1, r0
	beq _0802F7BC
	b _0802F7DC
_0802F7AC: .4byte 0x0000060A
_0802F7B0:
	ldr r0, _0802F7E0 @ =0x0000060C
	cmp r1, r0
	beq _0802F7BC
	add r0, #2
	cmp r1, r0
	bne _0802F7DC
_0802F7BC:
	ldrb r4, [r5, #0xC]
	ldrh r0, [r5, #0xC]
	lsr r3, r0, #8
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	mul r0, r7
	ldr r1, _0802F7E4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802F7E8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0802F7EC
_0802F7DC:
	mov r0, #0
	b _0802F802
_0802F7E0: .4byte 0x0000060C
_0802F7E4: .4byte 0x00000D64
_0802F7E8: .4byte 0x0201930C
_0802F7EC:
	cmp r6, r4
	bne _0802F7F4
	cmp r7, r3
	beq _0802F7DC
_0802F7F4:
	add r0, r5, #0
	add r1, r6, #0
	add r2, r7, #0
	bl CanEffectTargetZone
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_0802F802:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CanRedirectEffectToZone

