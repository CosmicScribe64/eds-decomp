	thumb_func_start sub_0804B640
sub_0804B640: @ 0x0804B640
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	add r7, r0, #0
	mov r2, #1
	and r2, r7
	ldr r4, _0804B68C @ =0x02018450
	ldrh r0, [r4]
	lsl r3, r0, #0x17
	lsr r1, r3, #0x1D
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0804B690 @ =0x00000D64
	add r5, r2, #0
	mul r5, r1
	mov r8, r5
	add r0, r8
	ldr r6, _0804B694 @ =0x0201930C
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r9, r0
	ldr r0, _0804B698 @ =0x00001AEA
	add r5, r6, r0
	ldrh r1, [r5]
	lsl r0, r1, #0x17
	lsr r0, r0, #0x18
	cmp r0, #0x64
	bne _0804B682
	b _0804B8D0
_0804B682:
	cmp r0, #0x64
	bgt _0804B69C
	cmp r0, #0
	beq _0804B6AA
	b _0804B9FA
_0804B68C: .4byte 0x02018450
_0804B690: .4byte 0x00000D64
_0804B694: .4byte 0x0201930C
_0804B698: .4byte 0x00001AEA
_0804B69C:
	cmp r0, #0x65
	bne _0804B6A2
	b _0804B94C
_0804B6A2:
	cmp r0, #0xC8
	bne _0804B6A8
	b _0804B9A0
_0804B6A8:
	b _0804B9FA
_0804B6AA:
	mov r0, #8
	ldrb r2, [r4]
	and r0, r2
	cmp r0, #0
	beq _0804B6B6
	b _0804B9FA
_0804B6B6:
	lsr r1, r3, #0x1D
	add r0, r7, #0
	mov r2, #1
	bl sub_0804A528
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804B6F8
	mov r0, #0x35
	cmp r7, #0
	beq _0804B6CE
	ldr r0, _0804B6EC @ =0x00008035
_0804B6CE:
	ldrh r4, [r4]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r3, _0804B6F0 @ =0x00001AE8
	add r2, r6, r3
	ldr r0, [r2]
	ldr r1, _0804B6F4 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #2
	b _0804B992
_0804B6EC: .4byte 0x00008035
_0804B6F0: .4byte 0x00001AE8
_0804B6F4: .4byte 0xFFFE01FF
_0804B6F8:
	mov r6, #0
	add r5, r4, #0
	mov r0, #0xA6
	lsl r0, r0, #1
	add r3, r5, r0
_0804B702:
	cmp r6, r7
	bne _0804B70C
	ldrh r1, [r5]
	lsl r0, r1, #0x17
	b _0804B710
