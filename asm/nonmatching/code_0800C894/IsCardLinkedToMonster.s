	thumb_func_start IsCardLinkedToMonster
IsCardLinkedToMonster: @ 0x0800CC18
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r0, r1
	ldr r3, _0800CC44 @ =0x00000D64
	add r1, r2, #0
	mul r1, r3
	add r0, r0, r1
	ldr r1, _0800CC48 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	bne _0800CC50
	b _0800CCB0
	.align 2, 0
_0800CC44: .4byte 0x00000D64
_0800CC48: .4byte 0x0201930C
_0800CC4C:
	mov r0, #1
	b _0800CCB2
_0800CC50:
	mov r5, #0
	mov r0, #1
	mov sl, r0
	mov r9, r3
	ldr r2, _0800CCC0 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r1, r1, #0x10
	mov r8, r1
_0800CC62:
	mov r4, #0
	add r0, r5, #0
	mov r1, sl
	and r0, r1
	mov r7, r9
	mul r7, r0
	mov r2, r8
	lsr r0, r2, #0xF
	ldr r1, _0800CCC4 @ =0x08622AB4
	add r6, r0, r1
_0800CC76:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _0800CCC8 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800CCA4
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0800CCA4
	ldrh r2, [r6]
	add r0, r5, #0
	add r1, r4, #0
	bl FindZoneLinkFromCard
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0800CC4C
_0800CCA4:
	add r4, #1
	cmp r4, #4
	ble _0800CC76
	add r5, #1
	cmp r5, #1
	ble _0800CC62
_0800CCB0:
	mov r0, #0
_0800CCB2:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0800CCC0: .4byte 0x000007FF
_0800CCC4: .4byte gCardIdToNumber
_0800CCC8: .4byte 0x0201930C
	thumb_func_end IsCardLinkedToMonster

