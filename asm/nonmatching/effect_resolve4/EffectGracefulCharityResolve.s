	thumb_func_start EffectGracefulCharityResolve
EffectGracefulCharityResolve: @ 0x08033DF4
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08033E3C
	ldr r0, _08033E24 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08033E28
	cmp r0, #0x80
	bne _08033E3C
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #3
	bl DrawCards
	mov r0, #0x7F
	b _08033E3E
_08033E24: .4byte 0x02017A40
_08033E28:
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #2
	mov r2, #0
	mov r3, #0
	bl DuelPrompt_PostDiscard
	mov r0, #0x7E
	b _08033E3E
_08033E3C:
	mov r0, #0
_08033E3E:
	pop {r1}
	bx r1
	thumb_func_end EffectGracefulCharityResolve
	.align 2, 0

