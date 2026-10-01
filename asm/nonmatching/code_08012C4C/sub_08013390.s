	thumb_func_start sub_08013390
sub_08013390: @ 0x08013390
	push {r4, r5, r6, r7, lr}
	ldr r5, _080133AC @ =0x020185C0
	ldrh r0, [r5]
	lsr r2, r0, #0xF
	ldr r1, _080133B0 @ =0x0000080A
	add r7, r5, r1
	ldrb r1, [r7]
	lsl r0, r1, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _080133B4
	cmp r6, #1
	beq _08013428
	b _080134B4
_080133AC: .4byte 0x020185C0
_080133B0: .4byte 0x0000080A
_080133B4:
	ldr r4, _08013404 @ =0x0201CFB0
	ldr r2, _08013408 @ =0x00000808
	add r1, r4, r2
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _0801340C @ =0x050003E0
	ldr r1, _08013410 @ =0x08687B9C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _08013414 @ =0x06016C80
	ldr r1, _08013418 @ =0x086887BC
	mov r2, #0x80
	lsl r2, r2, #3
	bl sub_080752B0
	ldr r0, _0801341C @ =0x0000085C
	add r4, r4, r0
	str r6, [r4]
	ldr r2, _08013420 @ =0x0000080C
	add r1, r5, r2
	ldr r0, _08013424 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _080134D6
_08013404: .4byte 0x0201CFB0
_08013408: .4byte 0x00000808
_0801340C: .4byte 0x050003E0
_08013410: .4byte gUnk_08687B9C
_08013414: .4byte 0x06016C80
_08013418: .4byte gUnk_086887BC
_0801341C: .4byte 0x0000085C
_08013420: .4byte 0x0000080C
_08013424: .4byte 0xFFFFF01F
_08013428:
	ldr r0, _08013494 @ =0x0000080C
	add r7, r5, r0
	ldrh r1, [r7]
	lsl r0, r1, #0x14
	lsr r5, r0, #0x19
	cmp r5, #0x5F
	bgt _080134B4
	ldr r0, _08013498 @ =0x00300058
	ldr r1, _0801349C @ =0x000040C0
	ldr r2, _080134A0 @ =0x0000F364
	ldr r4, _080134A4 @ =0x081A43E4
	mov r3, #0x1F
	and r5, r3
	lsl r3, r5, #1
	add r3, r3, r4
	ldrh r3, [r3]
	lsl r3, r3, #0x10
	bl sub_08076714
	ldrh r1, [r7]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r4, #0x7F
	and r0, r4
	lsl r0, r0, #5
	ldr r3, _080134A8 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r7]
	ldr r1, _080134AC @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801347C
	ldr r0, _080134B0 @ =0x0201CFB0
	ldrb r0, [r0]
	and r6, r0
	cmp r6, #0
	beq _080134D6
_0801347C:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x77
	bgt _080134D6
	add r0, #7
	and r0, r4
	lsl r0, r0, #5
	and r2, r3
	orr r2, r0
	strh r2, [r7]
	b _080134D6
	.align 2, 0
_08013494: .4byte 0x0000080C
_08013498: .4byte 0x00300058
_0801349C: .4byte 0x000040C0
_080134A0: .4byte 0x0000F364
_080134A4: .4byte gUnk_081A43E4
_080134A8: .4byte 0xFFFFF01F
_080134AC: .4byte 0x03000040
_080134B0: .4byte 0x0201CFB0
_080134B4:
	ldr r1, _080134DC @ =0x020192E4
	ldr r0, _080134E0 @ =0x00000D64
	mul r0, r2
	add r0, r0, r1
	mov r1, #0
	strh r1, [r0]
	add r0, r2, #0
	bl sub_08060934
	ldr r1, _080134E4 @ =0x020185C0
	ldr r2, _080134E8 @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080134D6:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080134DC: .4byte 0x020192E4
_080134E0: .4byte 0x00000D64
_080134E4: .4byte 0x020185C0
_080134E8: .4byte 0x0000080D
	thumb_func_end sub_08013390

