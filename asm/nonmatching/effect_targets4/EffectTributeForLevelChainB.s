	thumb_func_start EffectTributeForLevelChainB
EffectTributeForLevelChainB: @ 0x08040FB0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	ldr r0, _08040FD0 @ =0x02017A40
	ldr r1, _08040FD4 @ =0x000003E5
	add r4, r0, r1
	ldrb r6, [r4]
	cmp r6, #0
	beq _08040FD8
	cmp r6, #1
	beq _08041008
	b _0804111A
	.align 2, 0
_08040FD0: .4byte 0x02017A40
_08040FD4: .4byte 0x000003E5
_08040FD8:
	mov r0, #8
	neg r0, r0
	mov r2, r8
	ldrb r2, [r2, #0xA]
	and r0, r2
	mov r7, r8
	strb r0, [r7, #0xA]
	ldr r0, _08040FFC @ =0x00000206
	ldr r1, _08041000 @ =0x00000712
	ldr r3, _08041004 @ =0x080849F4
	mov r2, #0xB
	bl TextBoxOpen
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0804111A
	.align 2, 0
_08040FFC: .4byte 0x00000206
_08041000: .4byte 0x00000712
_08041004: .4byte gStrDesignateMonsterYouWishToTribute
_08041008:
	ldr r1, _0804101C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041020
	mov r0, #0
	strb r0, [r4]
	b _0804111C
	.align 2, 0
_0804101C: .4byte 0x03000040
_08041020:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0804111A
	ldr r0, _080410C4 @ =0x0201CFB0
	ldr r1, _080410C8 @ =0x00000824
	add r7, r0, r1
	ldr r2, _080410CC @ =0x00000828
	add r2, r2, r0
	mov r9, r2
	add r1, #8
	add r1, r1, r0
	mov sl, r1
	ldr r0, [r2]
	ldr r1, [r1]
	add r0, r0, r1
	ldr r5, [r7]
	lsl r0, r0, #0x18
	lsr r4, r0, #0x10
	ldrb r2, [r7]
	orr r4, r2
	mov r0, r8
	add r1, r4, #0
	bl EffectTributeForInsectCheck
	cmp r0, #0
	beq _08041114
	mov r0, #1
	bl PlaySE
	add r0, r6, #0
	mov r1, r8
	ldrb r1, [r1, #2]
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0804106E
	ldr r3, _080410D0 @ =0x00008008
_0804106E:
	ldrh r1, [r7]
	mov r7, sl
	ldrb r7, [r7]
	lsl r2, r7, #8
	mov r0, r9
	ldrb r0, [r0]
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	add r1, r4, #0
	bl TributeMonster
	and r5, r6
	mov r0, #0x94
	mul r0, r4
	ldr r1, _080410D4 @ =0x00000D64
	mul r1, r5
	add r0, r0, r1
	ldr r1, _080410D8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r0, _080410DC @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _080410E0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080410EC
	cmp r0, #0x17
	ble _080410E4
	cmp r0, #0x18
	beq _080410E8
	b _080410EC
_080410C4: .4byte 0x0201CFB0
_080410C8: .4byte 0x00000824
_080410CC: .4byte 0x00000828
_080410D0: .4byte 0x00008008
_080410D4: .4byte 0x00000D64
_080410D8: .4byte 0x0201930C
_080410DC: .4byte 0x000007FF
_080410E0: .4byte gCardStats
_080410E4:
	mov r0, #0
	b _08041100
_080410E8:
	mov r0, #0xA
	b _08041100
_080410EC:
	ldr r0, _0804110C @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r2, _08041110 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08041100:
	add r1, r0, #1
	mov r0, r8
	bl AddEffectTarget
	mov r0, #1
	b _0804111C
_0804110C: .4byte 0x000007FF
_08041110: .4byte gCardStats
_08041114:
	mov r0, #3
	bl PlaySE
_0804111A:
	mov r0, #0
_0804111C:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTributeForLevelChainB
	.align 2, 0

