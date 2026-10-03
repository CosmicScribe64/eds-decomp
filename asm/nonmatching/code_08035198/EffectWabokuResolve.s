	thumb_func_start EffectWabokuResolve
EffectWabokuResolve: @ 0x08035868
	push {lr}
	ldr r1, _080358A0 @ =0x020192E0
	ldr r2, _080358A4 @ =0x00001B12
	add r1, r1, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1E
	ldrb r2, [r0, #2]
	lsl r0, r2, #0x1F
	lsr r1, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r1, r0
	beq _08035898
	mov r0, #1
	and r0, r2
	mov r1, #0x36
	cmp r0, #0
	beq _0803588C
	ldr r1, _080358A8 @ =0x00008036
_0803588C:
	add r0, r1, #0
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08035898:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_080358A0: .4byte 0x020192E0
_080358A4: .4byte 0x00001B12
_080358A8: .4byte 0x00008036
	thumb_func_end EffectWabokuResolve