_0804B70C:
	ldrb r2, [r5, #1]
	lsl r0, r2, #0x1C
_0804B710:
	lsr r4, r0, #0x1D
	add r0, r6, #0
	str r3, [sp, #0]
	bl sub_08008860
	lsl r1, r6, #1
	mov r2, #0xA4
	lsl r2, r2, #1
	add r2, r2, r5
	mov r8, r2
	add r1, r8
	strh r0, [r1]
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _0804B784 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0804B788 @ =0x0201930C
	mov r8, r0
	add r1, r8
	ldrh r0, [r1, #4]
	ldr r3, [sp, #0]
	strh r0, [r3]
	add r3, #2
	add r6, #1
	cmp r6, #1
	ble _0804B702
	mov r0, #8
	ldrb r1, [r5]
	orr r0, r1
	strb r0, [r5]
	ldr r0, _0804B78C @ =0x000007FF
	mov r2, r9
	and r2, r0
	lsl r0, r2, #1
	ldr r3, _0804B790 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	mov r0, #0xB6
	lsl r0, r0, #2
	add r4, r5, #0
	cmp r1, r0
	bgt _0804B794
	sub r0, #2
	cmp r1, r0
	bge _0804B7B4
	mov r0, #0xB7
	lsl r0, r0, #1
	cmp r1, r0
	bne _0804B77C
	b _0804B88C
_0804B77C:
	add r0, #0xB8
	cmp r1, r0
	beq _0804B7D4
	b _0804B9FA
_0804B784: .4byte 0x00000D64
_0804B788: .4byte 0x0201930C
_0804B78C: .4byte 0x000007FF
_0804B790: .4byte gUnk_08622AB4
_0804B794:
	ldr r0, _0804B7A8 @ =0x000002F9
	cmp r1, r0
	beq _0804B7E8
	cmp r1, r0
	bgt _0804B7AC
	sub r0, #0x11
	cmp r1, r0
	beq _0804B7E8
	b _0804B9FA
	.align 2, 0
_0804B7A8: .4byte 0x000002F9
_0804B7AC:
	ldr r0, _0804B7CC @ =0x000002FE
	cmp r1, r0
	beq _0804B7B4
	b _0804B9FA
_0804B7B4:
	mov r0, #0x43
	cmp r7, #0
	beq _0804B7BC
	ldr r0, _0804B7D0 @ =0x00008043
_0804B7BC:
	mov r1, #0xFA
	lsl r1, r1, #1
_0804B7C0:
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	b _0804B9FA
	.align 2, 0
_0804B7CC: .4byte 0x000002FE
_0804B7D0: .4byte 0x00008043
_0804B7D4:
	mov r0, #0x43
	cmp r7, #0
	beq _0804B7DC
	ldr r0, _0804B7E4 @ =0x00008043
_0804B7DC:
	mov r1, #0xFA
	lsl r1, r1, #2
	b _0804B7C0
	.align 2, 0
_0804B7E4: .4byte 0x00008043
_0804B7E8:
	cmp r7, #0
	beq _0804B858
	ldrh r5, [r4]
	lsl r0, r5, #0x17
	lsr r0, r0, #0x1D
	mov r1, #0
	bl sub_080563B8
	add r1, r0, #0
	cmp r1, #0
	blt _0804B806
	add r0, r7, #0
	bl sub_08017FF4
	b _0804B9FA
_0804B806:
	ldrh r4, [r4]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0804A39C
	ldr r2, _0804B844 @ =0x020192E0
	ldr r6, _0804B848 @ =0x00001B14
	add r4, r2, r6
	ldr r3, [r4]
	lsr r0, r3, #9
	lsl r0, r0, #0x18
	lsr r0, r0, #8
	mov r1, #0x80
	lsl r1, r1, #0xC
	add r0, r0, r1
	lsr r0, r0, #0x10
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #9
	ldr r1, _0804B84C @ =0xFFFE01FF
	and r1, r3
	orr r1, r0
	str r1, [r4]
	ldr r3, _0804B850 @ =0x00001B16
	add r2, r2, r3
	ldr r0, _0804B854 @ =0xFFFFFE01
	ldrh r5, [r2]
	and r0, r5
	b _0804B8B4
	.align 2, 0
_0804B844: .4byte 0x020192E0
_0804B848: .4byte 0x00001B14
_0804B84C: .4byte 0xFFFE01FF
_0804B850: .4byte 0x00001B16
_0804B854: .4byte 0xFFFFFE01
_0804B858:
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _0804B878 @ =0x00000715
	ldr r3, _0804B87C @ =0x080859E0
	mov r2, #0xB
	bl sub_080602A4
	ldr r2, _0804B880 @ =0x020192E0
	ldr r6, _0804B884 @ =0x00001B16
	add r2, r2, r6
	ldr r0, _0804B888 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0xC8
	b _0804B8B2
	.align 2, 0
_0804B878: .4byte 0x00000715
_0804B87C: .4byte gUnk_080859E0
_0804B880: .4byte 0x020192E0
_0804B884: .4byte 0x00001B16
_0804B888: .4byte 0xFFFFFE01
_0804B88C:
	ldr r0, _0804B8BC @ =0x00000206
	ldr r1, _0804B8C0 @ =0x00000613
	ldr r3, _0804B8C4 @ =0x08085A18
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r2, _0804B8C8 @ =0x00001AEA
	add r2, r8
	ldr r0, _0804B8CC @ =0xFFFFFE01
	ldrh r3, [r2]
	and r0, r3
	mov r5, #0xC8
	lsl r5, r5, #1
	add r1, r5, #0
_0804B8B2:
	orr r0, r1
_0804B8B4:
	strh r0, [r2]
_0804B8B6:
	mov r0, #0
	b _0804B9FC
	.align 2, 0
_0804B8BC: .4byte 0x00000206
_0804B8C0: .4byte 0x00000613
_0804B8C4: .4byte gUnk_08085A18
_0804B8C8: .4byte 0x00001AEA
_0804B8CC: .4byte 0xFFFFFE01
_0804B8D0:
	mov r0, #0xF0
	bl sub_08052F38
	cmp r0, #0
	beq _0804B8B6
	ldrh r4, [r4]
	lsl r0, r4, #0x17
	lsr r0, r0, #0x1D
	ldr r3, _0804B930 @ =0x0201CFB0
	ldr r6, _0804B934 @ =0x0000082C
	add r6, r6, r3
	mov r8, r6
	ldr r2, [r6]
	cmp r0, r2
	beq _0804B944
	mov r4, #8
	cmp r7, #0
	beq _0804B8F6
	ldr r4, _0804B938 @ =0x00008008
_0804B8F6:
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	ldr r6, _0804B93C @ =0x00000828
	add r0, r3, r6
	lsl r2, r2, #0x18
	lsr r2, r2, #0x10
	ldrb r0, [r0]
	orr r2, r0
	add r0, r4, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, r8
	ldr r1, [r0]
	add r0, r7, #0
	bl sub_08017FF4
	ldrh r2, [r5]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804B940 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r5]
	b _0804B8B6
_0804B930: .4byte 0x0201CFB0
_0804B934: .4byte 0x0000082C
_0804B938: .4byte 0x00008008
_0804B93C: .4byte 0x00000828
_0804B940: .4byte 0xFFFFFE01
_0804B944:
	mov r0, #3
	bl sub_08077AEC
	b _0804B8B6
_0804B94C:
	add r0, r7, #0
	bl sub_0804A99C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804B9FA
	ldrh r4, [r4]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	mov r2, #0
	bl sub_0804A528
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804B984
	ldr r1, _0804B97C @ =0x00001AE8
	add r2, r6, r1
	ldr r0, [r2]
	ldr r1, _0804B980 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #2
	b _0804B992
_0804B97C: .4byte 0x00001AE8
_0804B980: .4byte 0xFFFE01FF
_0804B984:
	ldr r3, _0804B998 @ =0x00001AE8
	add r2, r6, r3
	ldr r0, [r2]
	ldr r1, _0804B99C @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #3
_0804B992:
	orr r0, r1
	str r0, [r2]
	b _0804B8B6
_0804B998: .4byte 0x00001AE8
_0804B99C: .4byte 0xFFFE01FF
_0804B9A0:
	bl sub_08076F9C
	add r4, r0, #0
	mov r0, #1
	and r4, r0
	mov r0, #0xE0
	cmp r7, #0
	beq _0804B9B2
	ldr r0, _0804BA0C @ =0x000080E0
_0804B9B2:
	ldr r5, _0804BA10 @ =0x0201AE60
	ldrh r1, [r5, #0x14]
	add r2, r4, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x12
	cmp r7, #0
	beq _0804B9C6
	ldr r0, _0804BA14 @ =0x00008012
_0804B9C6:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldrh r5, [r5, #0x14]
	cmp r4, r5
	beq _0804B9FA
	mov r4, #0x43
	cmp r7, #0
	beq _0804B9DE
	ldr r4, _0804BA18 @ =0x00008043
_0804B9DE:
	add r0, r6, #0
	sub r0, #0x28
	add r0, r8
	ldrh r0, [r0]
	bl sub_080754A4
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r4, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_0804B9FA:
	mov r0, #1
_0804B9FC:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0804BA0C: .4byte 0x000080E0
_0804BA10: .4byte 0x0201AE60
_0804BA14: .4byte 0x00008012
_0804BA18: .4byte 0x00008043
	thumb_func_end sub_0804B640

