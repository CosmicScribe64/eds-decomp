	thumb_func_start EffectParasiteParacideResolve
EffectParasiteParacideResolve: @ 0x08033218
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803326A
	ldrb r2, [r4, #2]
	mov r5, #1
	add r0, r5, #0
	and r0, r2
	mov r3, #0x94
	cmp r0, #0
	beq _08033236
	ldr r3, _08033274 @ =0x00008094
_08033236:
	ldrh r0, [r4, #2]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x1A
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r2, #1
	sub r2, r2, r0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	bne _0803325E
	ldr r1, _08033278 @ =0x00008060
_0803325E:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0803326A:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08033274: .4byte 0x00008094
_08033278: .4byte 0x00008060
	thumb_func_end EffectParasiteParacideResolve

