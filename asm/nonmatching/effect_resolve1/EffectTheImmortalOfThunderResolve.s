	thumb_func_start EffectTheImmortalOfThunderResolve
EffectTheImmortalOfThunderResolve: @ 0x080318F8
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08031930
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08031938 @ =0x00000BB8
	bl GainLifePoints
	mov r0, #1
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0x92
	cmp r0, #0
	beq _08031920
	ldr r2, _0803193C @ =0x00008092
_08031920:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08031930:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08031938: .4byte 0x00000BB8
_0803193C: .4byte 0x00008092
	thumb_func_end EffectTheImmortalOfThunderResolve

