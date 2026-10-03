	thumb_func_start CountSpellTraps
CountSpellTraps: @ 0x080091B4
	push {r4, lr}
	mov r3, #0
	mov r1, #1
	and r1, r0
	ldr r0, _080091E8 @ =0x00000D64
	mul r0, r1
	mov r2, #0xB9
	lsl r2, r2, #2
	add r1, r0, r2
	ldr r4, _080091EC @ =0x0201930C
	mov r2, #4
_080091CA:
	add r0, r1, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080091D6
	add r3, #1
_080091D6:
	add r1, #0x94
	sub r2, #1
	cmp r2, #0
	bge _080091CA
	add r0, r3, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080091E8: .4byte 0x00000D64
_080091EC: .4byte 0x0201930C
	thumb_func_end CountSpellTraps

