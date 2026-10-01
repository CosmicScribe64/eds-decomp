	thumb_func_start sub_080671E8
sub_080671E8: @ 0x080671E8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	ldr r3, _0806721C @ =0x0201DB20
	ldr r0, _08067220 @ =0x000018AC
	add r1, r3, r0
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r1, r8
	cmp r1, #2
	bne _08067224
	mov r2, #0xC5
	lsl r2, r2, #3
	add r3, r3, r2
	mov r0, #0
	mov r1, #6
	mov r2, #1
	bl sub_0807B100
	b _08067236
	.align 2, 0
_0806721C: .4byte 0x0201DB20
_08067220: .4byte 0x000018AC
_08067224:
	mov r2, #1
	neg r2, r2
	mov r4, #0xC5
	lsl r4, r4, #3
	add r3, r3, r4
	mov r0, #6
	mov r1, #0
	bl sub_0807B100
_08067236:
	ldr r2, _08067298 @ =0x0201DB20
	ldr r7, _0806729C @ =0x00000634
	add r5, r2, r7
	mov r0, #1
	ldrb r1, [r5]
	eor r0, r1
	strb r0, [r5]
	ldr r3, _080672A0 @ =0x00001C1C
	add r0, r2, r3
	ldrb r4, [r0]
	lsl r6, r4, #1
	mov r7, #0xA5
	lsl r7, r7, #5
	add r0, r2, r7
	add r0, r4, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r6, r0
	sub r7, #0xC
	add r1, r2, r7
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _080672A8
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r2, r1
	add r0, r6, r0
	ldrh r2, [r0]
	add r0, r4, #0
	add r1, r3, #0
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrb r2, [r5]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _080672A4 @ =0x06008000
	add r1, r1, r3
	bl sub_0807AFFC
	b _080672C2
	.align 2, 0
_08067298: .4byte 0x0201DB20
_0806729C: .4byte 0x00000634
_080672A0: .4byte 0x00001C1C
_080672A4: .4byte 0x06008000
_080672A8:
	str r0, [sp, #4]
	ldrb r4, [r5]
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r1, r0, #4
	sub r1, r1, r0
	lsl r1, r1, #7
	ldr r7, _08067308 @ =0x06008000
	add r1, r1, r7
	ldr r2, _0806730C @ =0x010005A0
	add r0, sp, #4
	bl CpuFastSet
_080672C2:
	mov r0, r8
	cmp r0, #3
	bne _08067320
	ldr r4, _08067310 @ =0x0201DB20
	mov r1, #0xC6
	lsl r1, r1, #3
	add r5, r4, r1
	mov r1, #0xFF
	add r0, r1, #0
	ldrh r2, [r5]
	and r0, r2
	lsr r0, r0, #3
	add r0, #9
	ldr r3, _08067314 @ =0x00000632
	add r2, r4, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldr r7, _08067318 @ =0x00000634
	add r2, r4, r7
	ldrb r2, [r2]
	mov r3, #0
	str r3, [sp, #0]
	mov r3, #1
	bl sub_08064E28
	ldrh r0, [r5]
	sub r0, #0x50
	strh r0, [r5]
	ldr r0, _0806731C @ =0x00000635
	add r4, r4, r0
	mov r0, #4
	b _08067356
	.align 2, 0
_08067308: .4byte 0x06008000
_0806730C: .4byte 0x010005A0
_08067310: .4byte 0x0201DB20
_08067314: .4byte 0x00000632
_08067318: .4byte 0x00000634
_0806731C: .4byte 0x00000635
_08067320:
	ldr r4, _08067400 @ =0x0201DB20
	mov r1, #0xC6
	lsl r1, r1, #3
	add r2, r4, r1
	mov r1, #0xFF
	add r0, r1, #0
	ldrh r2, [r2]
	and r0, r2
	lsr r0, r0, #3
	add r0, #0x1D
	ldr r3, _08067404 @ =0x00000632
	add r2, r4, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldr r7, _08067408 @ =0x00000634
	add r2, r4, r7
	ldrb r2, [r2]
	mov r3, #0
	str r3, [sp, #0]
	mov r3, #1
	bl sub_08064E28
	ldr r0, _0806740C @ =0x00000635
	add r4, r4, r0
	mov r0, #3
_08067356:
	strb r0, [r4]
	ldr r2, _08067400 @ =0x0201DB20
	ldr r3, _08067410 @ =0x00001710
	add r1, r2, r3
	mov r0, #1
	ldrb r4, [r1]
	orr r0, r4
	strb r0, [r1]
	ldr r7, _08067414 @ =0x00001C1C
	add r5, r2, r7
	ldrb r1, [r5]
	lsl r3, r1, #1
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r2, r4
	add r1, r1, r0
	ldrb r7, [r1]
	lsl r0, r7, #1
	add r1, r7, #0
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r3, r0
	sub r4, #0xC
	add r1, r2, r4
	add r0, r0, r1
	ldrh r0, [r0]
	mov r7, #0xC4
	lsl r7, r7, #3
	add r4, r2, r7
	add r3, r3, r4
	ldrh r1, [r3]
	ldr r3, _08067418 @ =0x00001BB0
	add r2, r2, r3
	bl sub_08065F34
	ldrb r5, [r5]
	lsl r0, r5, #1
	add r0, r0, r4
	ldrh r0, [r0]
	sub r0, #2
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r7, #0
_080673AC:
	lsl r0, r5, #0x10
	cmp r0, #0
	blt _08067428
	ldr r6, _08067400 @ =0x0201DB20
	ldr r4, _08067414 @ =0x00001C1C
	add r0, r6, r4
	ldrb r3, [r0]
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r6, r1
	add r0, r3, r0
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	add r0, r0, r3
	lsl r0, r0, #1
	ldr r4, _0806741C @ =0x00001494
	add r1, r6, r4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r5, r0
	bcs _08067428
	add r0, r3, #0
	add r1, r2, #0
	add r2, r5, #0
	bl sub_08068D1C
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r4, r7, #1
	lsl r2, r4, #0x18
	lsr r2, r2, #0x18
	ldr r3, _08067420 @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r7, #0
	ldr r7, _08067424 @ =0x00001BB8
	add r3, r6, r7
	bl sub_0806664C
	b _08067438
_08067400: .4byte 0x0201DB20
_08067404: .4byte 0x00000632
_08067408: .4byte 0x00000634
_0806740C: .4byte 0x00000635
_08067410: .4byte 0x00001710
_08067414: .4byte 0x00001C1C
_08067418: .4byte 0x00001BB0
_0806741C: .4byte 0x00001494
_08067420: .4byte 0x000018B0
_08067424: .4byte 0x00001BB8
_08067428:
	ldr r0, _08067464 @ =0x0201DB20
	lsl r1, r7, #4
	add r1, r1, r0
	ldr r0, _08067468 @ =0x00001BC4
	add r1, r1, r0
	mov r0, #0
	strb r0, [r1]
	add r4, r7, #1
_08067438:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r0, r4, #0x18
	lsr r7, r0, #0x18
	cmp r7, #4
	bls _080673AC
	ldr r1, _08067464 @ =0x0201DB20
	ldr r3, _0806746C @ =0x00001C14
	add r2, r1, r3
	mov r0, #0
	strb r0, [r2]
	ldr r4, _08067470 @ =0x00001BB8
	add r1, r1, r4
	mov r0, #5
	strb r0, [r1]
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08067464: .4byte 0x0201DB20
_08067468: .4byte 0x00001BC4
_0806746C: .4byte 0x00001C14
_08067470: .4byte 0x00001BB8
	thumb_func_end sub_080671E8

