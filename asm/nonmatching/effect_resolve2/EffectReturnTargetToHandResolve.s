	thumb_func_start EffectReturnTargetToHandResolve
EffectReturnTargetToHandResolve: @ 0x08031D30
	push {r4, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08031D70
	mov r2, #7
	ldrb r0, [r1, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08031D70
	ldrb r4, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r3, r1, #8
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _08031D78 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08031D7C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08031D70
	add r0, r4, #0
	add r1, r3, #0
	mov r2, #0
	bl ReturnFieldCardToHand
_08031D70:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08031D78: .4byte 0x00000D64
_08031D7C: .4byte 0x0201930C
	thumb_func_end EffectReturnTargetToHandResolve

