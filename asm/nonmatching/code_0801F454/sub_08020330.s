	thumb_func_start sub_08020330
sub_08020330: @ 0x08020330
	push {r4, r5, r6, r7, lr}
	sub sp, #0x100
	ldr r2, _08020354 @ =0x02017A40
	mov r0, #0xF4
	lsl r0, r0, #2
	add r3, r2, r0
	ldrb r1, [r3]
	lsr r0, r1, #1
	add r6, r2, #0
	cmp r0, #0xE
	bls _08020348
	b _08020AC8
_08020348:
	lsl r0, r0, #2
	ldr r1, _08020358 @ =0x0802035C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08020354: .4byte 0x02017A40
_08020358: .4byte 0x0802035C
_0802035C:
	.4byte _08020398
	.4byte _080203B6
	.4byte _080204D8
	.4byte _08020542
	.4byte _080205FC
	.4byte _0802066A
	.4byte _08020724
	.4byte _0802076A
	.4byte _08020814
	.4byte _0802083C
	.4byte _08020864
	.4byte _080208A8
	.4byte _08020948
	.4byte _080209C4
	.4byte _08020A50
_08020398:
	ldr r2, _080203F0 @ =0x000003D1
	add r1, r6, r2
	mov r0, #0
	strb r0, [r1]
	mov r4, #0xF4
	lsl r4, r4, #2
	add r3, r6, r4
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_080203B6:
	ldr r5, _080203F4 @ =0x02017A40
	ldr r0, _080203F0 @ =0x000003D1
	add r1, r5, r0
	ldrb r2, [r1]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r0, r5
	mov r3, #0xA0
	lsl r3, r3, #2
	add r0, r0, r3
	ldrh r0, [r0]
	bl sub_08047058
	add r4, r0, #0
	mov r0, #1
	neg r0, r0
	cmp r4, r0
	bne _080203FC
	mov r4, #0x90
	lsl r4, r4, #3
	add r0, r5, r4
	mov r1, #0
	str r1, [r0]
	ldr r2, _080203F8 @ =0x00000484
	add r0, r5, r2
	str r1, [r0]
	b _08020420
	.align 2, 0
_080203F0: .4byte 0x000003D1
_080203F4: .4byte 0x02017A40
_080203F8: .4byte 0x00000484
_080203FC:
	mov r0, #0x90
	lsl r0, r0, #3
	add r3, r5, r0
	ldr r2, _080204BC @ =0x0819A9D4
	lsl r1, r4, #1
	add r1, r1, r4
	lsl r1, r1, #3
	add r0, r2, #0
	add r0, #0x10
	add r0, r1, r0
	ldr r0, [r0]
	str r0, [r3]
	ldr r4, _080204C0 @ =0x00000484
	add r3, r5, r4
	add r2, #0x14
	add r1, r1, r2
	ldr r0, [r1]
	str r0, [r3]
_08020420:
	ldr r4, _080204C4 @ =0x02017A40
	ldr r0, _080204C8 @ =0x000003D1
	add r2, r4, r0
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r4
	mov r3, #0xA1
	lsl r3, r3, #2
	add r0, r0, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	cmp r0, #0
	beq _08020448
	mov r0, #0x90
	lsl r0, r0, #3
	add r1, r4, r0
	mov r0, #0
	str r0, [r1]
