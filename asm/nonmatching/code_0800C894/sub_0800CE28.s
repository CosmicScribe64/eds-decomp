	thumb_func_start sub_0800CE28
sub_0800CE28: @ 0x0800CE28
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _0800CE5C @ =0x020185C0
	ldrh r1, [r0]
	lsr r6, r1, #0xF
	ldrh r7, [r0, #2]
	ldrh r2, [r0, #4]
	mov r8, r2
	ldr r3, _0800CE60 @ =0x0000080A
	add r1, r0, r3
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x19
	add r3, r0, #0
	cmp r1, #6
	bls _0800CE50
	b _0800D214
_0800CE50:
	lsl r0, r1, #2
	ldr r1, _0800CE64 @ =0x0800CE68
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800CE5C: .4byte 0x020185C0
_0800CE60: .4byte 0x0000080A
_0800CE64: .4byte 0x0800CE68
_0800CE68:
	.4byte _0800CE84
	.4byte _0800CEB4
	.4byte _0800CEE8
	.4byte _0800CF20
	.4byte _0800CFEC
	.4byte _0800D128
	.4byte _0800D188
_0800CE84:
	ldr r1, _0800CEA4 @ =0x0201CFB0
	ldr r0, _0800CEA8 @ =0x00000808
	add r1, r1, r0
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_08024134
	ldr r2, _0800CEAC @ =0x020185C0
	ldr r3, _0800CEB0 @ =0x0000080A
	add r2, r2, r3
	b _0800D150
_0800CEA4: .4byte 0x0201CFB0
_0800CEA8: .4byte 0x00000808
_0800CEAC: .4byte 0x020185C0
_0800CEB0: .4byte 0x0000080A
_0800CEB4:
	ldr r1, _0800CED8 @ =0x0201CFB0
	ldr r0, _0800CEDC @ =0x00000808
	add r1, r1, r0
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r0, #1
	sub r0, r0, r6
	mov r1, #0
	mov r2, r8
	bl sub_08024134
	ldr r2, _0800CEE0 @ =0x020185C0
	ldr r3, _0800CEE4 @ =0x0000080A
	add r2, r2, r3
	b _0800D150
	.align 2, 0
_0800CED8: .4byte 0x0201CFB0
_0800CEDC: .4byte 0x00000808
_0800CEE0: .4byte 0x020185C0
_0800CEE4: .4byte 0x0000080A
_0800CEE8:
	ldr r1, _0800CF0C @ =0x0201CFB0
	ldr r0, _0800CF10 @ =0x00000808
	add r1, r1, r0
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _0800CF14 @ =0x0000080C
	add r1, r3, r0
	ldr r0, _0800CF18 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r0, _0800CF1C @ =0x0000080A
	add r3, r3, r0
	b _0800D10A
	.align 2, 0
_0800CF0C: .4byte 0x0201CFB0
_0800CF10: .4byte 0x00000808
_0800CF14: .4byte 0x0000080C
_0800CF18: .4byte 0xFFFFF01F
_0800CF1C: .4byte 0x0000080A
_0800CF20:
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_080623AC
	add r4, r0, #0
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_080623EC
	add r4, #8
	add r0, #8
	lsl r0, r0, #0x10
	orr r4, r0
	ldr r0, _0800CF5C @ =0x020185C0
	ldr r1, _0800CF60 @ =0x0000080C
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	cmp r6, #0
	beq _0800CF64
	add r3, r0, #0
	add r3, #0x40
	mov r0, #0x80
	lsl r0, r0, #0x11
	b _0800CF68
	.align 2, 0
_0800CF5C: .4byte 0x020185C0
_0800CF60: .4byte 0x0000080C
_0800CF64:
	mov r3, #0x80
	lsl r3, r3, #0x11
_0800CF68:
	orr r3, r0
	add r0, r4, #0
	mov r1, #0x40
	mov r2, #0xA4
	lsl r2, r2, #7
	bl sub_08076714
	ldr r3, _0800CFC8 @ =0x020185C0
	ldr r2, _0800CFCC @ =0x0000080C
	add r6, r3, r2
	ldrh r0, [r6]
	lsl r1, r0, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r7, #0x7F
	and r1, r7
	lsl r1, r1, #5
	ldr r5, _0800CFD0 @ =0xFFFFF01F
	add r2, r5, #0
	and r2, r0
	orr r2, r1
	strh r2, [r6]
	lsl r0, r2, #0x14
	lsr r4, r0, #0x19
	cmp r4, #0x1F
	bgt _0800CFDC
	ldr r1, _0800CFD4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0800CFB6
	ldr r1, _0800CFD8 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800CFB6
	b _0800D222
_0800CFB6:
	cmp r4, #0x17
	ble _0800CFBC
	b _0800D222
_0800CFBC:
	add r0, r4, #7
	and r0, r7
	lsl r0, r0, #5
	and r2, r5
	b _0800D1EE
	.align 2, 0
_0800CFC8: .4byte 0x020185C0
_0800CFCC: .4byte 0x0000080C
_0800CFD0: .4byte 0xFFFFF01F
_0800CFD4: .4byte 0x03000040
_0800CFD8: .4byte 0x0201CFB0
_0800CFDC:
	and r2, r5
	strh r2, [r6]
	ldr r0, _0800CFE8 @ =0x0000080A
	add r3, r3, r0
	b _0800D10A
	.align 2, 0
_0800CFE8: .4byte 0x0000080A
_0800CFEC:
	mov r1, #1
	mov sl, r1
	sub r4, r1, r6
	add r0, r4, #0
	mov r1, #0
	mov r2, r8
	bl sub_080623AC
	add r5, r0, #0
	add r0, r4, #0
	mov r1, #0
	mov r2, r8
	bl sub_080623EC
	add r4, r0, #0
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_080623AC
	sub r5, r5, r0
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_080623EC
	sub r4, r4, r0
	ldr r2, _0800D0E8 @ =0x020185C0
	mov r9, r2
	ldr r3, _0800D0EC @ =0x0000080C
	add r3, r9
	mov r8, r3
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	mul r5, r0
	mul r4, r0
	add r0, r5, #0
	cmp r5, #0
	bge _0800D03E
	add r0, #0x1F
_0800D03E:
	asr r5, r0, #5
	add r0, r4, #0
	cmp r4, #0
	bge _0800D048
	add r0, #0x1F
_0800D048:
	asr r4, r0, #5
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_080623AC
	add r1, r5, #0
	add r1, #8
	add r5, r1, r0
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_080623EC
	add r1, r4, #0
	add r1, #8
	add r4, r1, r0
	lsl r4, r4, #0x10
	orr r4, r5
	ldr r1, _0800D0F0 @ =0x081A43E4
	mov r2, r8
	ldrh r2, [r2]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	cmp r6, #0
	beq _0800D088
	mov r0, #0x40
	orr r3, r0
_0800D088:
	add r0, r4, #0
	mov r1, #0x40
	mov r2, #0xA4
	lsl r2, r2, #7
	bl sub_08076714
	mov r3, r8
	ldrh r0, [r3]
	lsl r1, r0, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r5, #0x7F
	and r1, r5
	lsl r1, r1, #5
	ldr r4, _0800D0F4 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r0
	orr r2, r1
	strh r2, [r3]
	lsl r0, r2, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0x1F
	bgt _0800D100
	ldr r1, _0800D0F8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0800D0D0
	ldr r1, _0800D0FC @ =0x0201CFB0
	mov r0, sl
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800D0D0
	b _0800D222
_0800D0D0:
	cmp r3, #0x17
	ble _0800D0D6
	b _0800D222
_0800D0D6:
	add r0, r3, #7
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	mov r0, r8
	strh r2, [r0]
	b _0800D222
	.align 2, 0
_0800D0E8: .4byte 0x020185C0
_0800D0EC: .4byte 0x0000080C
_0800D0F0: .4byte gUnk_081A43E4
_0800D0F4: .4byte 0xFFFFF01F
_0800D0F8: .4byte 0x03000040
_0800D0FC: .4byte 0x0201CFB0
_0800D100:
	and r2, r4
	mov r1, r8
	strh r2, [r1]
	ldr r3, _0800D124 @ =0x0000080A
	add r3, r9
_0800D10A:
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
	b _0800D222
	.align 2, 0
_0800D124: .4byte 0x0000080A
_0800D128:
	ldr r0, _0800D168 @ =0x050003E0
	ldr r1, _0800D16C @ =0x08687B9C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _0800D170 @ =0x06016C80
	ldr r1, _0800D174 @ =0x08687FBC
	mov r2, #0x80
	lsl r2, r2, #3
	bl sub_080752B0
	ldr r2, _0800D178 @ =0x020185C0
	ldr r3, _0800D17C @ =0x0000080C
	add r1, r2, r3
	ldr r0, _0800D180 @ =0xFFFFF01F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r0, _0800D184 @ =0x0000080A
	add r2, r2, r0
_0800D150:
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
	b _0800D222
_0800D168: .4byte 0x050003E0
_0800D16C: .4byte gUnk_08687B9C
_0800D170: .4byte 0x06016C80
_0800D174: .4byte gUnk_08687FBC
_0800D178: .4byte 0x020185C0
_0800D17C: .4byte 0x0000080C
_0800D180: .4byte 0xFFFFF01F
_0800D184: .4byte 0x0000080A
_0800D188:
	ldr r1, _0800D1F4 @ =0x0000080C
	add r6, r3, r1
	ldrh r2, [r6]
	lsl r0, r2, #0x14
	lsr r5, r0, #0x19
	cmp r5, #0x5F
	bgt _0800D214
	ldr r0, _0800D1F8 @ =0x00300058
	ldr r1, _0800D1FC @ =0x000040C0
	ldr r2, _0800D200 @ =0x0000F364
	ldr r4, _0800D204 @ =0x081A43E4
	mov r3, #0x1F
	and r5, r3
	lsl r3, r5, #1
	add r3, r3, r4
	ldrh r3, [r3]
	lsl r3, r3, #0x10
	bl sub_08076714
	ldrh r1, [r6]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r4, #0x7F
	and r0, r4
	lsl r0, r0, #5
	ldr r3, _0800D208 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r6]
	ldr r1, _0800D20C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0800D1DE
	ldr r1, _0800D210 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800D222
_0800D1DE:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x57
	bgt _0800D222
	add r0, #3
	and r0, r4
	lsl r0, r0, #5
	and r2, r3
_0800D1EE:
	orr r2, r0
	strh r2, [r6]
	b _0800D222
_0800D1F4: .4byte 0x0000080C
_0800D1F8: .4byte 0x00300058
_0800D1FC: .4byte 0x000040C0
_0800D200: .4byte 0x0000F364
_0800D204: .4byte gUnk_081A43E4
_0800D208: .4byte 0xFFFFF01F
_0800D20C: .4byte 0x03000040
_0800D210: .4byte 0x0201CFB0
_0800D214:
	ldr r0, _0800D230 @ =0x0000080D
	add r1, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800D222:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800D230: .4byte 0x0000080D
	thumb_func_end sub_0800CE28

