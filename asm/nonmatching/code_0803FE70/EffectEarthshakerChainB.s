	thumb_func_start EffectEarthshakerChainB
EffectEarthshakerChainB: @ 0x0804067C
	push {r4, lr}
	add r2, r0, #0
	ldr r0, _08040698 @ =0x02017A40
	ldr r1, _0804069C @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	add r1, r0, #0
	cmp r1, #4
	bgt _080406A0
	cmp r1, #1
	bge _080406DE
	cmp r1, #0
	beq _080406A6
	b _080406F0
_08040698: .4byte 0x02017A40
_0804069C: .4byte 0x000003E5
_080406A0:
	cmp r1, #5
	beq _080406BE
	b _080406F0
_080406A6:
	mov r0, #8
	neg r0, r0
	ldrb r3, [r2, #0xA]
	and r0, r3
	strb r0, [r2, #0xA]
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xA
	mov r2, #0
	mov r3, #0
	b _080406D8
_080406BE:
	ldrb r2, [r2, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	ldr r1, _080406E8 @ =0x020192E0
	ldr r3, _080406EC @ =0x00001B64
	add r2, r1, r3
	ldrh r2, [r2]
	add r3, #2
	add r1, r1, r3
	ldrh r3, [r1]
	mov r1, #0xB
_080406D8:
	bl DuelPrompt_Post
	ldrb r0, [r4]
_080406DE:
	add r0, #1
	strb r0, [r4]
	mov r0, #0
	b _08040706
	.align 2, 0
_080406E8: .4byte 0x020192E0
_080406EC: .4byte 0x00001B64
_080406F0:
	ldr r0, _0804070C @ =0x020192E0
	ldr r1, _08040710 @ =0x00001B64
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	bl AddEffectTarget
	mov r0, #1
_08040706:
	pop {r4}
	pop {r1}
	bx r1
_0804070C: .4byte 0x020192E0
_08040710: .4byte 0x00001B64
	thumb_func_end EffectEarthshakerChainB

