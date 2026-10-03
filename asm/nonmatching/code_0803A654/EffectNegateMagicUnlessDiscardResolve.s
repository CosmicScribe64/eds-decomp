	thumb_func_start EffectNegateMagicUnlessDiscardResolve
EffectNegateMagicUnlessDiscardResolve: @ 0x0803A720
	push {r4, r5, lr}
	add r3, r0, #0
	mov r0, #4
	ldrb r2, [r3, #4]
	and r0, r2
	cmp r0, #0
	bne _0803A7EC
	ldr r0, _0803A748 @ =0x02017A40
	mov r5, #0xF8
	lsl r5, r5, #2
	add r4, r0, r5
	ldrb r2, [r4]
	add r0, r2, #0
	cmp r0, #0x7F
	beq _0803A78A
	cmp r0, #0x7F
	bgt _0803A74C
	cmp r0, #0x7E
	beq _0803A79A
	b _0803A7EC
_0803A748: .4byte 0x02017A40
_0803A74C:
	cmp r0, #0x80
	bne _0803A7EC
	ldr r2, _0803A770 @ =0x020192E4
	ldrb r3, [r3, #2]
	lsl r4, r3, #0x1F
	lsr r0, r4, #0x1F
	mov r3, #1
	sub r0, r3, r0
	and r0, r3
	ldr r1, _0803A774 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	bne _0803A778
	mov r0, #0x7E
	b _0803A7EE
	.align 2, 0
_0803A770: .4byte 0x020192E4
_0803A774: .4byte 0x00000D64
_0803A778:
	lsr r0, r4, #0x1F
	sub r0, r3, r0
	mov r1, #0x11
	mov r2, #0
	mov r3, #0
	bl DuelPrompt_Post
	mov r0, #0x7F
	b _0803A7EE
_0803A78A:
	ldr r0, _0803A7D8 @ =0x020192E0
	ldr r5, _0803A7DC @ =0x00001B64
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, #0
	bne _0803A7EC
	sub r0, r2, #1
	strb r0, [r4]
_0803A79A:
	cmp r1, #0
	beq _0803A7EC
	ldr r0, _0803A7E0 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0803A7E4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0803A7D2
	mov r0, #1
	ldrb r3, [r3, #2]
	and r0, r3
	mov r1, #0xB0
	cmp r0, #0
	beq _0803A7C6
	ldr r1, _0803A7E8 @ =0x000080B0
_0803A7C6:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0803A7D2:
	mov r0, #0x64
	b _0803A7EE
	.align 2, 0
_0803A7D8: .4byte 0x020192E0
_0803A7DC: .4byte 0x00001B64
_0803A7E0: .4byte 0x000007FF
_0803A7E4: .4byte gCardStats
_0803A7E8: .4byte 0x000080B0
_0803A7EC:
	mov r0, #0
_0803A7EE:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectNegateMagicUnlessDiscardResolve

