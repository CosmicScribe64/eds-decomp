	thumb_func_start IsHandRevealed
IsHandRevealed: @ 0x0800A368
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	ldr r7, _0800A3D8 @ =0x020192E4
	mov r2, #1
	and r0, r2
	ldr r1, _0800A3DC @ =0x00000D64
	mul r0, r1
	add r0, r0, r7
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1F
	cmp r0, #0
	bne _0800A3D2
	sub r5, r2, r4
	ldr r1, _0800A3E0 @ =0x00000445
	add r0, r5, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	bgt _0800A3D2
	ldr r6, _0800A3E4 @ =0x00000461
	add r0, r4, #0
	add r1, r6, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	bgt _0800A3D2
	add r0, r5, #0
	add r1, r6, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	bgt _0800A3D2
	ldr r1, _0800A3E8 @ =0x00001B0E
	add r0, r7, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	cmp r4, r0
	bne _0800A3EC
	mov r6, #0x90
	lsl r6, r6, #3
	add r0, r4, #0
	add r1, r6, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	bgt _0800A3D2
	add r0, r5, #0
	add r1, r6, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	ble _0800A3EC
_0800A3D2:
	mov r0, #1
	b _0800A3EE
	.align 2, 0
_0800A3D8: .4byte 0x020192E4
_0800A3DC: .4byte 0x00000D64
_0800A3E0: .4byte 0x00000445
_0800A3E4: .4byte 0x00000461
_0800A3E8: .4byte 0x00001B0E
_0800A3EC:
	mov r0, #0
_0800A3EE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end IsHandRevealed

