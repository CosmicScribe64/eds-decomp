	thumb_func_start sub_080122B4
sub_080122B4: @ 0x080122B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, _080122F4 @ =0x020185C0
	ldrh r0, [r4]
	lsr r7, r0, #0xF
	ldrh r6, [r4, #2]
	ldrh r1, [r4, #4]
	mov sl, r1
	ldr r2, _080122F8 @ =0x0000080A
	add r2, r2, r4
	mov r8, r2
	ldrb r3, [r2]
	lsl r0, r3, #0x19
	lsr r5, r0, #0x19
	cmp r5, #0
	beq _08012300
	cmp r5, #1
	beq _080123AC
	bl sub_080611AC
	ldr r0, _080122FC @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _080124B0
_080122F4: .4byte 0x020185C0
_080122F8: .4byte 0x0000080A
_080122FC: .4byte 0x0000080D
_08012300:
	mov r0, #0x10
	bl sub_08077AEC
	mov r4, #1
	mov r0, #2
	neg r0, r0
	ldr r2, [sp, #0]
	and r2, r0
	orr r2, r7
	sub r0, #0x1D
	and r2, r0
	ldr r0, _08012390 @ =0x000001FF
	and r0, r6
	lsl r0, r0, #5
	ldr r1, _08012394 @ =0xFFFFC01F
	and r2, r1
	orr r2, r0
	str r2, [sp, #0]
	add r1, r7, #0
	and r1, r4
	mov r0, #0x94
	add r3, r6, #0
	mul r3, r0
	ldr r0, _08012398 @ =0x00000D64
	mul r0, r1
	add r3, r3, r0
	ldr r0, _0801239C @ =0x0201930C
	add r3, r3, r0
	ldrb r1, [r3, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r4
	lsl r0, r0, #0xE
	ldr r1, _080123A0 @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	and r0, r4
	lsl r0, r0, #0xF
	ldr r2, _080123A4 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldr r1, _080123A8 @ =0x08690D0C
	mov r2, #0x10
	neg r2, r2
	mov r3, #0x20
	neg r3, r3
	mov r0, sp
	bl sub_08024380
	add r0, r7, #0
	add r1, r6, #0
	bl sub_08060FD0
	mov r3, r8
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
	b _080124B0
	.align 2, 0
_08012390: .4byte 0x000001FF
_08012394: .4byte 0xFFFFC01F
_08012398: .4byte 0x00000D64
_0801239C: .4byte 0x0201930C
_080123A0: .4byte 0xFFFFBFFF
_080123A4: .4byte 0xFFFF7FFF
_080123A8: .4byte gUnk_08690D0C
_080123AC:
	ldr r0, _080124C0 @ =0x00000814
	add r0, r0, r4
	mov r9, r0
	add r0, r7, #0
	and r0, r5
	ldr r1, _080124C4 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov r8, r2
	ldr r1, _080124C8 @ =0x0201930C
	add r1, r8
	mov r0, #0x94
	add r4, r6, #0
	mul r4, r0
	add r1, r1, r4
	mov r0, r9
	bl sub_08007558
	add r0, r7, #0
	add r1, r6, #0
	mov r2, sl
	bl sub_08008CFC
	mov r3, r9
	ldrh r3, [r3]
	lsl r0, r3, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080124CC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _080124D0 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08012494
	mov r1, sp
	mov r3, #2
	neg r3, r3
	mov ip, r3
	mov r0, ip
	ldrb r1, [r1]
	and r0, r1
	orr r0, r7
	mov r1, sp
	strb r0, [r1]
	mov r7, #0x1F
	neg r7, r7
	and r0, r7
	strb r0, [r1]
	ldr r1, _080124D4 @ =0x000001FF
	add r0, r1, #0
	and r6, r0
	lsl r2, r6, #5
	mov r1, sp
	ldr r6, _080124D8 @ =0xFFFFC01F
	add r0, r6, #0
	ldrh r1, [r1]
	and r0, r1
	orr r0, r2
	mov r1, sp
	strh r0, [r1]
	mov r2, r8
	add r3, r4, r2
	ldr r0, _080124C8 @ =0x0201930C
	add r3, r3, r0
	ldrb r2, [r3, #6]
	lsl r1, r2, #0x1F
	mov r4, sp
	lsr r1, r1, #0x1F
	lsl r1, r1, #6
	ldrb r2, [r4, #1]
	mov r0, #0x41
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4, #1]
	ldrb r3, [r3, #6]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1F
	lsl r1, r1, #7
	mov r2, #0x7F
	and r0, r2
	orr r0, r1
	strb r0, [r4, #1]
	mov r3, r9
	ldr r1, [r3]
	lsl r0, r1, #0x13
	lsr r0, r0, #0x1F
	and r0, r5
	ldr r2, [sp, #4]
	mov r3, ip
	and r2, r3
	orr r2, r0
	str r2, [sp, #4]
	mov r0, #0xE
	mov r3, sl
	cmp r3, #0
	beq _08012474
	mov r0, #0xF
_08012474:
	lsl r0, r0, #1
	and r2, r7
	orr r2, r0
	and r2, r6
	ldr r0, _080124DC @ =0xFFFFBFFF
	and r2, r0
	mov r0, #0x80
	lsl r0, r0, #8
	orr r2, r0
	str r2, [sp, #4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
_08012494:
	ldr r2, _080124E0 @ =0x020185C0
	ldr r0, _080124E4 @ =0x0000080A
	add r2, r2, r0
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
_080124B0:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080124C0: .4byte 0x00000814
_080124C4: .4byte 0x00000D64
_080124C8: .4byte 0x0201930C
_080124CC: .4byte gUnk_08622AB4
_080124D0: .4byte 0xFFFFF880
_080124D4: .4byte 0x000001FF
_080124D8: .4byte 0xFFFFC01F
_080124DC: .4byte 0xFFFFBFFF
_080124E0: .4byte 0x020185C0
_080124E4: .4byte 0x0000080A
	thumb_func_end sub_080122B4

