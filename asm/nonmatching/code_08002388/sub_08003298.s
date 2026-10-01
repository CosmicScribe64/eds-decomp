	thumb_func_start sub_08003298
sub_08003298: @ 0x08003298
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	mov r0, #4
	mov sl, r0
	mov r1, r8
	cmp r1, #3
	bgt _080032B2
	mov r2, #5
	mov sl, r2
_080032B2:
	mov r0, #0
	bl sub_08002D34
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r4, _080033A0 @ =0x08198440
	mov r3, r8
	lsl r5, r3, #3
	add r1, r5, r4
	ldr r1, [r1]
	mov r2, #0x80
	lsl r2, r2, #2
	bl sub_08075294
	mov r0, #0xC0
	lsl r0, r0, #0x13
	add r4, #4
	add r5, r5, r4
	ldr r1, [r5]
	mov r4, #0x96
	lsl r4, r4, #8
	add r2, r4, #0
	bl sub_08075294
	ldr r0, _080033A4 @ =0x0600A000
	ldr r1, [r5]
	add r2, r4, #0
	bl sub_08075294
	ldr r2, _080033A8 @ =0x0201F7E0
	mov r1, #7
	mov r0, r8
	and r0, r1
	mov r1, #8
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
	mov r0, r8
	cmp r0, #4
	bne _08003326
	mov r0, #0x38
	and r0, r1
	cmp r0, #0
	bne _08003326
	mov r0, #0x39
	neg r0, r0
	and r0, r1
	mov r1, #0x20
	orr r0, r1
	strb r0, [r2]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1D
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl sub_0800323C
_08003326:
	mov r0, #0x20
	mov r1, #0xA
	mov r2, #0
	mov r3, #0
	bl sub_08074B38
	mov r7, #0
	cmp r7, sl
	bge _080033F8
	ldr r1, _080033AC @ =0x080813B8
	mov r9, r1
_0800333C:
	add r5, r7, #0
	mov r2, sl
	cmp r2, #5
	beq _08003346
	add r5, r7, #1
_08003346:
	ldr r1, _080033B0 @ =0x0819834C
	mov r3, r8
	lsl r0, r3, #2
	add r0, r8
	add r4, r0, r5
	lsl r0, r4, #1
	add r0, r0, r1
	ldrh r0, [r0]
	bl sub_08063DAC
	cmp r0, #0
	bne _080033BC
	mov r0, r9
	bl sub_080753CC
	lsl r1, r0, #2
	add r1, r1, r0
	lsr r0, r1, #0x1F
	add r1, r1, r0
	asr r1, r1, #1
	mov r0, #0x31
	sub r0, r0, r1
	lsl r4, r5, #4
	add r1, r4, #1
	ldr r2, _080033B4 @ =0x00000A05
	mov r3, r9
	bl sub_0807501C
	mov r0, r9
	bl sub_080753CC
	lsl r1, r0, #2
	add r1, r1, r0
	lsr r0, r1, #0x1F
	add r1, r1, r0
	asr r1, r1, #1
	mov r0, #0x30
	sub r0, r0, r1
	add r1, r4, #0
	ldr r2, _080033B8 @ =0x00000A01
	mov r3, r9
	bl sub_0807501C
	b _080033F2
	.align 2, 0
_080033A0: .4byte gUnk_08198440
_080033A4: .4byte 0x0600A000
_080033A8: .4byte 0x0201F7E0
_080033AC: .4byte gUnk_080813B8
_080033B0: .4byte gUnk_0819834C
_080033B4: .4byte 0x00000A05
_080033B8: .4byte 0x00000A01
_080033BC:
	ldr r1, _0800348C @ =0x08198468
	lsl r0, r4, #2
	add r0, r0, r1
	ldr r6, [r0]
	add r0, r6, #0
	bl sub_080753CC
	lsl r4, r0, #2
	add r4, r4, r0
	lsr r0, r4, #0x1F
	add r4, r4, r0
	asr r4, r4, #1
	mov r0, #0x31
	sub r0, r0, r4
	lsl r5, r5, #4
	add r1, r5, #1
	ldr r2, _08003490 @ =0x00000A05
	add r3, r6, #0
	bl sub_0807501C
	mov r0, #0x30
	sub r0, r0, r4
	add r1, r5, #0
	ldr r2, _08003494 @ =0x00000A01
	add r3, r6, #0
	bl sub_0807501C
_080033F2:
	add r7, #1
	cmp r7, sl
	blt _0800333C
_080033F8:
	ldr r0, _08003498 @ =0x06015800
	mov r1, #0
	bl sub_08075114
	mov r0, #0x20
	mov r1, #2
	mov r2, #0
	mov r3, #0
	bl sub_08074B38
	mov r7, #0
	mov r0, #0xA0
	lsl r0, r0, #4
	mov r8, r0
	mov r6, #0x50
	mov r5, #0x51
_08003418:
	ldr r1, _0800349C @ =0x081984CC
	lsl r0, r7, #2
	add r0, r0, r1
	ldr r4, [r0]
	add r2, r7, #6
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r1, r8
	orr r2, r1
	add r0, r5, #0
	mov r1, #1
	add r3, r4, #0
	bl sub_0807501C
	add r2, r7, #2
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r3, r8
	orr r2, r3
	add r0, r6, #0
	mov r1, #0
	add r3, r4, #0
	bl sub_0807501C
	add r6, #0x20
	add r5, #0x20
	add r7, #1
	cmp r7, #2
	ble _08003418
	ldr r5, _080034A0 @ =0x06015000
	add r0, r5, #0
	mov r1, #0
	bl sub_08075114
	ldr r0, _080034A4 @ =0x05000260
	ldr r1, _080034A8 @ =0x0871C850
	mov r2, #0x20
	bl sub_08075294
	ldr r1, _080034AC @ =0x0871CA50
	mov r4, #0xA0
	lsl r4, r4, #1
	add r0, r5, #0
	add r2, r4, #0
	bl sub_08075294
	ldr r0, _080034B0 @ =0x06015400
	ldr r1, _080034B4 @ =0x0871CB90
	add r2, r4, #0
	bl sub_08075294
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800348C: .4byte gUnk_08198468
_08003490: .4byte 0x00000A05
_08003494: .4byte 0x00000A01
_08003498: .4byte 0x06015800
_0800349C: .4byte gUnk_081984CC
_080034A0: .4byte 0x06015000
_080034A4: .4byte 0x05000260
_080034A8: .4byte gUnk_0871C850
_080034AC: .4byte gUnk_0871CA50
_080034B0: .4byte 0x06015400
_080034B4: .4byte gUnk_0871CB90
	thumb_func_end sub_08003298

