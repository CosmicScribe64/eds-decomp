	thumb_func_start DeckEdit_DrawPortraitTilemap
DeckEdit_DrawPortraitTilemap: @ 0x08064E28
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r4, [sp, #0x1C]
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r0, #0x5A
	mul r0, r2
	add r6, r0, r4
	cmp r3, #0
	beq _08064E56
	cmp r3, #1
	beq _08064EA4
	b _08064EEE
_08064E56:
	mov r4, #0
	lsl r1, r1, #0x18
	mov r0, #0x1F
	mov r8, r0
	add r7, r5, #0
	and r7, r0
	asr r1, r1, #0x18
	mov ip, r1
	ldr r1, _08064EA0 @ =0x0600F000
	mov r9, r1
_08064E6A:
	mov r1, ip
	add r0, r1, r4
	mov r1, r8
	and r0, r1
	lsl r0, r0, #5
	add r0, r7, r0
	lsl r0, r0, #1
	mov r1, r9
	add r2, r0, r1
	mov r3, #0
	add r5, r4, #1
_08064E80:
	add r0, r6, #0
	add r1, r0, #1
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	strh r0, [r2]
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #8
	bls _08064E80
	lsl r0, r5, #0x18
	lsr r4, r0, #0x18
	cmp r4, #9
	bls _08064E6A
	b _08064EEE
_08064EA0: .4byte 0x0600F000
_08064EA4:
	mov r4, #0
	lsl r1, r1, #0x18
	lsl r0, r5, #0x18
	asr r7, r0, #0x18
	mov r0, #0x1F
	mov ip, r0
	asr r1, r1, #0x18
	mov r9, r1
	ldr r1, _08064EFC @ =0x0600F000
	mov r8, r1
_08064EB8:
	mov r3, #0
	add r5, r4, #1
	mov r1, r9
	add r0, r1, r4
	mov r1, ip
	and r0, r1
	lsl r4, r0, #5
_08064EC6:
	add r1, r7, r3
	mov r0, ip
	and r1, r0
	add r1, r1, r4
	lsl r1, r1, #1
	add r1, r8
	add r2, r6, #0
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	strh r2, [r1]
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #8
	bls _08064EC6
	lsl r0, r5, #0x18
	lsr r4, r0, #0x18
	cmp r4, #9
	bls _08064EB8
_08064EEE:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08064EFC: .4byte 0x0600F000
	thumb_func_end DeckEdit_DrawPortraitTilemap

