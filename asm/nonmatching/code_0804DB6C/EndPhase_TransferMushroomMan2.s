	thumb_func_start EndPhase_TransferMushroomMan2
EndPhase_TransferMushroomMan2: @ 0x0804E5B4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x80
	add r5, r0, #0
	ldr r7, _0804E5D4 @ =0x020192E0
	ldr r0, _0804E5D8 @ =0x00001B22
	add r4, r7, r0
	ldrb r0, [r4]
	cmp r0, #1
	beq _0804E6A0
	cmp r0, #1
	bgt _0804E5DC
	cmp r0, #0
	beq _0804E5E4
	b _0804E770
_0804E5D4: .4byte 0x020192E0
_0804E5D8: .4byte 0x00001B22
_0804E5DC:
	cmp r0, #2
	bne _0804E5E2
	b _0804E6E8
_0804E5E2:
	b _0804E770
_0804E5E4:
	add r2, r7, #4
	mov r3, #1
	add r0, r5, #0
	and r0, r3
	ldr r1, _0804E674 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
	add r2, r6, r2
	ldr r0, _0804E678 @ =0x000001F3
	ldrh r2, [r2]
	cmp r2, r0
	bhi _0804E5FE
	b _0804E770
_0804E5FE:
	sub r0, r3, r5
	bl CountFreeMonsterZones
	cmp r0, #0
	bne _0804E60A
	b _0804E770
_0804E60A:
	ldr r1, _0804E67C @ =0x00001B23
	add r0, r7, r1
	ldrb r2, [r0]
	cmp r2, #4
	bls _0804E616
	b _0804E770
_0804E616:
	mov r1, #1
	mov r8, r1
	add r2, r7, #0
	add r4, r0, #0
	ldr r0, _0804E680 @ =0x000007FF
	mov ip, r0
	mov r7, #0x8A
	lsl r7, r7, #2
_0804E626:
	mov r0, #0x94
	ldrb r1, [r4]
	mul r0, r1
	add r0, r0, r6
	add r1, r2, #0
	add r1, #0x2C
	add r3, r0, r1
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0804E690
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _0804E690
	mov r0, ip
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0804E684 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _0804E690
	ldr r0, _0804E688 @ =0x00001B22
	add r1, r2, r0
	ldrb r3, [r1]
	add r3, #1
	strb r3, [r1]
	cmp r5, #0
	beq _0804E6CE
	ldr r0, _0804E68C @ =0x0201AE60
	mov r2, r8
	strh r2, [r0, #0x14]
	add r0, r3, #1
	strb r0, [r1]
	b _0804E6CE
	.align 2, 0
_0804E674: .4byte 0x00000D64
_0804E678: .4byte 0x000001F3
_0804E67C: .4byte 0x00001B23
_0804E680: .4byte 0x000007FF
_0804E684: .4byte gCardIdToNumber
_0804E688: .4byte 0x00001B22
_0804E68C: .4byte 0x0201AE60
_0804E690:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #4
	bls _0804E626
	b _0804E770
_0804E6A0:
	ldr r1, _0804E6D4 @ =0x08085BD8
	ldr r0, _0804E6D8 @ =0x08624244
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _0804E6DC @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _0804E6E0 @ =0x00000206
	ldr r1, _0804E6E4 @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0804E6CE:
	mov r0, #0
	b _0804E772
	.align 2, 0
_0804E6D4: .4byte gStrAskTransferControlFmt
_0804E6D8: .4byte gUnk_08624244
_0804E6DC: .4byte gCardNames
_0804E6E0: .4byte 0x00000206
_0804E6E4: .4byte 0x00000712
_0804E6E8:
	ldr r0, _0804E74C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0804E730
	mov r0, #0x43
	cmp r5, #0
	beq _0804E6F8
	ldr r0, _0804E750 @ =0x00008043
_0804E6F8:
	mov r1, #0xFA
	lsl r1, r1, #1
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	lsl r4, r5, #0x18
	lsr r4, r4, #0x18
	ldr r1, _0804E754 @ =0x00001B23
	add r0, r7, r1
	ldrb r0, [r0]
	lsl r0, r0, #8
	orr r4, r0
	mov r0, #1
	sub r0, r0, r5
	bl FindFreeMonsterZone
	mov r2, #1
	sub r2, r2, r5
	lsl r2, r2, #0x18
	lsl r0, r0, #0x18
	lsr r2, r2, #8
	orr r2, r0
	lsr r2, r2, #0x10
	add r0, r5, #0
	add r1, r4, #0
	bl MoveFieldCard
_0804E730:
	ldr r2, _0804E758 @ =0x020192E0
	ldr r0, _0804E754 @ =0x00001B23
	add r1, r2, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #4
	bhi _0804E760
	ldr r0, _0804E75C @ =0x00001B22
	add r1, r2, r0
	mov r0, #0
	b _0804E768
_0804E74C: .4byte 0x0201AE60
_0804E750: .4byte 0x00008043
_0804E754: .4byte 0x00001B23
_0804E758: .4byte 0x020192E0
_0804E75C: .4byte 0x00001B22
_0804E760:
	ldr r0, _0804E76C @ =0x00001B22
	add r1, r2, r0
	ldrb r0, [r1]
	add r0, #1
_0804E768:
	strb r0, [r1]
	b _0804E6CE
_0804E76C: .4byte 0x00001B22
_0804E770:
	mov r0, #1
_0804E772:
	add sp, #0x80
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EndPhase_TransferMushroomMan2
	.align 2, 0

