	thumb_func_start EffectTributeTargetChainB
EffectTributeTargetChainB: @ 0x0803E160
	push {r4, r5, lr}
	add r5, r0, #0
	mov r2, #1
	ldrb r0, [r5, #2]
	and r2, r0
	cmp r2, #0
	beq _0803E194
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	bl AiPickEffectTribute
	add r2, r0, #0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r5, #0xA]
	and r0, r1
	strb r0, [r5, #0xA]
	cmp r2, r4
	ble _0803E214
	add r0, r5, #0
	mov r1, #1
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	b _0803E214
_0803E194:
	ldr r0, _0803E1C0 @ =0x02017A40
	ldr r3, _0803E1C4 @ =0x000003E5
	add r4, r0, r3
	ldrb r0, [r4]
	cmp r0, #0
	bne _0803E1D4
	ldr r0, _0803E1C8 @ =0x00000206
	ldr r1, _0803E1CC @ =0x00000712
	ldr r3, _0803E1D0 @ =0x08083CC8
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #8
	neg r0, r0
	ldrb r1, [r5, #0xA]
	and r0, r1
	strb r0, [r5, #0xA]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0803E224
	.align 2, 0
_0803E1C0: .4byte 0x02017A40
_0803E1C4: .4byte 0x000003E5
_0803E1C8: .4byte 0x00000206
_0803E1CC: .4byte 0x00000712
_0803E1D0: .4byte gStrDesignateOwnTribute
_0803E1D4:
	ldr r1, _0803E1E4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803E1E8
	strb r2, [r4]
	b _0803E224
_0803E1E4: .4byte 0x03000040
_0803E1E8:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803E224
	ldr r0, _0803E218 @ =0x0201CFB0
	ldr r2, _0803E21C @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803E220 @ =0x00000828
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r5, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803E224
_0803E214:
	mov r0, #1
	b _0803E226
_0803E218: .4byte 0x0201CFB0
_0803E21C: .4byte 0x00000824
_0803E220: .4byte 0x00000828
_0803E224:
	mov r0, #0
_0803E226:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectTributeTargetChainB

