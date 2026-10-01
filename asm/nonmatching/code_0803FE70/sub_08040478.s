	thumb_func_start sub_08040478
sub_08040478: @ 0x08040478
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	mov r2, #1
	ldrb r0, [r6, #2]
	and r2, r0
	cmp r2, #0
	beq _08040534
	mov r0, #8
	neg r0, r0
	ldrb r1, [r6, #0xA]
	and r0, r1
	strb r0, [r6, #0xA]
	mov r5, #0
	ldr r2, _08040524 @ =0x0201930C
	mov ip, r2
	mov r3, #1
	mov r9, r3
	ldr r4, _08040528 @ =0x00000D64
	mov r8, r4
	ldr r7, _0804052C @ =0x000007FF
_080404A6:
	mov r2, #5
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r4, r8
	mul r4, r0
_080404B2:
	mov r0, #0x94
	mul r0, r2
	add r0, r0, r4
	mov r3, ip
	add r1, r0, r3
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	cmp r3, #0
	beq _080404E6
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080404E6
	and r3, r7
	lsl r0, r3, #2
	ldr r1, _08040530 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080405A0
_080404E6:
	add r2, #1
	cmp r2, #0xA
	ble _080404B2
	mov r2, #5
	mov r0, #1
	and r0, r5
	ldr r1, _08040528 @ =0x00000D64
	mul r0, r1
	ldr r1, _08040524 @ =0x0201930C
	mov r3, #2
	add r0, r0, r1
	mov r4, #0xB9
	lsl r4, r4, #2
	add r1, r0, r4
_08040502:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08040514
	add r0, r3, #0
	ldrb r4, [r1, #6]
	and r0, r4
	cmp r0, #0
	beq _080405A6
_08040514:
	add r1, #0x94
	add r2, #1
	cmp r2, #0xA
	ble _08040502
	add r5, #1
	cmp r5, #1
	ble _080404A6
	b _080405C8
_08040524: .4byte 0x0201930C
_08040528: .4byte 0x00000D64
_0804052C: .4byte 0x000007FF
_08040530: .4byte gUnk_08621DE0
_08040534:
	ldr r0, _08040560 @ =0x02017A40
	ldr r1, _08040564 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _08040574
	ldr r0, _08040568 @ =0x00000206
	ldr r1, _0804056C @ =0x00000712
	ldr r3, _08040570 @ =0x08084704
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r6, #0xA]
	and r0, r2
	strb r0, [r6, #0xA]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
	b _080405CA
_08040560: .4byte 0x02017A40
_08040564: .4byte 0x000003E5
_08040568: .4byte 0x00000206
_0804056C: .4byte 0x00000712
_08040570: .4byte gUnk_08084704
_08040574:
	ldr r1, _08040588 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804058C
	strb r2, [r4]
	mov r0, #0
	b _080405CA
	.align 2, 0
_08040588: .4byte 0x03000040
_0804058C:
	ldr r0, _0804059C @ =0x000E000E
	bl sub_08052F38
	cmp r0, #0
	bne _080405AC
	mov r0, #0
	b _080405CA
	.align 2, 0
_0804059C: .4byte 0x000E000E
_080405A0:
	add r0, r6, #0
	add r1, r5, #0
	b _080405C4
_080405A6:
	add r0, r6, #0
	add r1, r5, #0
	b _080405C4
_080405AC:
	ldr r0, _080405D8 @ =0x0201CFB0
	ldr r3, _080405DC @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	ldr r4, _080405E0 @ =0x00000828
	add r2, r0, r4
	add r3, #8
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r6, #0
_080405C4:
	bl sub_0803DDAC
_080405C8:
	mov r0, #1
_080405CA:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080405D8: .4byte 0x0201CFB0
_080405DC: .4byte 0x00000824
_080405E0: .4byte 0x00000828
	thumb_func_end sub_08040478

