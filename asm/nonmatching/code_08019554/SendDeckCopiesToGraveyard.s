	thumb_func_start SendDeckCopiesToGraveyard
SendDeckCopiesToGraveyard: @ 0x08019E0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0]
	mov r6, #0
	ldr r1, _08019E60 @ =0x020192E4
	mov r2, #1
	and r2, r0
	ldr r3, _08019E64 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	mov sl, r1
	ldrb r0, [r0, #3]
	cmp r6, r0
	bge _08019EE2
	add r5, r2, #0
	mov r7, #0
	ldr r0, _08019E68 @ =0x000007C4
	add r0, sl
	mov r9, r0
_08019E46:
	add r0, r5, #0
	mul r0, r3
	add r0, r7, r0
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r0, _08019E6C @ =0x0000FFFF
	cmp r4, r0
	bne _08019E70
	mov r0, #0
	b _08019EA2
	.align 2, 0
_08019E60: .4byte 0x020192E4
_08019E64: .4byte 0x00000D64
_08019E68: .4byte 0x000007C4
_08019E6C: .4byte 0x0000FFFF
_08019E70:
	ldr r0, _08019E88 @ =0x000007CF
	cmp r4, r0
	bhi _08019E90
	add r0, #0x30
	add r1, r0, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08019E8C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08019EA2
_08019E88: .4byte 0x000007CF
_08019E8C: .4byte gCardNumberToId
_08019E90:
	ldr r1, _08019F1C @ =0xFFFFF830
	add r0, r4, r1
	ldr r1, _08019F20 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08019F24 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_08019EA2:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	bl IsSameCardName
	cmp r0, #0
	beq _08019ECE
	ldr r0, _08019F28 @ =0x00000D64
	mul r0, r5
	add r0, r9
	add r0, r0, r7
	mov r3, #0x67
	mov r1, r8
	cmp r1, #0
	beq _08019EC2
	ldr r3, _08019F2C @ =0x00008067
_08019EC2:
	ldrh r1, [r0]
	ldrh r2, [r0, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
_08019ECE:
	add r7, #4
	add r6, #1
	ldr r1, _08019F30 @ =0x020192E4
	ldr r3, _08019F28 @ =0x00000D64
	add r0, r5, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r6, r0
	blt _08019E46
_08019EE2:
	mov r6, #0
	mov r1, #1
	mov r0, r8
	and r1, r0
	ldr r2, _08019F28 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	add r0, sl
	ldrb r0, [r0, #5]
	cmp r6, r0
	bge _08019FAE
	add r5, r1, #0
	mov r7, #0
	ldr r1, _08019F34 @ =0x00000A44
	add r1, sl
	mov r9, r1
_08019F02:
	add r0, r5, #0
	mul r0, r2
	add r0, r7, r0
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r0, _08019F38 @ =0x0000FFFF
	cmp r4, r0
	bne _08019F3C
	mov r0, #0
	b _08019F6E
	.align 2, 0
_08019F1C: .4byte 0xFFFFF830
_08019F20: .4byte 0x000007FF
_08019F24: .4byte gCardNumberToId
_08019F28: .4byte 0x00000D64
_08019F2C: .4byte 0x00008067
_08019F30: .4byte 0x020192E4
_08019F34: .4byte 0x00000A44
_08019F38: .4byte 0x0000FFFF
_08019F3C:
	ldr r0, _08019F54 @ =0x000007CF
	cmp r4, r0
	bhi _08019F5C
	add r0, #0x30
	add r1, r0, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08019F58 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08019F6E
_08019F54: .4byte 0x000007CF
_08019F58: .4byte gCardNumberToId
_08019F5C:
	ldr r1, _08019FF0 @ =0xFFFFF830
	add r0, r4, r1
	ldr r1, _08019FF4 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08019FF8 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_08019F6E:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	bl IsSameCardName
	cmp r0, #0
	beq _08019F9A
	ldr r0, _08019FFC @ =0x00000D64
	mul r0, r5
	add r0, r9
	add r0, r0, r7
	mov r3, #0xDD
	mov r1, r8
	cmp r1, #0
	beq _08019F8E
	ldr r3, _0801A000 @ =0x000080DD
_08019F8E:
	ldrh r1, [r0]
	ldrh r2, [r0, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
_08019F9A:
	add r7, #4
	add r6, #1
	ldr r1, _0801A004 @ =0x020192E4
	ldr r2, _08019FFC @ =0x00000D64
	add r0, r5, #0
	mul r0, r2
	add r0, r0, r1
	ldrb r0, [r0, #5]
	cmp r6, r0
	blt _08019F02
_08019FAE:
	ldr r0, [sp, #0]
	cmp r0, #0
	beq _08019FE0
	cmp r4, #0xC6
	bne _08019FE0
	mov r0, #0xD6
	mov r1, r8
	cmp r1, #0
	beq _08019FC2
	ldr r0, _0801A008 @ =0x000080D6
_08019FC2:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x60
	mov r1, r8
	cmp r1, #0
	beq _08019FD6
	ldr r0, _0801A00C @ =0x00008060
_08019FD6:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08019FE0:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08019FF0: .4byte 0xFFFFF830
_08019FF4: .4byte 0x000007FF
_08019FF8: .4byte gCardNumberToId
_08019FFC: .4byte 0x00000D64
_0801A000: .4byte 0x000080DD
_0801A004: .4byte 0x020192E4
_0801A008: .4byte 0x000080D6
_0801A00C: .4byte 0x00008060
	thumb_func_end SendDeckCopiesToGraveyard

