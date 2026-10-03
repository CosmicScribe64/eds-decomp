	thumb_func_start IsSpellTrapZoneFree
IsSpellTrapZoneFree: @ 0x08008C24
	push {r4, r5, lr}
	add r4, r1, #0
	mov r5, #1
	and r0, r5
	mov r1, #0x94
	mul r1, r4
	ldr r2, _08008C5C @ =0x00000D64
	mul r2, r0
	add r1, r1, r2
	ldr r3, _08008C60 @ =0x0201930C
	add r1, r1, r3
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08008C64
	add r0, r3, #0
	sub r0, #0x28
	add r0, r2, r0
	ldr r0, [r0, #8]
	lsl r0, r0, #8
	lsr r0, r0, #0x16
	asr r0, r4
	and r0, r5
	cmp r0, #0
	bne _08008C64
	mov r0, #1
	b _08008C66
	.align 2, 0
_08008C5C: .4byte 0x00000D64
_08008C60: .4byte 0x0201930C
_08008C64:
	mov r0, #0
_08008C66:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end IsSpellTrapZoneFree

