	thumb_func_start AiPickStrongestHandMonster
AiPickStrongestHandMonster: @ 0x08056A94
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	str r0, [sp, #0]
	str r1, [sp, #4]
	mov r0, #1
	neg r0, r0
	str r0, [sp, #8]
	str r0, [sp, #0xC]
	mov r1, #0
	mov r8, r1
	ldr r1, _08056B38 @ =0x020192E4
	mov r2, #1
	ldr r3, [sp, #4]
	and r2, r3
	ldr r3, _08056B3C @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r8, r0
	blt _08056AC8
	b _08056BE0
_08056AC8:
	mov sl, r2
	ldr r0, _08056B40 @ =0x000007FF
	mov r9, r0
_08056ACE:
	mov r0, sl
	mul r0, r3
	ldr r1, [sp, #0]
	add r0, r1, r0
	mov r2, r8
	lsl r1, r2, #2
	ldr r3, _08056B44 @ =0x00000684
	add r1, r1, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r5, #0
	beq _08056BCA
	add r4, r5, #0
	mov r0, r9
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _08056B48 @ =0x08621DE0
	add r6, r0, r1
	ldr r0, [r6]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08056BCA
	add r0, r5, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08056BCA
	lsl r0, r4, #1
	ldr r2, _08056B4C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #1
	bl AiIsKeyCard
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08056BCA
	ldr r0, [r6]
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08056B5A
	cmp r0, #0x17
	ble _08056B50
	cmp r0, #0x18
	beq _08056B54
	b _08056B5A
	.align 2, 0
_08056B38: .4byte 0x020192E4
_08056B3C: .4byte 0x00000D64
_08056B40: .4byte 0x000007FF
_08056B44: .4byte 0x00000684
_08056B48: .4byte gCardStats
_08056B4C: .4byte gCardIdToNumber
_08056B50:
	mov r0, #0
	b _08056B72
_08056B54:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08056B72
_08056B5A:
	add r0, r5, #0
	mov r3, r9
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08056BA0 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08056B72:
	add r2, r0, #0
	ldr r3, [sp, #0xC]
	cmp r3, r2
	bge _08056BCA
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _08056BA0 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08056BAC
	cmp r0, #0x17
	ble _08056BA4
	cmp r0, #0x18
	beq _08056BA8
	b _08056BAC
	.align 2, 0
_08056BA0: .4byte gCardStats
_08056BA4:
	mov r0, #0
	b _08056BC0
_08056BA8:
	mov r0, #0xA
	b _08056BC0
_08056BAC:
	mov r0, r9
	and r5, r0
	lsl r0, r5, #2
	ldr r1, _08056C70 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08056BC0:
	cmp r0, #4
	bls _08056BCA
	str r2, [sp, #0xC]
	mov r2, r8
	str r2, [sp, #8]
_08056BCA:
	mov r3, #1
	add r8, r3
	ldr r1, _08056C74 @ =0x020192E4
	ldr r3, _08056C78 @ =0x00000D64
	mov r0, sl
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r8, r0
	bge _08056BE0
	b _08056ACE
_08056BE0:
	ldr r0, [sp, #8]
	cmp r0, #0
	bge _08056CCA
	mov r1, #0
	mov r8, r1
	mov r1, #1
	ldr r2, [sp, #4]
	and r1, r2
	ldr r2, _08056C78 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	ldr r3, _08056C74 @ =0x020192E4
	add r0, r0, r3
	ldrb r0, [r0, #2]
	cmp r8, r0
	bge _08056CC8
	mov r9, r1
	ldr r0, _08056C7C @ =0x000007FF
	mov sl, r0
_08056C06:
	mov r0, r9
	mul r0, r2
	ldr r1, [sp, #0]
	add r0, r1, r0
	mov r2, r8
	lsl r1, r2, #2
	ldr r3, _08056C80 @ =0x00000684
	add r1, r1, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r5, #0
	beq _08056CB4
	add r4, r5, #0
	mov r0, sl
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _08056C70 @ =0x08621DE0
	add r6, r0, r1
	ldr r0, [r6]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08056CB4
	add r0, r5, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08056CB4
	lsl r0, r4, #1
	ldr r2, _08056C84 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #1
	bl AiIsKeyCard
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08056CB4
	ldr r0, [r6]
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08056C92
	cmp r0, #0x17
	ble _08056C88
	cmp r0, #0x18
	beq _08056C8C
	b _08056C92
	.align 2, 0
_08056C70: .4byte gCardStats
_08056C74: .4byte 0x020192E4
_08056C78: .4byte 0x00000D64
_08056C7C: .4byte 0x000007FF
_08056C80: .4byte 0x00000684
_08056C84: .4byte gCardIdToNumber
_08056C88:
	mov r0, #0
	b _08056CA8
_08056C8C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08056CA8
_08056C92:
	mov r3, sl
	and r5, r3
	lsl r0, r5, #2
	ldr r1, _08056CDC @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08056CA8:
	ldr r2, [sp, #0xC]
	cmp r2, r0
	bge _08056CB4
	str r0, [sp, #0xC]
	mov r3, r8
	str r3, [sp, #8]
_08056CB4:
	mov r0, #1
	add r8, r0
	ldr r1, _08056CE0 @ =0x020192E4
	ldr r2, _08056CE4 @ =0x00000D64
	mov r0, r9
	mul r0, r2
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r8, r0
	blt _08056C06
_08056CC8:
	ldr r0, [sp, #8]
_08056CCA:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08056CDC: .4byte gCardStats
_08056CE0: .4byte 0x020192E4
_08056CE4: .4byte 0x00000D64
	thumb_func_end AiPickStrongestHandMonster

