	thumb_func_start sub_08039638
sub_08039638: @ 0x08039638
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sl, r0
	mov r0, #4
	mov r1, sl
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08039654
	b _0803978E
_08039654:
	ldr r0, _08039670 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08039750
	cmp r0, #0x7F
	bgt _08039674
	cmp r0, #0x7E
	bne _0803966C
	b _0803976C
_0803966C:
	b _0803978E
	.align 2, 0
_08039670: .4byte 0x02017A40
_08039674:
	cmp r0, #0x80
	beq _0803967A
	b _0803978E
_0803967A:
	mov r4, #0
	mov r8, r4
	mov r0, sl
	add r0, #0xC
	str r0, [sp, #0]
_08039684:
	mov r1, r8
	cmp r1, #0
	beq _08039698
	mov r2, sl
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r4, #1
	sub r6, r4, r0
	b _080396A0
_08039698:
	mov r1, sl
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r6, r0, #0x1F
_080396A0:
	lsl r0, r6, #1
	ldr r2, [sp, #0]
	add r3, r2, r0
	add r0, r6, #0
	mov r4, #1
	and r0, r4
	ldr r1, _0803971C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	ldr r0, _08039720 @ =0x020192E4
	add r1, r2, r0
	ldrb r0, [r1, #2]
	strh r0, [r3]
	mov r7, #0
	ldrb r1, [r1, #2]
	cmp r7, r1
	bge _080396F8
	ldr r1, _08039724 @ =0x02019968
	ldr r4, _08039728 @ =0xFFFFF97C
	add r0, r1, r4
	add r0, r0, r2
	mov r9, r0
	add r4, r2, r1
	add r5, r3, #0
_080396D0:
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	cmp r0, r6
	beq _080396E0
	ldrh r0, [r5]
	sub r0, #1
	strh r0, [r5]
_080396E0:
	add r0, r6, #0
	mov r1, #0
	mov r2, r8
	mov r3, #1
	bl sub_080193D4
	add r4, #4
	add r7, #1
	mov r0, r9
	ldrb r0, [r0, #2]
	cmp r7, r0
	blt _080396D0
_080396F8:
	mov r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #1
	ble _08039684
	mov r4, #0
	mov r8, r4
_08039706:
	mov r0, r8
	cmp r0, #0
	beq _0803972C
	mov r1, sl
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r2, r1, r0
	b _08039734
	.align 2, 0
_0803971C: .4byte 0x00000D64
_08039720: .4byte 0x020192E4
_08039724: .4byte 0x02019968
_08039728: .4byte 0xFFFFF97C
_0803972C:
	mov r2, sl
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r2, r0, #0x1F
_08039734:
	lsl r0, r2, #1
	ldr r4, [sp, #0]
	add r0, r4, r0
	ldrh r1, [r0]
	add r0, r2, #0
	bl sub_080199E0
	mov r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #1
	ble _08039706
	mov r0, #0x7F
	b _08039790
_08039750:
	mov r4, sl
	ldrb r4, [r4, #2]
	lsl r2, r4, #0x1F
	lsr r0, r2, #0x1F
	add r2, r0, #0
	lsl r2, r2, #1
	mov r1, sl
	add r1, #0xC
	add r1, r1, r2
	ldrh r1, [r1]
	bl sub_08046BE0
	mov r0, #0x7E
	b _08039790
_0803976C:
	mov r0, sl
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	mov r2, #1
	sub r0, r2, r0
	lsr r1, r1, #0x1F
	sub r2, r2, r1
	lsl r2, r2, #1
	mov r1, sl
	add r1, #0xC
	add r1, r1, r2
	ldrh r1, [r1]
	bl sub_08046BE0
	mov r0, #0x7D
	b _08039790
_0803978E:
	mov r0, #0
_08039790:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08039638

