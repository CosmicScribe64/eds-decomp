	thumb_func_start sub_08013190
sub_08013190: @ 0x08013190
	push {r4, r5, r6, r7, lr}
	ldr r1, _080131B0 @ =0x020185C0
	ldr r0, _080131B4 @ =0x0000080A
	add r7, r1, r0
	ldrb r2, [r7]
	lsl r0, r2, #0x19
	lsr r5, r0, #0x19
	add r6, r1, #0
	cmp r5, #1
	beq _08013234
	cmp r5, #1
	bgt _080131B8
	cmp r5, #0
	beq _080131C6
	b _08013378
	.align 2, 0
_080131B0: .4byte 0x020185C0
_080131B4: .4byte 0x0000080A
_080131B8:
	cmp r5, #2
	bne _080131BE
	b _080132CC
_080131BE:
	cmp r5, #3
	bne _080131C4
	b _08013324
_080131C4:
	b _08013378
_080131C6:
	ldrh r0, [r6, #2]
	cmp r0, #4
	bhi _08013212
	ldr r0, _08013274 @ =0x050003E0
	ldr r4, _08013278 @ =0x08198D68
	ldrh r2, [r6, #2]
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r4
	ldr r1, [r1]
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _0801327C @ =0x06016C80
	ldrh r2, [r6, #2]
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #2
	add r2, r4, #4
	add r1, r1, r2
	ldr r1, [r1]
	mov r2, #0x80
	lsl r2, r2, #4
	bl sub_080752B0
	ldrh r1, [r6, #2]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r4
	ldrh r0, [r0, #8]
	bl sub_08077B70
	ldr r0, _08013280 @ =0x0201CFB0
	ldr r2, _08013284 @ =0x0000085C
	add r0, r0, r2
	str r5, [r0]
_08013212:
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
	ldr r0, _08013288 @ =0x0000080C
	add r1, r6, r0
	ldr r0, _0801328C @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
_08013234:
	ldr r0, _08013288 @ =0x0000080C
	add r5, r6, r0
	ldrh r1, [r5]
	lsl r0, r1, #0x14
	lsr r4, r0, #0x19
	cmp r4, #0x3F
	bgt _08013298
	ldr r0, _08013290 @ =0x00200058
	ldr r2, _08013294 @ =0x0000F364
	lsl r1, r4, #5
	mov r3, #0x90
	lsl r3, r3, #4
	sub r3, r3, r1
	lsl r3, r3, #0x10
	lsl r1, r4, #2
	orr r3, r1
	mov r1, #0xC0
	bl sub_08076714
	ldrh r2, [r5]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _0801328C @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r5]
	b _08013386
	.align 2, 0
_08013274: .4byte 0x050003E0
_08013278: .4byte gUnk_08198D68
_0801327C: .4byte 0x06016C80
_08013280: .4byte 0x0201CFB0
_08013284: .4byte 0x0000085C
_08013288: .4byte 0x0000080C
_0801328C: .4byte 0xFFFFF01F
_08013290: .4byte 0x00200058
_08013294: .4byte 0x0000F364
_08013298:
	ldr r0, _080132C0 @ =0x00200058
	ldr r2, _080132C4 @ =0x0000F364
	mov r1, #0xC0
	bl sub_080761F0
	ldr r2, _080132C8 @ =0x0000080A
	add r3, r6, r2
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
	b _08013386
	.align 2, 0
_080132C0: .4byte 0x00200058
_080132C4: .4byte 0x0000F364
_080132C8: .4byte 0x0000080A
_080132CC:
	ldr r0, _08013310 @ =0x00200058
	ldr r2, _08013314 @ =0x0000F364
	mov r1, #0xC0
	bl sub_080761F0
	ldr r1, _08013318 @ =0x08198D68
	ldrh r2, [r6, #2]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r0, r1
	ldrh r0, [r0, #8]
	bl sub_0807E6B0
	cmp r0, #0
	bne _08013386
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
	ldr r0, _0801331C @ =0x0000080C
	add r1, r6, r0
	ldr r0, _08013320 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	b _08013386
_08013310: .4byte 0x00200058
_08013314: .4byte 0x0000F364
_08013318: .4byte gUnk_08198D68
_0801331C: .4byte 0x0000080C
_08013320: .4byte 0xFFFFF01F
_08013324:
	ldr r0, _08013350 @ =0x00200058
	ldr r2, _08013354 @ =0x0000F364
	mov r1, #0xC0
	bl sub_080761F0
	ldr r0, _08013358 @ =0x0000080C
	add r3, r6, r0
	ldrh r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1D
	bgt _08013360
	add r0, #1
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _0801335C @ =0xFFFFF01F
	and r1, r2
	orr r1, r0
	strh r1, [r3]
	b _08013386
	.align 2, 0
_08013350: .4byte 0x00200058
_08013354: .4byte 0x0000F364
_08013358: .4byte 0x0000080C
_0801335C: .4byte 0xFFFFF01F
_08013360:
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
	b _08013386
_08013378:
	ldr r2, _0801338C @ =0x0000080D
	add r1, r6, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08013386:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801338C: .4byte 0x0000080D
	thumb_func_end sub_08013190

