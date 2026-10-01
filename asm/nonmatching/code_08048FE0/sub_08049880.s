	thumb_func_start sub_08049880
sub_08049880: @ 0x08049880
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x14
	add r5, r1, #0
	add r4, r2, #0
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	mul r0, r4
	ldr r1, _08049904 @ =0x00000D64
	add r6, r2, #0
	mul r6, r1
	add r0, r0, r6
	ldr r1, _08049908 @ =0x0201930C
	mov ip, r1
	add r0, ip
	ldrb r0, [r0, #7]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1F
	mov r8, r0
	mov r3, sp
	mov r0, #1
	add r1, r5, #0
	and r1, r0
	ldrb r2, [r3, #2]
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #2]
	mov r0, sp
	strh r7, [r0]
	ldr r2, _0804990C @ =0x000007FF
	add r0, r7, #0
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _08049910 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08049914 @ =0x000002DA
	cmp r1, r0
	bne _080498DC
	b _08049AD8
_080498DC:
	cmp r1, r0
	bgt _08049964
	ldr r0, _08049918 @ =0x00000191
	cmp r1, r0
	bne _080498E8
	b _080499F4
_080498E8:
	cmp r1, r0
	bgt _08049930
	cmp r1, #0x58
	bne _080498F2
	b _080499D8
_080498F2:
	cmp r1, #0x58
	bgt _0804991C
	cmp r1, #0xF
	bne _080498FC
	b _080499F4
_080498FC:
	cmp r1, #0x51
	bne _08049902
	b _08049A28
_08049902:
	b _08049B64
_08049904: .4byte 0x00000D64
_08049908: .4byte 0x0201930C
_0804990C: .4byte 0x000007FF
_08049910: .4byte gUnk_08622AB4
_08049914: .4byte 0x000002DA
_08049918: .4byte 0x00000191
_0804991C:
	ldr r0, _0804992C @ =0x00000105
	cmp r1, r0
	beq _080499E0
	add r0, #0x81
	cmp r1, r0
	bne _0804992A
	b _08049A28
_0804992A:
	b _08049B64
_0804992C: .4byte 0x00000105
_08049930:
	mov r0, #0xD6
	lsl r0, r0, #1
	cmp r1, r0
	beq _080499F4
	cmp r1, r0
	bgt _08049944
	sub r0, #0xC
	cmp r1, r0
	beq _080499FC
	b _080499B8
_08049944:
	ldr r0, _08049954 @ =0x000001FF
	cmp r1, r0
	beq _080499D8
	cmp r1, r0
	bgt _08049958
	sub r0, #6
	b _080499BA
	.align 2, 0
_08049954: .4byte 0x000001FF
_08049958:
	ldr r0, _08049960 @ =0x00000243
	cmp r1, r0
	beq _080499FC
	b _08049B64
_08049960: .4byte 0x00000243
_08049964:
	ldr r0, _08049984 @ =0x00000599
	cmp r1, r0
	bgt _080499A8
	sub r0, #1
	cmp r1, r0
	bge _080499F4
	ldr r0, _08049988 @ =0x0000034D
	cmp r1, r0
	beq _080499F4
	cmp r1, r0
	bgt _0804998C
	sub r0, #0x72
	cmp r1, r0
	beq _080499FC
	add r0, #0xB
	b _080499BA
_08049984: .4byte 0x00000599
_08049988: .4byte 0x0000034D
_0804998C:
	ldr r0, _0804999C @ =0x00000536
	cmp r1, r0
	bne _08049994
	b _08049AD8
_08049994:
	cmp r1, r0
	bgt _080499A0
	sub r0, #0xDE
	b _080499BA
_0804999C: .4byte 0x00000536
_080499A0:
	ldr r0, _080499A4 @ =0x00000596
	b _080499BA
_080499A4: .4byte 0x00000596
_080499A8:
	ldr r0, _080499C0 @ =0x000005A4
	cmp r1, r0
	beq _080499F4
	cmp r1, r0
	bgt _080499C4
	sub r0, #5
	cmp r1, r0
	beq _080499F4
_080499B8:
	add r0, #3
_080499BA:
	cmp r1, r0
	beq _080499F4
	b _08049B64
_080499C0: .4byte 0x000005A4
_080499C4:
	ldr r0, _080499D4 @ =0x000005E6
	cmp r1, r0
	beq _080499F4
	add r0, #3
	cmp r1, r0
	bne _080499D2
	b _08049B14
_080499D2:
	b _08049B64
_080499D4: .4byte 0x000005E6
_080499D8:
	mov r1, #1
	neg r1, r1
	add r0, r5, #0
	b _080499E4
_080499E0:
	add r0, r5, #0
	add r1, r4, #0
_080499E4:
	bl sub_08008AF8
	mov r1, #0
	cmp r0, #0
	ble _080499F0
	mov r1, #1
_080499F0:
	add r0, r1, #0
	b _08049B66
_080499F4:
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	b _08049A14
_080499FC:
	ldr r1, _08049A20 @ =0x020192E0
	ldr r6, _08049A24 @ =0x00001B12
	add r1, r1, r6
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #4
	beq _08049A0E
	b _08049B64
_08049A0E:
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #2
_08049A14:
	bl sub_0802CFD0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08049B66
	.align 2, 0
_08049A20: .4byte 0x020192E0
_08049A24: .4byte 0x00001B12
_08049A28:
	ldr r2, _08049A54 @ =0x00000291
	add r0, r5, #0
	add r1, r4, #0
	bl sub_0800A78C
	cmp r0, #0
	bne _08049A38
	b _08049B64
_08049A38:
	mov r6, #0
	ldr r0, _08049A58 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _08049A5C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #0x51
	beq _08049A60
	mov r0, #0xC3
	lsl r0, r0, #1
	cmp r1, r0
	beq _08049A68
	b _08049A6A
_08049A54: .4byte 0x00000291
_08049A58: .4byte 0x000007FF
_08049A5C: .4byte gUnk_08622AB4
_08049A60:
	ldr r6, _08049A64 @ =0x000002E5
	b _08049A6E
_08049A64: .4byte 0x000002E5
_08049A68:
	ldr r6, _08049AB4 @ =0x00000187
_08049A6A:
	cmp r6, #0
	ble _08049B64
_08049A6E:
	mov r4, #0
	ldr r3, _08049AB8 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _08049ABC @ =0x00000D64
	mul r1, r0
	add r2, r1, r3
	ldrb r5, [r2, #3]
	cmp r4, r5
	bge _08049B64
	ldr r5, _08049AC0 @ =0x000007C4
	add r0, r3, r5
	add r1, r1, r0
_08049A88:
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r3, _08049AC4 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, r6
	bne _08049ACC
	mov r0, #0
	ldr r1, _08049AC8 @ =0x0000058A
	bl sub_08008524
	cmp r0, #0
	bgt _08049B64
	mov r0, #1
	ldr r1, _08049AC8 @ =0x0000058A
	bl sub_08008524
	cmp r0, #0
	bgt _08049B64
_08049AB0:
	mov r0, #1
	b _08049B66
_08049AB4: .4byte 0x00000187
_08049AB8: .4byte 0x020192E4
_08049ABC: .4byte 0x00000D64
_08049AC0: .4byte 0x000007C4
_08049AC4: .4byte gUnk_08622AB4
_08049AC8: .4byte 0x0000058A
_08049ACC:
	add r1, #4
	add r4, #1
	ldrb r5, [r2, #3]
	cmp r4, r5
	blt _08049A88
	b _08049B64
_08049AD8:
	add r0, r5, #0
	add r1, r4, #0
	bl sub_0800A430
	ldr r1, _08049B10 @ =0x0000FFFF
	cmp r0, r1
	bne _08049B64
	add r0, r5, #0
	bl sub_08008C6C
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _08049B64
	mov r4, #0
	mov r6, #1
_08049AF8:
	add r0, r7, #0
	sub r1, r6, r5
	add r2, r4, #0
	bl sub_0802B1B8
	cmp r0, #0
	bne _08049B60
	add r4, #1
	cmp r4, #4
	ble _08049AF8
	b _08049B64
	.align 2, 0
_08049B10: .4byte 0x0000FFFF
_08049B14:
	mov r0, r8
	cmp r0, #0
	beq _08049B64
	mov r4, #0
	mov r0, ip
	sub r0, #0x28
	add r0, r6, r0
	ldrb r1, [r0, #4]
	cmp r4, r1
	bge _08049B64
	ldr r0, _08049B58 @ =0x000008DC
	add r0, ip
	add r5, r2, #0
	add r2, r6, r0
	mov r3, #0xF8
	lsl r3, r3, #0x11
_08049B34:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #2
	ldr r6, _08049B5C @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08049AB0
	add r2, #4
	add r4, #1
	cmp r4, r1
	blt _08049B34
	b _08049B64
	.align 2, 0
_08049B58: .4byte 0x000008DC
_08049B5C: .4byte gUnk_08621DE0
_08049B60:
	mov r0, r8
	b _08049B66
_08049B64:
	mov r0, #0
_08049B66:
	add sp, #0x14
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08049880
	.align 2, 0

