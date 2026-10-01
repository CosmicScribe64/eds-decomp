	thumb_func_start sub_08026030
sub_08026030: @ 0x08026030
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, _080260D0 @ =0x0201F820
	mov r1, #0xAF
	lsl r1, r1, #4
	add r0, r7, #0
	bl sub_08075278
	bl sub_08076F9C
	add r4, r0, #0
	lsr r0, r4, #0x1F
	add r0, r4, r0
	asr r0, r0, #1
	lsl r0, r0, #1
	sub r4, r4, r0
	lsl r4, r4, #0x18
	ldr r0, _080260D4 @ =0x02017A30
	ldrh r0, [r0, #6]
	ldr r1, _080260D8 @ =0x00000ACA
	add r6, r7, r1
	strb r0, [r6]
	ldr r5, _080260DC @ =0x08082308
	lsr r4, r4, #0x17
	sub r0, #1
	mov r1, #6
	bl __modsi3
	lsl r0, r0, #2
	add r0, r4, r0
	add r0, r0, r5
	ldrb r0, [r0]
	ldr r2, _080260E0 @ =0x00000AC7
	add r2, r2, r7
	mov r8, r2
	strb r0, [r2]
	ldrb r0, [r6]
	sub r0, #1
	mov r1, #6
	bl __modsi3
	lsl r0, r0, #2
	add r4, r4, r0
	add r5, #1
	add r4, r4, r5
	ldrb r1, [r4]
	add r2, r1, #2
	add r0, r2, #0
	asr r0, r0, #2
	lsl r0, r0, #2
	sub r0, r2, r0
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	lsl r0, r2, #2
	add r0, r0, r2
	add r0, #2
	ldr r3, _080260E4 @ =0x00000AC9
	add r1, r7, r3
	strb r0, [r1]
	ldr r3, _080260E8 @ =0x080822FC
	add r1, r2, #2
	add r0, r1, #0
	asr r0, r0, #2
	lsl r0, r0, #2
	sub r0, r1, r0
	mov r4, r8
	ldrb r4, [r4]
	lsl r1, r4, #2
	add r0, r0, r1
	add r0, r0, r3
	ldrb r0, [r0]
	strb r0, [r6]
	mov r0, #1
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080260D0: .4byte 0x0201F820
_080260D4: .4byte 0x02017A30
_080260D8: .4byte 0x00000ACA
_080260DC: .4byte gUnk_08082308
_080260E0: .4byte 0x00000AC7
_080260E4: .4byte 0x00000AC9
_080260E8: .4byte gUnk_080822FC
	thumb_func_end sub_08026030

