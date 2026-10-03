	thumb_func_start EffectMoveAllEquipsResolve
EffectMoveAllEquipsResolve: @ 0x0803C254
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _0803C354
	mov r0, #7
	ldrb r2, [r6, #0xA]
	and r0, r2
	cmp r0, #1
	bne _0803C354
	mov r7, #0
_0803C278:
	mov r4, #5
	add r0, r7, #1
	str r0, [sp, #4]
	str r7, [sp, #0]
	lsl r0, r7, #0x18
	lsr r0, r0, #0x18
	mov sl, r0
_0803C286:
	add r5, r4, #0
	ldrb r1, [r6, #0xC]
	mov r9, r1
	ldrh r2, [r6, #0xC]
	lsr r3, r2, #8
	mov r0, #1
	mov r8, r0
	add r1, r7, #0
	and r1, r0
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r0, _0803C2E4 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803C2E8 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0803C348
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0803C348
	ldr r2, _0803C2EC @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #2
	ldr r1, _0803C2F0 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0803C2F4
	cmp r0, #0x15
	blt _0803C2F4
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0803C2F6
_0803C2E4: .4byte 0x00000D64
_0803C2E8: .4byte 0x0201930C
_0803C2EC: .4byte 0x000007FF
_0803C2F0: .4byte gCardStats
_0803C2F4:
	mov r0, #0
_0803C2F6:
	cmp r0, #3
	bne _0803C348
	lsl r1, r5, #0x18
	lsr r1, r1, #0x10
	mov r2, sl
	orr r1, r2
	add r0, r6, #0
	str r3, [sp, #8]
	bl EffectTailorOfTheFickleCheck
	ldr r3, [sp, #8]
	cmp r0, #0
	bne _0803C314
	mov r0, #0
	mov r8, r0
_0803C314:
	ldr r0, [sp, #0]
	add r1, r5, #0
	mov r2, r9
	bl IsValidEquipTarget
	cmp r0, #0
	bne _0803C326
	mov r1, #0
	mov r8, r1
_0803C326:
	mov r2, r8
	cmp r2, #0
	beq _0803C33E
	lsl r0, r7, #0x18
	lsl r1, r4, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	ldrh r1, [r6, #0xC]
	bl MoveEquipCard
	b _0803C348
_0803C33E:
	add r0, r7, #0
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_0803C348:
	add r4, #1
	cmp r4, #9
	ble _0803C286
	ldr r7, [sp, #4]
	cmp r7, #1
	ble _0803C278
_0803C354:
	mov r0, #0
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectMoveAllEquipsResolve
	.align 2, 0

