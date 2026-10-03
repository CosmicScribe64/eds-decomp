	thumb_func_start EffectDustTornadoChainB
EffectDustTornadoChainB: @ 0x080405E4
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _08040614 @ =0x02017A40
	ldr r1, _08040618 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _08040628
	ldr r0, _0804061C @ =0x00000206
	ldr r1, _08040620 @ =0x00000712
	ldr r3, _08040624 @ =0x08084740
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _0804066E
_08040614: .4byte 0x02017A40
_08040618: .4byte 0x000003E5
_0804061C: .4byte 0x00000206
_08040620: .4byte 0x00000712
_08040624: .4byte gStrDesignateOpponentSpellTrapToDestroy
_08040628:
	ldr r1, _0804063C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040640
	mov r0, #0
	strb r0, [r5]
	b _0804066E
	.align 2, 0
_0804063C: .4byte 0x03000040
_08040640:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08040650
	mov r0, #0
	b _0804066E
_08040650:
	ldr r0, _08040674 @ =0x0201CFB0
	ldr r3, _08040678 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl TryAddEffectTarget
	mov r0, #1
_0804066E:
	pop {r4, r5}
	pop {r1}
	bx r1
_08040674: .4byte 0x0201CFB0
_08040678: .4byte 0x00000824
	thumb_func_end EffectDustTornadoChainB

