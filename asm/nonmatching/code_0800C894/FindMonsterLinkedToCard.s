	thumb_func_start FindMonsterLinkedToCard
FindMonsterLinkedToCard: @ 0x0800CD68
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r0, r1
	ldr r3, _0800CD94 @ =0x00000D64
	add r1, r2, #0
	mul r1, r3
	add r0, r0, r1
	ldr r1, _0800CD98 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	bne _0800CDA8
	mov r0, #0
	b _0800CE0A
_0800CD94: .4byte 0x00000D64
_0800CD98: .4byte 0x0201930C
_0800CD9C:
	lsl r0, r5, #0x18
	lsl r1, r4, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	b _0800CE0A
_0800CDA8:
	mov r5, #0
	mov r0, #1
	mov sl, r0
	mov r9, r3
	ldr r2, _0800CE18 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r1, r1, #0x10
	mov r8, r1
_0800CDBA:
	mov r4, #0
	add r0, r5, #0
	mov r1, sl
	and r0, r1
	mov r7, r9
	mul r7, r0
	mov r2, r8
	lsr r0, r2, #0xF
	ldr r1, _0800CE1C @ =0x08622AB4
	add r6, r0, r1
_0800CDCE:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _0800CE20 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800CDFC
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0800CDFC
	ldrh r2, [r6]
	add r0, r5, #0
	add r1, r4, #0
	bl FindZoneLinkFromCard
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0800CD9C
_0800CDFC:
	add r4, #1
	cmp r4, #4
	ble _0800CDCE
	add r5, #1
	cmp r5, #1
	ble _0800CDBA
	ldr r0, _0800CE24 @ =0x0000FFFF
_0800CE0A:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0800CE18: .4byte 0x000007FF
_0800CE1C: .4byte gCardIdToNumber
_0800CE20: .4byte 0x0201930C
_0800CE24: .4byte 0x0000FFFF
	thumb_func_end FindMonsterLinkedToCard

