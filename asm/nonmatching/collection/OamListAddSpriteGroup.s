	thumb_func_start OamListAddSpriteGroup
OamListAddSpriteGroup: @ 0x08077EF4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	add r6, r0, #0
	ldr r0, [sp, #0x50]
	ldr r4, [sp, #0x54]
	ldr r5, [sp, #0x58]
	mov sl, r5
	ldr r5, [sp, #0x60]
	mov ip, r5
	ldr r5, [sp, #0x64]
	mov r8, r5
	ldr r5, [sp, #0x68]
	mov r9, r5
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0x10]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0x14]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0x18]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x1C]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	add r2, r4, #0
	mov r0, sl
	lsl r0, r0, #0x18
	mov sl, r0
	ldr r1, [sp, #0x5C]
	lsl r5, r1, #0x18
	lsr r5, r5, #0x18
	str r5, [sp, #0x20]
	mov r3, ip
	lsl r3, r3, #0x18
	lsr r5, r3, #0x18
	str r5, [sp, #0x24]
	mov r0, r8
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	mov r3, r9
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0x2C]
	mov r0, #0xC0
	lsl r0, r0, #0x12
	mov r5, sl
	and r0, r5
	lsr r5, r0, #0x18
	cmp r1, #0
	bne _08077F68
	b _08078090
_08077F68:
	cmp r1, #1
	beq _08077F6E
	b _080784C8
_08077F6E:
	cmp r4, #1
	beq _08077FCC
	cmp r4, #1
	bgt _08077F7C
	cmp r4, #0
	beq _08077F82
	b _080784C8
_08077F7C:
	cmp r4, #2
	beq _0807802E
	b _080784C8
_08077F82:
	mov r4, #0
	ldr r0, [sp, #0x14]
	cmp r4, r0
	bcc _08077F8C
	b _080784C8
_08077F8C:
	ldr r1, [sp, #0x20]
	lsl r1, r1, #4
	mov r8, r1
_08077F92:
	ldr r2, [sp, #0x6C]
	str r2, [sp, #0]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	bl OamListAddTemplate
	add r7, r0, #0
	ldrh r1, [r6, #4]
	add r1, r8
	ldr r3, [sp, #0x24]
	lsl r0, r3, #9
	orr r1, r0
	lsl r0, r5, #0xA
	orr r1, r0
	strh r1, [r7, #4]
	ldr r0, [sp, #0x2C]
	ldrh r1, [r7]
	orr r0, r1
	strh r0, [r7]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r2, [sp, #0x14]
	cmp r4, r2
	bcc _08077F92
	b _080784C8
_08077FCC:
	mov r4, #0
	ldr r3, [sp, #0x14]
	cmp r4, r3
	bcc _08077FD6
	b _080784C8
_08077FD6:
	ldr r7, [sp, #0x18]
	lsl r7, r7, #0x10
	mov r9, r7
	ldr r0, [sp, #0x1C]
	lsl r0, r0, #0x10
	mov r8, r0
_08077FE2:
	str r5, [sp, #0]
	ldr r1, [sp, #0x20]
	str r1, [sp, #4]
	ldr r2, [sp, #0x24]
	str r2, [sp, #8]
	ldr r3, [sp, #0x6C]
	str r3, [sp, #0xC]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	mov r7, r9
	asr r2, r7, #0x10
	mov r7, r8
	asr r3, r7, #0x10
	bl OamListAddTemplateAt
	add r7, r0, #0
	ldr r1, [sp, #0x20]
	lsl r0, r1, #4
	ldrh r2, [r6, #4]
	add r0, r2, r0
	ldr r3, [sp, #0x24]
	lsl r1, r3, #9
	orr r0, r1
	lsl r1, r5, #0xA
	orr r0, r1
	strh r0, [r7, #4]
	ldr r0, [sp, #0x2C]
	ldrh r1, [r7]
	orr r0, r1
	strh r0, [r7]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r2, [sp, #0x14]
	cmp r4, r2
	bcc _08077FE2
	b _080784C8
_0807802E:
	mov r4, #0
	ldr r3, [sp, #0x14]
	cmp r4, r3
	bcc _08078038
	b _080784C8
_08078038:
	ldr r7, [sp, #0x18]
	lsl r7, r7, #0x10
	mov r9, r7
	ldr r0, [sp, #0x1C]
	lsl r0, r0, #0x10
	mov r8, r0
_08078044:
	str r5, [sp, #0]
	ldr r1, [sp, #0x20]
	str r1, [sp, #4]
	ldr r2, [sp, #0x24]
	str r2, [sp, #8]
	ldr r3, [sp, #0x6C]
	str r3, [sp, #0xC]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	mov r7, r9
	asr r2, r7, #0x10
	mov r7, r8
	asr r3, r7, #0x10
	bl OamListAddTemplateOffset
	add r7, r0, #0
	ldr r1, [sp, #0x20]
	lsl r0, r1, #4
	ldrh r2, [r6, #4]
	add r0, r2, r0
	ldr r3, [sp, #0x24]
	lsl r1, r3, #9
	orr r0, r1
	lsl r1, r5, #0xA
	orr r0, r1
	strh r0, [r7, #4]
	ldr r0, [sp, #0x2C]
	ldrh r1, [r7]
	orr r0, r1
	strh r0, [r7]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r2, [sp, #0x14]
	cmp r4, r2
	bcc _08078044
	b _080784C8
_08078090:
	cmp r2, #8
	bls _08078096
	b _080784C8
_08078096:
	lsl r0, r2, #2
	ldr r1, _080780A0 @ =0x080780A4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080780A0: .4byte 0x080780A4
_080780A4:
	.4byte _080780C8
	.4byte _08078124
	.4byte _0807819C
	.4byte _08078214
	.4byte _080782E0
	.4byte _080784C8
	.4byte _080784C8
	.4byte _080784C8
	.4byte _080783D4
_080780C8:
	mov r4, #0
	ldr r3, [sp, #0x14]
	cmp r4, r3
	bcc _080780D2
	b _080784C8
_080780D2:
	ldr r7, [sp, #0x6C]
	str r7, [sp, #0]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	bl OamListAddTemplate
	add r7, r0, #0
	ldrh r2, [r6, #4]
	ldr r1, _08078120 @ =0x0000FF0F
	add r0, r1, #0
	add r1, r2, #0
	and r1, r0
	ldr r3, [sp, #0x20]
	lsl r0, r3, #4
	add r1, r1, r0
	mov r0, #0xF0
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	ldr r2, [sp, #0x24]
	lsl r0, r2, #9
	orr r1, r0
	lsl r0, r5, #0xA
	orr r1, r0
	strh r1, [r7, #4]
	ldr r0, [sp, #0x2C]
	ldrh r3, [r7]
	orr r0, r3
	strh r0, [r7]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x14]
	cmp r4, r0
	bcc _080780D2
	b _080784C8
_08078120: .4byte 0x0000FF0F
_08078124:
	mov r4, #0
	ldr r1, [sp, #0x14]
	cmp r4, r1
	bcc _0807812E
	b _080784C8
_0807812E:
	ldr r2, [sp, #0x18]
	lsl r2, r2, #0x10
	mov r9, r2
	ldr r3, [sp, #0x1C]
	lsl r3, r3, #0x10
	mov r8, r3
_0807813A:
	str r5, [sp, #0]
	ldr r7, [sp, #0x20]
	str r7, [sp, #4]
	ldr r0, [sp, #0x24]
	str r0, [sp, #8]
	ldr r1, [sp, #0x6C]
	str r1, [sp, #0xC]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	mov r3, r9
	asr r2, r3, #0x10
	mov r7, r8
	asr r3, r7, #0x10
	bl OamListAddTemplateAt
	add r7, r0, #0
	ldrh r2, [r6, #4]
	ldr r1, _08078198 @ =0x0000FF0F
	add r0, r1, #0
	add r1, r2, #0
	and r1, r0
	ldr r3, [sp, #0x20]
	lsl r0, r3, #4
	add r1, r1, r0
	mov r0, #0xF0
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	ldr r2, [sp, #0x24]
	lsl r0, r2, #9
	orr r1, r0
	lsl r0, r5, #0xA
	orr r1, r0
	strh r1, [r7, #4]
	ldr r0, [sp, #0x2C]
	ldrh r3, [r7]
	orr r0, r3
	strh r0, [r7]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x14]
	cmp r4, r0
	bcc _0807813A
	b _080784C8
	.align 2, 0
_08078198: .4byte 0x0000FF0F
_0807819C:
	mov r4, #0
	ldr r1, [sp, #0x14]
	cmp r4, r1
	bcc _080781A6
	b _080784C8
_080781A6:
	ldr r2, [sp, #0x18]
	lsl r2, r2, #0x10
	mov r9, r2
	ldr r3, [sp, #0x1C]
	lsl r3, r3, #0x10
	mov r8, r3
_080781B2:
	str r5, [sp, #0]
	ldr r7, [sp, #0x20]
	str r7, [sp, #4]
	ldr r0, [sp, #0x24]
	str r0, [sp, #8]
	ldr r1, [sp, #0x6C]
	str r1, [sp, #0xC]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	mov r3, r9
	asr r2, r3, #0x10
	mov r7, r8
	asr r3, r7, #0x10
	bl OamListAddTemplateOffset
	add r7, r0, #0
	ldrh r2, [r6, #4]
	ldr r1, _08078210 @ =0x0000FF0F
	add r0, r1, #0
	add r1, r2, #0
	and r1, r0
	ldr r3, [sp, #0x20]
	lsl r0, r3, #4
	add r1, r1, r0
	mov r0, #0xF0
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	ldr r2, [sp, #0x24]
	lsl r0, r2, #9
	orr r1, r0
	lsl r0, r5, #0xA
	orr r1, r0
	strh r1, [r7, #4]
	ldr r0, [sp, #0x2C]
	ldrh r3, [r7]
	orr r0, r3
	strh r0, [r7]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x14]
	cmp r4, r0
	bcc _080781B2
	b _080784C8
	.align 2, 0
_08078210: .4byte 0x0000FF0F
_08078214:
	mov r4, #0
	ldr r1, [sp, #0x14]
	cmp r4, r1
	bcc _0807821E
	b _080784C8
_0807821E:
	mov r2, #0x80
	lsl r2, r2, #2
	mov sl, r2
	ldr r3, _08078264 @ =0x0000FC0F
	mov r9, r3
	mov r7, #0xF0
	mov r8, r7
	lsl r5, r5, #0xA
_0807822E:
	ldr r0, [sp, #0x6C]
	str r0, [sp, #0]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	bl OamListAddTemplate
	add r7, r0, #0
	ldrh r2, [r6, #4]
	mov r1, #0xE0
	lsl r1, r1, #3
	add r0, r1, #0
	add r1, r0, #0
	and r1, r2
	cmp r1, sl
	beq _08078288
	cmp r1, sl
	bgt _08078268
	cmp r1, #0
	beq _0807827A
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r1, r0
	beq _08078280
	b _080782C2
	.align 2, 0
_08078264: .4byte 0x0000FC0F
_08078268:
	mov r0, #0xC0
	lsl r0, r0, #2
	cmp r1, r0
	beq _0807829A
	mov r0, #0x80
	lsl r0, r0, #3
	cmp r1, r0
	beq _080782AE
	b _080782C2
_0807827A:
	mov r1, r9
	and r1, r2
	b _080782B6
_08078280:
	mov r1, r9
	and r1, r2
	add r1, #0x10
	b _080782B6
_08078288:
	mov r1, r9
	and r1, r2
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	mov r2, sl
	orr r1, r2
	b _080782BE
_0807829A:
	mov r1, r9
	and r1, r2
	add r1, #0x10
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	mov r3, sl
	orr r1, r3
	b _080782BE
_080782AE:
	ldr r1, _080782DC @ =0x0000F80F
	add r0, r1, #0
	add r1, r2, #0
	and r1, r0
_080782B6:
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
_080782BE:
	orr r1, r5
	strh r1, [r7, #4]
_080782C2:
	ldr r0, [sp, #0x2C]
	ldrh r2, [r7]
	orr r0, r2
	strh r0, [r7]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r3, [sp, #0x14]
	cmp r4, r3
	bcc _0807822E
	b _080784C8
	.align 2, 0
_080782DC: .4byte 0x0000F80F
_080782E0:
	mov r4, #0
	ldr r0, [sp, #0x14]
	cmp r4, r0
	bcc _080782EA
	b _080784C8
_080782EA:
	mov r1, #0x80
	lsl r1, r1, #2
	mov sl, r1
	ldr r2, _08078330 @ =0x0000FC0F
	mov r9, r2
	mov r3, #0xF0
	mov r8, r3
	lsl r5, r5, #0xA
_080782FA:
	ldr r7, [sp, #0x6C]
	str r7, [sp, #0]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	bl OamListAddTemplate
	add r7, r0, #0
	ldrh r2, [r6, #4]
	mov r1, #0xE0
	lsl r1, r1, #3
	add r0, r1, #0
	add r1, r0, #0
	and r1, r2
	cmp r1, sl
	beq _08078354
	cmp r1, sl
	bgt _08078334
	cmp r1, #0
	beq _08078346
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r1, r0
	beq _0807834C
	b _0807838E
	.align 2, 0
_08078330: .4byte 0x0000FC0F
_08078334:
	mov r0, #0xC0
	lsl r0, r0, #2
	cmp r1, r0
	beq _08078366
	mov r0, #0x80
	lsl r0, r0, #3
	cmp r1, r0
	beq _0807837A
	b _0807838E
_08078346:
	mov r1, r9
	and r1, r2
	b _08078382
_0807834C:
	mov r1, r9
	and r1, r2
	add r1, #0x10
	b _08078382
_08078354:
	mov r1, r9
	and r1, r2
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	mov r2, sl
	orr r1, r2
	b _0807838A
_08078366:
	mov r1, r9
	and r1, r2
	add r1, #0x10
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	mov r3, sl
	orr r1, r3
	b _0807838A
_0807837A:
	ldr r1, _080783CC @ =0x0000F80F
	add r0, r1, #0
	add r1, r2, #0
	and r1, r0
_08078382:
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
_0807838A:
	orr r1, r5
	strh r1, [r7, #4]
_0807838E:
	mov r2, #0xFF
	lsl r2, r2, #8
	add r1, r2, #0
	ldrh r3, [r6]
	and r1, r3
	mov r0, #0xFF
	ldr r2, [sp, #0x1C]
	and r0, r2
	orr r1, r0
	ldr r3, [sp, #0x2C]
	orr r1, r3
	strh r1, [r7]
	mov r0, #0xFE
	lsl r0, r0, #8
	add r1, r0, #0
	ldrh r2, [r6, #2]
	and r1, r2
	ldr r3, _080783D0 @ =0x000001FF
	add r2, r3, #0
	ldr r0, [sp, #0x18]
	and r0, r2
	orr r1, r0
	strh r1, [r7, #2]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x14]
	cmp r4, r0
	bcc _080782FA
	b _080784C8
_080783CC: .4byte 0x0000F80F
_080783D0: .4byte 0x000001FF
_080783D4:
	mov r4, #0
	ldr r1, [sp, #0x14]
	cmp r4, r1
	bcs _080784C8
	mov r2, #0x80
	lsl r2, r2, #2
	mov sl, r2
	ldr r3, _08078420 @ =0x0000FC0F
	mov r9, r3
	mov r7, #0xF0
	mov r8, r7
	lsl r5, r5, #0xA
_080783EC:
	ldr r0, [sp, #0x6C]
	str r0, [sp, #0]
	add r0, r6, #0
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	bl OamListAddTemplate
	add r7, r0, #0
	ldrh r2, [r6, #4]
	mov r1, #0xE0
	lsl r1, r1, #3
	add r0, r1, #0
	add r1, r0, #0
	and r1, r2
	cmp r1, sl
	beq _08078444
	cmp r1, sl
	bgt _08078424
	cmp r1, #0
	beq _08078436
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r1, r0
	beq _0807843C
	b _0807847E
_08078420: .4byte 0x0000FC0F
_08078424:
	mov r0, #0xC0
	lsl r0, r0, #2
	cmp r1, r0
	beq _08078456
	mov r0, #0x80
	lsl r0, r0, #3
	cmp r1, r0
	beq _0807846A
	b _0807847E
_08078436:
	mov r1, r9
	and r1, r2
	b _08078472
_0807843C:
	mov r1, r9
	and r1, r2
	add r1, #0x10
	b _08078472
_08078444:
	mov r1, r9
	and r1, r2
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	mov r2, sl
	orr r1, r2
	b _0807847A
_08078456:
	mov r1, r9
	and r1, r2
	add r1, #0x10
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	mov r3, sl
	orr r1, r3
	b _0807847A
_0807846A:
	ldr r1, _080784DC @ =0x0000F80F
	add r0, r1, #0
	add r1, r2, #0
	and r1, r0
_08078472:
	mov r0, r8
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
_0807847A:
	orr r1, r5
	strh r1, [r7, #4]
_0807847E:
	ldrh r2, [r6]
	mov r3, #0xFF
	lsl r3, r3, #8
	add r0, r3, #0
	add r1, r2, #0
	and r1, r0
	mov r3, #0xFF
	mov r0, #0xFF
	and r0, r2
	ldr r2, [sp, #0x1C]
	add r0, r2, r0
	and r0, r3
	orr r1, r0
	ldr r3, [sp, #0x2C]
	orr r1, r3
	strh r1, [r7]
	ldrh r3, [r6, #2]
	mov r1, #0xFE
	lsl r1, r1, #8
	add r0, r1, #0
	add r1, r3, #0
	and r1, r0
	ldr r2, _080784E0 @ =0x000001FF
	add r0, r2, #0
	and r0, r3
	ldr r3, [sp, #0x18]
	add r0, r3, r0
	and r0, r2
	orr r1, r0
	strh r1, [r7, #2]
	add r6, #8
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x14]
	cmp r4, r0
	bcc _080783EC
_080784C8:
	add r0, r7, #0
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080784DC: .4byte 0x0000F80F
_080784E0: .4byte 0x000001FF
	thumb_func_end OamListAddSpriteGroup

