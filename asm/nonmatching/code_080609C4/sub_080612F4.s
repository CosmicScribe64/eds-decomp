	thumb_func_start sub_080612F4
sub_080612F4: @ 0x080612F4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	mov r9, r1
	cmp r2, #5
	beq _0806131C
	cmp r2, #5
	bgt _08061314
	cmp r2, #0
	bne _08061312
	b _08061448
_08061312:
	b _08061562
_08061314:
	cmp r2, #0xA
	bne _0806131A
	b _08061448
_0806131A:
	b _08061562
_0806131C:
	mov r7, #0
	ldr r0, _08061394 @ =0x0201930C
	mov ip, r0
_08061322:
	mov r4, #0
	add r1, r7, #1
	str r1, [sp, #0]
_08061328:
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r5, _08061398 @ =0x00000D64
	add r0, r1, #0
	mul r0, r5
	add r2, r2, r0
	mov r3, ip
	add r2, ip
	ldr r0, [r2]
	lsl r0, r0, #0x14
	add r1, r4, #1
	mov sl, r1
	cmp r0, #0
	beq _08061438
	mov r0, #2
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #0
	beq _08061438
	mov r6, #0
	add r0, r2, #0
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r6, r0
	bge _08061438
_08061360:
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r0, r2, #0
	mul r0, r5
	add r1, r1, r0
	add r1, r1, r3
	lsl r2, r6, #1
	add r0, r1, #0
	add r0, #0xA
	add r0, r0, r2
	ldrh r3, [r0]
	add r1, #0x4A
	add r1, r1, r2
	ldrb r0, [r1]
	sub r0, #1
	cmp r0, #9
	bhi _08061416
	lsl r0, r0, #2
	ldr r1, _0806139C @ =0x080613A0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08061394: .4byte 0x0201930C
_08061398: .4byte 0x00000D64
_0806139C: .4byte 0x080613A0
_080613A0:
	.4byte _080613C8
	.4byte _080613F0
	.4byte _08061416
	.4byte _08061416
	.4byte _080613C8
	.4byte _08061416
	.4byte _080613F0
	.4byte _08061416
	.4byte _08061416
	.4byte _080613C8
_080613C8:
	mov r2, r8
	lsl r0, r2, #0x18
	mov r2, r9
	lsl r1, r2, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	cmp r3, r0
	bne _08061416
	lsl r1, r7, #0x18
	lsl r0, r4, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	mov r2, #0x83
	lsl r2, r2, #2
	add r0, r3, #0
	bl sub_0806120C
	b _08061416
_080613F0:
	mov r1, r8
	lsl r0, r1, #0x18
	mov r2, r9
	lsl r1, r2, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	cmp r3, r0
	bne _08061416
	lsl r1, r7, #0x18
	lsl r0, r4, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	mov r2, #0x86
	lsl r2, r2, #2
	add r0, r3, #0
	bl sub_0806120C
_08061416:
	add r6, #1
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r5, _0806144C @ =0x00000D64
	add r0, r2, #0
	mul r0, r5
	add r1, r1, r0
	ldr r3, _08061450 @ =0x0201930C
	add r1, r1, r3
	add r1, #0x8A
	mov ip, r3
	ldrh r1, [r1]
	cmp r6, r1
	blt _08061360
_08061438:
	mov r4, sl
	cmp r4, #0xA
	bgt _08061440
	b _08061328
_08061440:
	ldr r7, [sp, #0]
	cmp r7, #1
	bgt _08061448
	b _08061322
_08061448:
	mov r6, #0
	b _080614FC
_0806144C: .4byte 0x00000D64
_08061450: .4byte 0x0201930C
_08061454:
	mov r1, #1
	mov r0, r8
	and r1, r0
	mov r0, #0x94
	mov r4, r9
	mul r4, r0
	add r0, r4, #0
	mul r1, r3
	add r0, r0, r1
	add r2, r0, r2
	lsl r1, r6, #1
	add r0, r2, #0
	add r0, #0xA
	add r0, r0, r1
	ldrh r3, [r0]
	add r0, r2, #0
	add r0, #0x4A
	add r0, r0, r1
	ldrb r1, [r0]
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0806148A
	mov r0, r8
	cmp r0, #0
	bne _080614FA
_0806148A:
	sub r0, r1, #1
	cmp r0, #9
	bhi _080614FA
	lsl r0, r0, #2
	ldr r1, _0806149C @ =0x080614A0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0806149C: .4byte 0x080614A0
_080614A0:
	.4byte _080614C8
	.4byte _080614E2
	.4byte _080614FA
	.4byte _080614FA
	.4byte _080614C8
	.4byte _080614FA
	.4byte _080614E2
	.4byte _080614FA
	.4byte _080614FA
	.4byte _080614C8
_080614C8:
	mov r2, r8
	lsl r1, r2, #0x18
	mov r4, r9
	lsl r0, r4, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	mov r2, #0x83
	lsl r2, r2, #2
	add r0, r3, #0
	bl sub_0806120C
	b _080614FA
_080614E2:
	mov r0, r8
	lsl r1, r0, #0x18
	mov r2, r9
	lsl r0, r2, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	mov r2, #0x86
	lsl r2, r2, #2
	add r0, r3, #0
	bl sub_0806120C
_080614FA:
	add r6, #1
_080614FC:
	mov r1, #1
	mov r3, r8
	and r1, r3
	mov r0, #0x94
	mov r4, r9
	mul r4, r0
	add r0, r4, #0
	ldr r3, _08061574 @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r2, _08061578 @ =0x0201930C
	add r0, r0, r2
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r6, r0
	blt _08061454
	ldr r3, _0806157C @ =0x020192E4
	mov r2, #1
	mov r0, r8
	and r0, r2
	ldr r1, _08061574 @ =0x00000D64
	mul r1, r0
	add r1, r1, r3
	ldrb r0, [r1, #0xB]
	lsr r3, r0, #4
	add r0, r2, #0
	ldrb r1, [r1, #0xC]
	and r0, r1
	lsl r0, r0, #4
	orr r0, r3
	mov r1, r9
	asr r0, r1
	and r0, r2
	cmp r0, #0
	beq _08061562
	mov r3, r8
	lsl r2, r3, #0x18
	lsr r2, r2, #0x18
	mov r4, #0xF0
	lsl r4, r4, #4
	add r1, r4, #0
	add r0, r2, #0
	orr r0, r1
	mov r3, r9
	lsl r1, r3, #0x18
	lsr r1, r1, #0x10
	orr r1, r2
	mov r2, #0x86
	lsl r2, r2, #2
	bl sub_0806120C
_08061562:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08061574: .4byte 0x00000D64
_08061578: .4byte 0x0201930C
_0806157C: .4byte 0x020192E4
	thumb_func_end sub_080612F4

