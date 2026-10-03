	thumb_func_start EffectShareThePainResolve
EffectShareThePainResolve: @ 0x08035BBC
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08035C04
	ldr r0, _08035C00 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _08035C04
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r5, #1
	sub r0, r5, r0
	mov r1, #1
	neg r1, r1
	bl CountTributableMonsters
	cmp r0, #0
	beq _08035C04
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	bl DuelPrompt_PostTribute
	mov r0, #0x7F
	b _08035C06
	.align 2, 0
_08035C00: .4byte 0x02017A40
_08035C04:
	mov r0, #0
_08035C06:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectShareThePainResolve

