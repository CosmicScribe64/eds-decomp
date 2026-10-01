	thumb_func_start sub_08065F34
sub_08065F34: @ 0x08065F34
	push {r4, r5, r6, r7, lr}
	add r6, r2, #0
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	ldr r1, _08065F64 @ =0xFFFF0000
	add r0, r0, r1
	lsr r4, r0, #0x10
	cmp r4, #0
	beq _08065F68
	mov r0, #0xC0
	add r1, r4, #0
	bl sub_0807B504
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r0, #0xB0
	lsl r0, r0, #7
	sub r0, r0, r5
	asr r0, r0, #8
	add r1, r4, #0
	bl sub_0807B504
	b _08065F6C
_08065F64: .4byte 0xFFFF0000
_08065F68:
	mov r5, #0xC0
	mov r0, #0x57
_08065F6C:
	mul r0, r7
	strh r0, [r6, #2]
	strh r5, [r6]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08065F34

