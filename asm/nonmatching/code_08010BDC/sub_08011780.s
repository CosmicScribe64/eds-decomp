	thumb_func_start sub_08011780
sub_08011780: @ 0x08011780
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	ldr r1, _080117A8 @ =0x020185C0
	ldrh r0, [r1]
	lsr r3, r0, #0xF
	ldrh r7, [r1, #2]
	ldr r2, _080117AC @ =0x0000080A
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r6, r0, #0x19
	cmp r6, #1
	beq _080117E0
	cmp r6, #1
	bgt _080117B0
	cmp r6, #0
	beq _080117B8
	b _08011988
_080117A8: .4byte 0x020185C0
_080117AC: .4byte 0x0000080A
_080117B0:
	cmp r6, #2
	bne _080117B6
	b _0801194C
_080117B6:
	b _08011988
_080117B8:
	cmp r7, #9
	bgt _080117C6
	add r1, r7, #5
	add r0, r3, #0
	bl sub_08060FD0
	b _080117CE
_080117C6:
	add r0, r3, #0
	mov r1, #0xA
	bl sub_08060FD0
_080117CE:
	ldr r2, _080117D8 @ =0x020185C0
	ldr r3, _080117DC @ =0x0000080A
	add r2, r2, r3
	b _08011968
	.align 2, 0
_080117D8: .4byte 0x020185C0
_080117DC: .4byte 0x0000080A
_080117E0:
	cmp r7, #9
	bgt _08011858
	ldr r4, _08011840 @ =0x00000814
	add r0, r1, r4
	add r1, r3, #0
	and r1, r6
	ldr r2, _08011844 @ =0x00000D64
	add r4, r1, #0
	mul r4, r2
	ldr r1, _08011848 @ =0x0201930C
	add r1, r4, r1
	mov r2, #0x94
	add r5, r7, #0
	mul r5, r2
	mov r2, #0xB9
	lsl r2, r2, #2
	add r2, r2, r5
	mov ip, r2
	add r1, ip
	str r3, [sp, #0xC]
	bl sub_08007558
	ldr r3, [sp, #0xC]
	lsl r3, r3, #0x10
	lsr r0, r3, #0x10
	and r0, r6
	mov r1, #2
	neg r1, r1
	ldr r2, [sp, #0]
	and r2, r1
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	mov r0, #0xA
	orr r2, r0
	ldr r0, _0801184C @ =0x000001FF
	and r7, r0
	lsl r1, r7, #5
	ldr r0, _08011850 @ =0xFFFFC01F
	and r2, r0
	orr r2, r1
	str r2, [sp, #0]
	add r4, r4, r5
	ldr r0, _08011854 @ =0x020195F0
	add r4, r4, r0
	b _080118A6
	.align 2, 0
_08011840: .4byte 0x00000814
_08011844: .4byte 0x00000D64
_08011848: .4byte 0x0201930C
_0801184C: .4byte 0x000001FF
_08011850: .4byte 0xFFFFC01F
_08011854: .4byte 0x020195F0
_08011858:
	ldr r2, _0801192C @ =0x00000814
	add r0, r1, r2
	add r1, r3, #0
	and r1, r6
	ldr r2, _08011930 @ =0x00000D64
	add r4, r1, #0
	mul r4, r2
	str r4, [sp, #8]
	ldr r1, _08011934 @ =0x0201930C
	mov r8, r1
	add r1, r4, #0
	add r1, r8
	mov r2, #0x94
	add r4, r7, #0
	mul r4, r2
	add r1, r1, r4
	str r3, [sp, #0xC]
	bl sub_08007558
	ldr r3, [sp, #0xC]
	lsl r3, r3, #0x10
	lsr r0, r3, #0x10
	and r0, r6
	mov r1, #2
	neg r1, r1
	ldr r2, [sp, #0]
	and r2, r1
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	mov r0, #0x14
	orr r2, r0
	ldr r0, _08011938 @ =0xFFFFC01F
	and r2, r0
	str r2, [sp, #0]
	ldr r0, [sp, #8]
	add r4, r4, r0
	add r4, r8
_080118A6:
	ldrb r1, [r4, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xE
	ldr r1, _0801193C @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r4, [r4, #6]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xF
	ldr r2, _08011940 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	lsr r1, r3, #0x10
	mov r0, #2
	neg r0, r0
	ldr r2, [sp, #4]
	and r2, r0
	orr r2, r1
	sub r0, #0x1D
	and r2, r0
	mov r0, #0x16
	orr r2, r0
	str r2, [sp, #4]
	ldr r4, _08011944 @ =0x02018DD4
	ldr r0, [r4]
	lsl r1, r0, #0x13
	lsr r1, r1, #0x1F
	ldr r3, _08011930 @ =0x00000D64
	mul r1, r3
	ldr r3, _08011948 @ =0x020192E4
	add r1, r1, r3
	ldrb r1, [r1, #2]
	lsl r3, r1, #5
	ldr r1, _08011938 @ =0xFFFFC01F
	and r2, r1
	orr r2, r3
	sub r1, #0x20
	and r2, r1
	mov r1, #0x80
	lsl r1, r1, #8
	orr r2, r1
	str r2, [sp, #4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	sub r4, #0xA
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _0801199C
_0801192C: .4byte 0x00000814
_08011930: .4byte 0x00000D64
_08011934: .4byte 0x0201930C
_08011938: .4byte 0xFFFFC01F
_0801193C: .4byte 0xFFFFBFFF
_08011940: .4byte 0xFFFF7FFF
_08011944: .4byte 0x02018DD4
_08011948: .4byte 0x020192E4
_0801194C:
	cmp r7, #9
	bgt _0801195A
	add r1, r7, #5
	add r0, r3, #0
	bl sub_08008EB4
	b _08011962
_0801195A:
	add r0, r3, #0
	add r1, r7, #0
	bl sub_08008EB4
_08011962:
	ldr r2, _08011980 @ =0x020185C0
	ldr r4, _08011984 @ =0x0000080A
	add r2, r2, r4
_08011968:
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _0801199C
_08011980: .4byte 0x020185C0
_08011984: .4byte 0x0000080A
_08011988:
	bl sub_080611AC
	ldr r1, _080119A8 @ =0x020185C0
	ldr r0, _080119AC @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801199C:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080119A8: .4byte 0x020185C0
_080119AC: .4byte 0x0000080D
	thumb_func_end sub_08011780

