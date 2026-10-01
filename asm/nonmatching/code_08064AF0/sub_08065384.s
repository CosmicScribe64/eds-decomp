	thumb_func_start sub_08065384
sub_08065384: @ 0x08065384
	push {r4, r5, r6, r7, lr}
	sub sp, #0x50
	add r7, r1, #0
	lsl r2, r2, #0x10
	lsr r5, r2, #0x10
	lsl r3, r3, #0x10
	lsr r6, r3, #0x10
	ldr r1, _080653EC @ =0x08087554
	add r0, sp, #0x10
	bl sub_080752D0
	add r0, sp, #0x10
	bl sub_080753CC
	cmp r0, #0x64
	ble _080653AA
	add r1, sp, #0x74
	mov r0, #0
	strb r0, [r1]
_080653AA:
	bl sub_08064FCC
	add r4, r0, #0
	bl sub_08065034
	add r3, r0, #0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r1, r5, #0
	add r1, #8
	mov r2, #0x1F
	and r1, r2
	add r0, r6, #3
	and r0, r2
	lsl r0, r0, #5
	add r1, r1, r0
	lsl r1, r1, #1
	add r1, r7, r1
	mov r0, #2
	str r0, [sp, #0]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	str r0, [sp, #0xC]
	add r0, sp, #0x10
	add r2, r4, #0
	bl sub_08079340
	add sp, #0x50
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080653EC: .4byte gUnk_08087554
	thumb_func_end sub_08065384

