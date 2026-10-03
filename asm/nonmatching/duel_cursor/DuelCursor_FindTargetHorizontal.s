	thumb_func_start DuelCursor_FindTargetHorizontal
DuelCursor_FindTargetHorizontal: @ 0x08052B78
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	mov r9, r1
	mov sl, r2
	str r3, [sp, #0]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldr r6, [r1]
	ldr r5, [r2]
	ldr r4, [r3]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r4, [sp, #0xC]
	ldr r7, _08052BBC @ =0x020192E4
	mov r0, #1
	and r0, r6
	str r0, [sp, #0x10]
_08052BA4:
	mov r0, r8
	mov r1, #4
	and r0, r1
	cmp r0, #0
	beq _08052C26
	cmp r5, #5
	beq _08052C06
	cmp r5, #5
	bgt _08052BC0
	cmp r5, #0
	beq _08052BEE
	b _08052C26
_08052BBC: .4byte 0x020192E4
_08052BC0:
	cmp r5, #0xA
	beq _08052C1A
	cmp r5, #0xB
	bne _08052C26
	cmp r6, #0
	beq _08052BE4
	cmp r4, #0
	bgt _08052BDA
	ldr r0, _08052BE0 @ =0x00000D64
	ldr r1, [sp, #0x10]
	mul r0, r1
	add r0, r0, r7
	ldrb r4, [r0, #2]
_08052BDA:
	sub r4, #1
	b _08052C26
	.align 2, 0
_08052BE0: .4byte 0x00000D64
_08052BE4:
	ldrb r0, [r7, #2]
	sub r0, #1
	cmp r4, r0
	blt _08052BFC
	b _08052C02
_08052BEE:
	cmp r6, #0
	beq _08052BF8
	cmp r4, #0
	bgt _08052BDA
	b _08052C00
_08052BF8:
	cmp r4, #3
	bgt _08052C00
_08052BFC:
	add r4, #1
	b _08052C26
_08052C00:
	mov r5, #0xA
_08052C02:
	mov r4, #0
	b _08052C26
_08052C06:
	cmp r6, #0
	beq _08052C0E
	add r0, r4, #4
	b _08052C10
_08052C0E:
	add r0, r4, #1
_08052C10:
	mov r1, #5
	bl __modsi3
	add r4, r0, #0
	b _08052C26
_08052C1A:
	mov r5, #0
	neg r0, r6
	orr r0, r6
	asr r4, r0, #0x1F
	mov r0, #4
	and r4, r0
_08052C26:
	mov r0, #8
	mov r1, r8
	and r0, r1
	cmp r0, #0
	beq _08052CA0
	cmp r5, #5
	beq _08052C82
	cmp r5, #5
	bgt _08052C3E
	cmp r5, #0
	beq _08052C6A
	b _08052CA0
_08052C3E:
	cmp r5, #0xA
	beq _08052C96
	cmp r5, #0xB
	bne _08052CA0
	cmp r6, #0
	beq _08052C60
	ldr r0, _08052C5C @ =0x00000D64
	ldr r1, [sp, #0x10]
	mul r0, r1
	add r0, r0, r7
	ldrb r0, [r0, #2]
	sub r0, #1
	cmp r4, r0
	blt _08052C72
	b _08052C9E
_08052C5C: .4byte 0x00000D64
_08052C60:
	cmp r4, #0
	bgt _08052C66
	ldrb r4, [r7, #2]
_08052C66:
	sub r4, #1
	b _08052CA0
_08052C6A:
	cmp r6, #0
	beq _08052C7A
	cmp r4, #3
	bgt _08052C76
_08052C72:
	add r4, #1
	b _08052CA0
_08052C76:
	mov r5, #0xA
	b _08052C9E
_08052C7A:
	cmp r4, #0
	bgt _08052C66
	mov r5, #0xA
	b _08052C9E
_08052C82:
	cmp r6, #0
	beq _08052C8A
	add r0, r4, #1
	b _08052C8C
_08052C8A:
	add r0, r4, #4
_08052C8C:
	mov r1, #5
	bl __modsi3
	add r4, r0, #0
	b _08052CA0
_08052C96:
	mov r5, #0
	mov r4, #4
	cmp r6, #0
	beq _08052CA0
_08052C9E:
	mov r4, #0
_08052CA0:
	ldr r0, [sp, #4]
	cmp r0, r6
	bne _08052CB6
	ldr r1, [sp, #8]
	cmp r1, r5
	bne _08052CB6
	ldr r0, [sp, #0xC]
	cmp r0, r4
	bne _08052CB6
	mov r0, #0
	b _08052CD8
_08052CB6:
	add r0, r6, #0
	add r1, r5, #0
	add r2, r4, #0
	ldr r3, [sp, #0x34]
	bl DuelCursor_IsValidTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08052CCA
	b _08052BA4
_08052CCA:
	mov r1, r9
	str r6, [r1]
	mov r0, sl
	str r5, [r0]
	ldr r1, [sp, #0]
	str r4, [r1]
	mov r0, #1
_08052CD8:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelCursor_FindTargetHorizontal

