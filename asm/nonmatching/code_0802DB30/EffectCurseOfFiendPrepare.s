	thumb_func_start EffectCurseOfFiendPrepare
EffectCurseOfFiendPrepare: @ 0x0802E938
	push {r4, lr}
	add r4, r0, #0
	ldr r1, _0802E970 @ =0x020192E0
	ldr r0, _0802E974 @ =0x00001B12
	add r1, r1, r0
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #4
	bne _0802E96C
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountMonsters
	cmp r0, #0
	bgt _0802E978
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	bl CountMonsters
	cmp r0, #0
	bgt _0802E978
_0802E96C:
	mov r0, #0
	b _0802E97A
_0802E970: .4byte 0x020192E0
_0802E974: .4byte 0x00001B12
_0802E978:
	mov r0, #1
_0802E97A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectCurseOfFiendPrepare

