	thumb_func_start EffectGraverobberResolve
EffectGraverobberResolve: @ 0x08036510
	push {r4, r5, r6, lr}
	sub sp, #4
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08036560
	mov r0, #7
	ldrb r2, [r4, #0xA]
	and r0, r2
	cmp r0, #2
	bne _08036560
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r6, #1
	sub r0, r6, r0
	ldrh r2, [r4, #0xC]
	lsl r1, r2, #0x14
	lsr r5, r1, #0x14
	add r1, r5, #0
	mov r2, sp
	bl GetGraveyardCardById
	cmp r0, #0
	beq _08036560
	add r0, r6, #0
	ldrb r4, [r4, #2]
	and r0, r4
	mov r2, #0xD5
	cmp r0, #0
	beq _08036554
	ldr r2, _0803656C @ =0x000080D5
_08036554:
	add r1, r5, #0
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08036560:
	mov r0, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803656C: .4byte 0x000080D5
	thumb_func_end EffectGraverobberResolve

