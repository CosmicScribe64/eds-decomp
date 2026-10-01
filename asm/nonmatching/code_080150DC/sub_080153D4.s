	thumb_func_start sub_080153D4
sub_080153D4: @ 0x080153D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r4, _080153FC @ =0x020185C0
	ldr r1, _08015400 @ =0x0000080A
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	cmp r0, #7
	bls _080153F2
	b _080156F8
_080153F2:
	lsl r0, r0, #2
	ldr r1, _08015404 @ =0x08015408
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080153FC: .4byte 0x020185C0
_08015400: .4byte 0x0000080A
_08015404: .4byte 0x08015408
_08015408:
	.4byte _08015428
	.4byte _08015440
	.4byte _0801547C
	.4byte _08015494
	.4byte _080154AC
	.4byte _080154C4
	.4byte _080154F4
	.4byte _080156D0
_08015428:
	mov r0, #0
	mov r1, #0
	bl sub_080240A8
	ldr r2, _08015438 @ =0x020185C0
	ldr r3, _0801543C @ =0x0000080A
	add r2, r2, r3
	b _08015456
_08015438: .4byte 0x020185C0
_0801543C: .4byte 0x0000080A
_08015440:
	bl sub_080619E8
	ldr r2, _08015470 @ =0x020185C0
	ldr r7, _08015474 @ =0x0000080C
	add r1, r2, r7
	ldr r0, _08015478 @ =0xFFFFF01F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	sub r7, #2
	add r2, r2, r7
_08015456:
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _0801570A
	.align 2, 0
