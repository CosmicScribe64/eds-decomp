	thumb_func_start CountOtherFaceUpSameNameMonsters
CountOtherFaceUpSameNameMonsters: @ 0x08009298
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r5, r0, #0
	mov sl, r1
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	mov r2, sl
	mul r2, r0
	ldr r0, _080092D4 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080092D8 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _080092D0
	cmp r3, #0
	bne _080092DC
_080092D0:
	mov r0, #0
	b _08009344
_080092D4: .4byte 0x00000D64
_080092D8: .4byte 0x0201930C
_080092DC:
	mov r7, #0
	mov r6, #0
	mov r0, #1
	mov r9, r0
_080092E4:
	mov r4, #0
	add r0, r6, #1
	mov r8, r0
_080092EA:
	add r2, r5, #0
	cmp r6, #0
	bne _080092F4
	mov r0, r9
	sub r2, r0, r2
_080092F4:
	cmp r5, r2
	bne _080092FC
	cmp sl, r4
	beq _08009336
_080092FC:
	mov r0, r9
	and r2, r0
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _08009354 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08009358 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08009336
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08009336
	add r0, r2, #0
	add r1, r3, #0
	str r3, [sp, #0]
	bl IsSameCardName
	ldr r3, [sp, #0]
	cmp r0, #0
	beq _08009336
	add r7, #1
_08009336:
	add r4, #1
	cmp r4, #4
	ble _080092EA
	mov r6, r8
	cmp r6, #1
	ble _080092E4
	add r0, r7, #0
_08009344:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08009354: .4byte 0x00000D64
_08009358: .4byte 0x0201930C
	thumb_func_end CountOtherFaceUpSameNameMonsters

