	thumb_func_start EffectTimeSealResolve
EffectTimeSealResolve: @ 0x080364DC
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08036504
	mov r0, #1
	ldrb r1, [r1, #2]
	and r0, r1
	mov r1, #0x44
	cmp r0, #0
	bne _080364F8
	ldr r1, _0803650C @ =0x00008044
_080364F8:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08036504:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_0803650C: .4byte 0x00008044
	thumb_func_end EffectTimeSealResolve