_08015470: .4byte 0x020185C0
_08015474: .4byte 0x0000080C
_08015478: .4byte 0xFFFFF01F
_0801547C:
	ldr r1, _0801548C @ =0x020185C0
	ldrh r0, [r1, #2]
	bl sub_08061A1C
	ldr r2, _0801548C @ =0x020185C0
	ldr r7, _08015490 @ =0x0000080A
	add r3, r2, r7
	b _080156D6
_0801548C: .4byte 0x020185C0
_08015490: .4byte 0x0000080A
_08015494:
	ldr r1, _080154A4 @ =0x020185C0
	ldrh r0, [r1, #2]
	bl sub_08061D24
	ldr r2, _080154A4 @ =0x020185C0
	ldr r7, _080154A8 @ =0x0000080A
	add r3, r2, r7
	b _080156D6
_080154A4: .4byte 0x020185C0
_080154A8: .4byte 0x0000080A
_080154AC:
	ldr r1, _080154BC @ =0x020185C0
	ldrh r0, [r1, #2]
	bl sub_08061E54
	ldr r2, _080154BC @ =0x020185C0
	ldr r7, _080154C0 @ =0x0000080A
	add r3, r2, r7
	b _080156D6
_080154BC: .4byte 0x020185C0
_080154C0: .4byte 0x0000080A
_080154C4:
	bl sub_0805ED9C
	ldr r4, _080154EC @ =0x020185C0
	ldrh r0, [r4, #2]
	bl sub_0805F00C
	ldr r0, _080154F0 @ =0x0000080A
	add r4, r4, r0
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _0801570A
_080154EC: .4byte 0x020185C0
_080154F0: .4byte 0x0000080A
_080154F4:
	mov r1, #0
	ldr r6, _08015574 @ =0x04000050
	ldr r2, _08015578 @ =0x04000052
	mov r9, r2
_080154FC:
	mov r5, #0
	lsl r3, r1, #5
	mov r8, r3
	lsl r0, r1, #1
	add r1, #1
	mov sl, r1
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	str r0, [sp, #0]
_08015510:
	lsl r1, r5, #5
	add r4, r1, #0
	add r4, #0x44
	mov r2, r8
	add r2, #2
	ldr r7, _0801557C @ =0x020185C0
	ldr r3, _08015580 @ =0x0000080C
	add r0, r7, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1F
	bgt _0801554A
	sub r4, #0x68
	sub r2, #0x40
	mul r4, r0
	mul r2, r0
	add r0, r4, #0
	cmp r4, #0
	bge _0801553A
	add r0, #0x1F
_0801553A:
	asr r4, r0, #5
	add r0, r2, #0
	cmp r2, #0
	bge _08015544
	add r0, #0x1F
_08015544:
	asr r2, r0, #5
	add r4, #0x68
	add r2, #0x40
_0801554A:
	ldr r7, _0801557C @ =0x020185C0
	ldr r1, _08015580 @ =0x0000080C
	add r0, r7, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0xF
	bgt _08015584
	mov r7, #0xF4
	lsl r7, r7, #4
	add r0, r7, #0
	strh r0, [r6]
	mov r0, #0x10
	sub r0, r0, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r3, r0
	mov r0, r9
	strh r3, [r0]
	b _080155F0
	.align 2, 0
_08015574: .4byte 0x04000050
_08015578: .4byte 0x04000052
_0801557C: .4byte 0x020185C0
_08015580: .4byte 0x0000080C
_08015584:
	add r0, r3, #0
	sub r0, #0x10
	cmp r0, #0x1F
	bhi _080155A4
	ldr r1, _0801559C @ =0x000027A7
	add r0, r1, #0
	strh r0, [r6]
	add r0, r3, #0
	sub r0, #0x10
	ldr r3, _080155A0 @ =0x04000054
	strh r0, [r3]
	b _080155F0
_0801559C: .4byte 0x000027A7
_080155A0: .4byte 0x04000054
_080155A4:
	add r0, r3, #0
	sub r0, #0x30
	cmp r0, #0x1F
	bhi _080155C4
	ldr r7, _080155BC @ =0x000027A7
	add r0, r7, #0
	strh r0, [r6]
	mov r0, #0x4F
	sub r0, r0, r3
	ldr r1, _080155C0 @ =0x04000054
	b _080155EE
	.align 2, 0
_080155BC: .4byte 0x000027A7
_080155C0: .4byte 0x04000054
_080155C4:
	cmp r3, #0x67
	ble _080155E8
	mov r7, #0xF4
	lsl r7, r7, #4
	add r0, r7, #0
	strh r0, [r6]
	mov r1, #0x78
	sub r1, r1, r3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r3, #0
	sub r0, #0x68
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	mov r0, r9
	strh r1, [r0]
	b _080155F0
_080155E8:
	mov r0, #0
	strh r0, [r6]
	mov r1, r9
_080155EE:
	strh r0, [r1]
_080155F0:
	ldr r0, _08015620 @ =0x020185C0
	ldr r3, _08015624 @ =0x0000080C
	add r0, r0, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0xF
	bgt _0801562C
	lsl r0, r2, #0x10
	orr r4, r0
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	ldr r7, [sp, #0]
	add r2, r2, r7
	ldr r1, _08015628 @ =0x081A4424
	lsl r0, r3, #1
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	add r0, r4, #0
	mov r1, #0x80
	bl sub_08076448
	b _08015640
_08015620: .4byte 0x020185C0
_08015624: .4byte 0x0000080C
_08015628: .4byte gUnk_081A4424
_0801562C:
	lsl r0, r2, #0x10
	orr r4, r0
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	ldr r0, [sp, #0]
	add r2, r2, r0
	add r0, r4, #0
	mov r1, #0x80
	bl sub_080763D0
_08015640:
	add r5, #1
	cmp r5, #3
	bgt _08015648
	b _08015510
_08015648:
	mov r1, sl
	cmp r1, #4
	bgt _08015650
	b _080154FC
_08015650:
	ldr r1, _080156AC @ =0x020185C0
	ldr r2, _080156B0 @ =0x0000080C
	add r4, r1, r2
	ldrh r3, [r4]
	lsl r0, r3, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x77
	bgt _080156C0
	ldr r1, _080156B4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08015678
	ldr r1, _080156B8 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801568C
_08015678:
	cmp r2, #0x6F
	bgt _0801568C
	add r0, r2, #7
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _080156BC @ =0xFFFFF01F
	and r1, r3
	orr r1, r0
	strh r1, [r4]
_0801568C:
	ldr r7, _080156AC @ =0x020185C0
	ldr r0, _080156B0 @ =0x0000080C
	add r3, r7, r0
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080156BC @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _0801570A
	.align 2, 0
_080156AC: .4byte 0x020185C0
_080156B0: .4byte 0x0000080C
_080156B4: .4byte 0x03000040
_080156B8: .4byte 0x0201CFB0
_080156BC: .4byte 0xFFFFF01F
_080156C0:
	ldr r1, _080156C8 @ =0x020185C0
	ldr r2, _080156CC @ =0x0000080A
	add r3, r1, r2
	b _080156D6
_080156C8: .4byte 0x020185C0
_080156CC: .4byte 0x0000080A
_080156D0:
	ldr r7, _080156F0 @ =0x020185C0
	ldr r0, _080156F4 @ =0x0000080A
	add r3, r7, r0
_080156D6:
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _0801570A
	.align 2, 0
_080156F0: .4byte 0x020185C0
_080156F4: .4byte 0x0000080A
_080156F8:
	bl sub_08060578
	ldr r2, _0801571C @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
_0801570A:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801571C: .4byte 0x0000080D
	thumb_func_end sub_080153D4