_08020448:
	ldrb r1, [r2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r2, r0, r4
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	bge _08020464
	ldr r3, _080204C0 @ =0x00000484
	add r1, r4, r3
	mov r0, #0
	str r0, [r1]
_08020464:
	ldr r1, _080204CC @ =0x00000282
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r3, #0xA0
	lsl r3, r3, #2
	add r1, r2, r3
	ldrh r1, [r1]
	bl sub_080197C0
	ldr r2, _080204D0 @ =0x02017FB0
	mov r0, #0xC2
	lsl r0, r0, #2
	add r2, r2, r0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #9
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	mov r2, #0xF9
	lsl r2, r2, #2
	add r0, r4, r2
	mov r1, #0
	strb r1, [r0]
	ldr r3, _080204D4 @ =0x000003E5
	add r0, r4, r3
	strb r1, [r0]
	mov r0, #0xF4
	lsl r0, r0, #2
	add r3, r4, r0
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08020AE0
	.align 2, 0
_080204BC: .4byte gUnk_0819A9D4
_080204C0: .4byte 0x00000484
_080204C4: .4byte 0x02017A40
_080204C8: .4byte 0x000003D1
_080204CC: .4byte 0x00000282
_080204D0: .4byte 0x02017FB0
_080204D4: .4byte 0x000003E5
_080204D8:
	mov r1, #0x90
	lsl r1, r1, #3
	add r0, r6, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _080204FC
	mov r2, #0xF4
	lsl r2, r2, #2
	add r3, r6, r2
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #2
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08020AE0
_080204FC:
	ldr r3, _08020590 @ =0x000003D1
	add r4, r6, r3
	ldrb r1, [r4]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	mov r2, #0xA0
	lsl r2, r2, #2
	add r5, r6, r2
	add r0, r0, r5
	bl sub_0801FA48
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802052C
	ldr r0, _08020594 @ =0x0000F091
	ldrb r3, [r4]
	lsl r1, r3, #2
	add r1, r1, r3
	lsl r1, r1, #2
	add r1, r1, r5
	mov r2, #0x14
	bl sub_080229BC
_0802052C:
	mov r4, #0xF4
	lsl r4, r4, #2
	add r3, r6, r4
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_08020542:
	ldr r0, _08020590 @ =0x000003D1
	add r5, r6, r0
	ldrb r1, [r5]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	mov r2, #0xA0
	lsl r2, r2, #2
	add r7, r6, r2
	add r0, r0, r7
	bl sub_0801FA48
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080205C6
	mov r3, #0xF0
	lsl r3, r3, #2
	add r2, r6, r3
	ldrh r4, [r2]
	cmp r4, #1
	bls _08020598
	mov r0, #0x90
	lsl r0, r0, #3
	add r3, r6, r0
	ldrb r1, [r5]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r7
	lsl r1, r4, #2
	add r1, r1, r4
	lsl r1, r1, #2
	mov r4, #0x96
	lsl r4, r4, #2
	add r2, r6, r4
	add r1, r1, r2
	ldr r2, [r3]
	b _080205AC
	.align 2, 0
_08020590: .4byte 0x000003D1
_08020594: .4byte 0x0000F091
_08020598:
	mov r3, #0x90
	lsl r3, r3, #3
	add r1, r6, r3
	ldrb r4, [r5]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r7
	ldr r2, [r1]
	mov r1, #0
_080205AC:
	bl _call_via_r2
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080205C6
	ldr r1, _080205F4 @ =0x02017FB0
	mov r0, #0xC2
	lsl r0, r0, #2
	add r1, r1, r0
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_080205C6:
	ldr r0, _080205F4 @ =0x02017FB0
	mov r3, #0xC2
	lsl r3, r3, #2
	add r0, r0, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	cmp r0, #0
	blt _080205D8
	b _08020AE0
_080205D8:
	ldr r2, _080205F8 @ =0x02017A40
	mov r4, #0xF4
	lsl r4, r4, #2
	add r2, r2, r4
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08020AE0
	.align 2, 0
_080205F4: .4byte 0x02017FB0
_080205F8: .4byte 0x02017A40
_080205FC:
	ldr r1, _08020620 @ =0x00000484
	add r0, r6, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _08020624
	mov r2, #0xF4
	lsl r2, r2, #2
	add r3, r6, r2
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #2
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08020AE0
	.align 2, 0
_08020620: .4byte 0x00000484
_08020624:
	ldr r3, _080206B4 @ =0x000003D1
	add r4, r6, r3
	ldrb r1, [r4]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	mov r2, #0xA0
	lsl r2, r2, #2
	add r5, r6, r2
	add r0, r0, r5
	bl sub_0801FA48
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08020654
	ldr r0, _080206B8 @ =0x0000F081
	ldrb r3, [r4]
	lsl r1, r3, #2
	add r1, r1, r3
	lsl r1, r1, #2
	add r1, r1, r5
	mov r2, #0x14
	bl sub_080229BC
_08020654:
	mov r4, #0xF4
	lsl r4, r4, #2
	add r3, r6, r4
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_0802066A:
	ldr r0, _080206B4 @ =0x000003D1
	add r5, r6, r0
	ldrb r1, [r5]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	mov r2, #0xA0
	lsl r2, r2, #2
	add r7, r6, r2
	add r0, r0, r7
	bl sub_0801FA48
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080206EC
	mov r3, #0xF0
	lsl r3, r3, #2
	add r2, r6, r3
	ldrh r4, [r2]
	cmp r4, #1
	bls _080206C0
	ldr r0, _080206BC @ =0x00000484
	add r3, r6, r0
	ldrb r1, [r5]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r7
	lsl r1, r4, #2
	add r1, r1, r4
	lsl r1, r1, #2
	mov r4, #0x96
	lsl r4, r4, #2
	add r2, r6, r4
	add r1, r1, r2
	ldr r2, [r3]
	b _080206D2
_080206B4: .4byte 0x000003D1
_080206B8: .4byte 0x0000F081
_080206BC: .4byte 0x00000484
_080206C0:
	ldr r3, _08020718 @ =0x00000484
	add r1, r6, r3
	ldrb r4, [r5]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r7
	ldr r2, [r1]
	mov r1, #0
_080206D2:
	bl _call_via_r2
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080206EC
	ldr r1, _0802071C @ =0x02017FB0
	mov r0, #0xC2
	lsl r0, r0, #2
	add r1, r1, r0
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_080206EC:
	ldr r0, _0802071C @ =0x02017FB0
	mov r3, #0xC2
	lsl r3, r3, #2
	add r0, r0, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	blt _080206FE
	b _08020AE0
_080206FE:
	ldr r2, _08020720 @ =0x02017A40
	mov r4, #0xF4
	lsl r4, r4, #2
	add r2, r2, r4
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08020AE0
_08020718: .4byte 0x00000484
_0802071C: .4byte 0x02017FB0
_08020720: .4byte 0x02017A40
_08020724:
	ldr r1, _08020750 @ =0x000003D1
	add r0, r6, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	mov r2, #0xF0
	lsl r2, r2, #2
	add r1, r6, r2
	ldrb r0, [r0]
	ldrh r1, [r1]
	cmp r0, r1
	bcs _08020754
	mov r3, #0xF4
	lsl r3, r3, #2
	add r0, r6, r3
	mov r1, #1
	ldrb r4, [r0]
	and r1, r4
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	b _08020AE0
_08020750: .4byte 0x000003D1
_08020754:
	mov r0, #0xF4
	lsl r0, r0, #2
	add r3, r6, r0
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_0802076A:
	ldr r1, _080207FC @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _080207E2
	ldr r0, _08020800 @ =0x0000F061
	mov r1, #0xF0
	lsl r1, r1, #2
	add r4, r6, r1
	ldrh r1, [r4]
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	mov r5, #0
	ldrh r4, [r4]
	cmp r5, r4
	bge _080207BE
	mov r2, #0xF0
	lsl r2, r2, #2
	add r7, r6, r2
	mov r3, #0xA0
	lsl r3, r3, #2
	add r4, r6, r3
_0802079C:
	mov r0, sp
	strh r5, [r0]
	add r0, #2
	add r1, r4, #0
	mov r2, #0x14
	bl sub_08075294
	ldr r0, _08020804 @ =0x0000F062
	mov r1, sp
	mov r2, #0x16
	bl sub_080229BC
	add r4, #0x14
	add r5, #1
	ldrh r0, [r7]
	cmp r5, r0
	blt _0802079C
_080207BE:
	ldr r0, _08020808 @ =0x0000F063
	ldr r1, _0802080C @ =0x02017A40
	mov r2, #0xF0
	lsl r2, r2, #2
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _08020810 @ =0x02017FB0
	mov r3, #0xC2
	lsl r3, r3, #2
	add r1, r1, r3
	mov r0, #0x7F
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
_080207E2:
	ldr r2, _0802080C @ =0x02017A40
	mov r0, #0xF4
	lsl r0, r0, #2
	add r2, r2, r0
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08020AE0
_080207FC: .4byte 0x02015EE8
_08020800: .4byte 0x0000F061
_08020804: .4byte 0x0000F062
_08020808: .4byte 0x0000F063
_0802080C: .4byte 0x02017A40
_08020810: .4byte 0x02017FB0
_08020814:
	ldr r4, _08020838 @ =0x02017CC0
	add r0, r4, #0
	mov r1, #0
	bl sub_0801A7B4
	mov r1, #0xA8
	lsl r1, r1, #1
	add r4, r4, r1
	ldrb r2, [r4]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _08020AE0
	.align 2, 0
_08020838: .4byte 0x02017CC0
_0802083C:
	bl sub_0801A32C
	cmp r0, #0
	bne _08020846
	b _08020AE0
_08020846:
	ldr r2, _08020860 @ =0x02017A40
	mov r3, #0xF4
	lsl r3, r3, #2
	add r2, r2, r3
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08020AE0
_08020860: .4byte 0x02017A40
_08020864:
	ldr r1, _08020900 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08020882
	ldr r0, _08020904 @ =0x02017FB0
	mov r4, #0xC2
	lsl r4, r4, #2
	add r0, r0, r4
	ldrb r0, [r0]
	lsr r0, r0, #7
	cmp r0, #0
	bne _08020882
	b _08020AE0
_08020882:
	mov r0, #0x91
	lsl r0, r0, #3
	add r1, r6, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r4, #0xF4
	lsl r4, r4, #2
	add r3, r6, r4
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_080208A8:
	ldr r4, _08020908 @ =0x02017A40
	mov r1, #0xF0
	lsl r1, r1, #2
	add r0, r4, r1
	ldrh r2, [r0]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	mov r3, #0x9B
	lsl r3, r3, #2
	add r1, r4, r3
	add r0, r0, r1
	sub r2, #1
	lsl r1, r2, #2
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r4
	ldr r2, _0802090C @ =0x00000282
	add r1, r1, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	mov r5, #1
	sub r1, r5, r1
	bl sub_0802D30C
	cmp r0, #0
	beq _08020914
	mov r3, #0x92
	lsl r3, r3, #3
	add r1, r4, r3
	mov r0, #0
	strb r0, [r1]
	ldr r0, _08020910 @ =0x00000491
	add r2, r4, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x7F
	and r0, r1
	strb r0, [r2]
	b _0802092A
	.align 2, 0
_08020900: .4byte 0x02015EE8
_08020904: .4byte 0x02017FB0
_08020908: .4byte 0x02017A40
_0802090C: .4byte 0x00000282
_08020910: .4byte 0x00000491
_08020914:
	mov r2, #0xF4
	lsl r2, r2, #2
	add r3, r4, r2
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	add r0, r5, #0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_0802092A:
	ldr r2, _08020944 @ =0x02017A40
	mov r3, #0xF4
	lsl r3, r3, #2
	add r2, r2, r3
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08020AE0
_08020944: .4byte 0x02017A40
_08020948:
	mov r4, #0xF0
	lsl r4, r4, #2
	add r0, r6, r4
	ldrh r2, [r0]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	mov r3, #0x9B
	lsl r3, r3, #2
	add r1, r6, r3
	add r0, r0, r1
	sub r2, #1
	lsl r1, r2, #2
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r6
	ldr r4, _080209A4 @ =0x00000282
	add r1, r1, r4
	ldrb r1, [r1]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	mov r4, #1
	sub r1, r4, r1
	bl sub_0801FEA0
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08020982
	b _08020AE0
_08020982:
	ldr r0, _080209A8 @ =0x00000491
	add r1, r6, r0
	mov r0, #0x80
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080209AC
	mov r1, #0xF4
	lsl r1, r1, #2
	add r0, r6, r1
	add r1, r4, #0
	ldrb r2, [r0]
	and r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	b _08020AE0
_080209A4: .4byte 0x00000282
_080209A8: .4byte 0x00000491
_080209AC:
	mov r0, #0xF4
	lsl r0, r0, #2
	add r3, r6, r0
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	add r0, r4, #0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08020AE0
_080209C4:
	ldr r4, _08020A10 @ =0x02017A40
	mov r1, #0xF0
	lsl r1, r1, #2
	add r0, r4, r1
	ldrh r2, [r0]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	mov r3, #0x9B
	lsl r3, r3, #2
	add r1, r4, r3
	add r0, r0, r1
	sub r2, #1
	lsl r1, r2, #2
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r4
	ldr r2, _08020A14 @ =0x00000282
	add r1, r1, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	bl sub_0802D30C
	cmp r0, #0
	beq _08020A1C
	mov r3, #0x92
	lsl r3, r3, #3
	add r1, r4, r3
	mov r0, #0
	strb r0, [r1]
	ldr r0, _08020A18 @ =0x00000491
	add r1, r4, r0
	mov r0, #0x7F
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _08020A32
_08020A10: .4byte 0x02017A40
_08020A14: .4byte 0x00000282
_08020A18: .4byte 0x00000491
_08020A1C:
	mov r0, #0xF4
	lsl r0, r0, #2
	add r3, r4, r0
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_08020A32:
	ldr r2, _08020A4C @ =0x02017A40
	mov r1, #0xF4
	lsl r1, r1, #2
	add r2, r2, r1
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08020AE0
_08020A4C: .4byte 0x02017A40
_08020A50:
	add r4, r6, #0
	mov r2, #0xF0
	lsl r2, r2, #2
	add r0, r4, r2
	ldrh r2, [r0]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	mov r3, #0x9B
	lsl r3, r3, #2
	add r1, r4, r3
	add r0, r0, r1
	sub r2, #1
	lsl r1, r2, #2
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r4
	ldr r2, _08020AA8 @ =0x00000282
	add r1, r1, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	bl sub_0801FEA0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08020AE0
	ldr r3, _08020AAC @ =0x00000491
	add r1, r4, r3
	mov r0, #0x80
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08020AB0
	mov r1, #0xF4
	lsl r1, r1, #2
	add r0, r4, r1
	mov r1, #1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	b _08020AE0
_08020AA8: .4byte 0x00000282
_08020AAC: .4byte 0x00000491
_08020AB0:
	mov r4, #0xF4
	lsl r4, r4, #2
	add r3, r6, r4
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08020AE0
_08020AC8:
	mov r0, #2
	neg r0, r0
	and r0, r1
	strb r0, [r3]
	ldr r0, _08020AEC @ =0x000003D2
	add r1, r2, r0
	mov r0, #1
	strb r0, [r1]
	ldr r3, _08020AF0 @ =0x000003D3
	add r1, r2, r3
	mov r0, #0
	strb r0, [r1]
_08020AE0:
	mov r0, #1
	add sp, #0x100
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08020AEC: .4byte 0x000003D2
_08020AF0: .4byte 0x000003D3
	thumb_func_end sub_08020330

