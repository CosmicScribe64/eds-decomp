	thumb_func_start sub_0806B3B0
sub_0806B3B0: @ 0x0806B3B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2C
	ldr r0, _0806B5E4 @ =0x03000040
	ldr r1, _0806B5E8 @ =0x000003FF
	ldrh r0, [r0, #6]
	and r0, r1
	str r0, [sp, #0x28]
	ldr r4, _0806B5EC @ =0x0201E148
	add r0, r4, #0
	bl sub_0807B114
	ldr r2, _0806B5F0 @ =0x000010E8
	add r1, r4, r2
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0806B3DE
	b _0806B63C
_0806B3DE:
	mov r3, #2
	ldsh r0, [r4, r3]
	cmp r0, #4
	bne _0806B3EC
	ldrb r5, [r4, #0xD]
	cmp r5, #3
	beq _0806B3FA
_0806B3EC:
	cmp r0, #3
	beq _0806B3F2
	b _0806B63C
_0806B3F2:
	ldrb r4, [r4, #0xD]
	cmp r4, #4
	beq _0806B3FA
	b _0806B63C
_0806B3FA:
	ldr r6, _0806B5F4 @ =0x0201DB20
	ldr r0, _0806B5F8 @ =0x00001710
	add r1, r6, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r7, _0806B5FC @ =0x0600D000
	ldr r2, _0806B600 @ =0x01000200
	add r0, sp, #0x20
	add r1, r7, #0
	bl CpuFastSet
	ldr r4, _0806B604 @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _0806B608 @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #2
	cmp r0, #0
	blt _0806B45E
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r6, r4
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #2
	add r0, r3, #0
	bl sub_08068D1C
	ldr r5, _0806B60C @ =0x0000063A
	add r1, r6, r5
	ldrb r1, [r1]
	lsr r3, r1, #3
	mov r2, #0xC8
	lsl r2, r2, #3
	add r1, r6, r2
	str r1, [sp, #0]
	mov r1, #1
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl sub_08065108
_0806B45E:
	ldr r4, _0806B604 @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _0806B608 @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #1
	mov r4, #0xC8
	lsl r4, r4, #3
	add r4, r4, r6
	mov sl, r4
	cmp r0, #0
	blt _0806B4AA
	mov r5, #0xA5
	lsl r5, r5, #5
	add r0, r6, r5
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #1
	add r0, r3, #0
	bl sub_08068D1C
	ldr r2, _0806B60C @ =0x0000063A
	add r1, r6, r2
	ldrh r3, [r1]
	add r3, #0x10
	mov r1, #0xFF
	and r3, r1
	asr r3, r3, #3
	str r4, [sp, #0]
	mov r1, #2
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl sub_08065108
_0806B4AA:
	ldr r3, _0806B604 @ =0x0201F73C
	ldrb r4, [r3]
	lsl r1, r4, #1
	ldr r0, _0806B608 @ =0x0201E140
	add r5, r1, r0
	mov r3, #0
	ldsh r2, [r5, r3]
	add r2, #1
	mov r0, #0xA5
	lsl r0, r0, #5
	add r0, r0, r6
	mov r9, r0
	add r0, r4, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	ldr r0, _0806B610 @ =0x00001494
	add r0, r0, r6
	mov r8, r0
	add r1, r8
	ldrh r1, [r1]
	cmp r2, r1
	bge _0806B506
	ldrh r2, [r5]
	add r2, #1
	add r0, r4, #0
	add r1, r3, #0
	bl sub_08068D1C
	ldr r2, _0806B60C @ =0x0000063A
	add r1, r6, r2
	ldrh r3, [r1]
	add r3, #0x48
	mov r1, #0xFF
	and r3, r1
	asr r3, r3, #3
	mov r4, sl
	str r4, [sp, #0]
	mov r1, #4
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl sub_08065108
_0806B506:
	ldr r5, _0806B604 @ =0x0201F73C
	ldrb r4, [r5]
	lsl r1, r4, #1
	ldr r0, _0806B608 @ =0x0201E140
	add r5, r1, r0
	mov r3, #0
	ldsh r2, [r5, r3]
	add r2, #2
	mov r3, r9
	add r0, r4, r3
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	add r1, r8
	ldrh r1, [r1]
	cmp r2, r1
	bge _0806B556
	ldrh r2, [r5]
	add r2, #2
	add r0, r4, #0
	add r1, r3, #0
	bl sub_08068D1C
	ldr r4, _0806B60C @ =0x0000063A
	add r1, r6, r4
	ldrh r3, [r1]
	add r3, #0x58
	mov r1, #0xFF
	and r3, r1
	asr r3, r3, #3
	mov r5, sl
	str r5, [sp, #0]
	mov r1, #5
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl sub_08065108
_0806B556:
	ldr r7, _0806B614 @ =0x0600C000
	ldr r0, _0806B618 @ =0x0000063E
	add r4, r6, r0
	ldrh r3, [r4]
	add r3, #0x20
	mov r5, #0xFF
	and r3, r5
	asr r3, r3, #3
	mov r0, #0x1E
	str r0, [sp, #0]
	mov r0, #6
	str r0, [sp, #4]
	mov r1, sl
	str r1, [sp, #8]
	mov r0, #0
	add r1, r7, #0
	mov r2, #0
	bl sub_08079834
	ldr r2, _0806B604 @ =0x0201F73C
	ldrb r3, [r2]
	lsl r2, r3, #1
	mov r6, r9
	add r0, r3, r6
	ldrb r1, [r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r2, r0
	add r0, r8
	ldrh r0, [r0]
	cmp r0, #0
	beq _0806B61C
	ldr r6, _0806B608 @ =0x0201E140
	add r0, r2, r6
	ldrh r2, [r0]
	add r0, r3, #0
	bl sub_08068D1C
	ldrh r3, [r4]
	add r3, #0x20
	and r3, r5
	asr r3, r3, #3
	mov r1, sl
	str r1, [sp, #0]
	add r1, r7, #0
	mov r2, #0
	bl sub_0806518C
	mov r0, #0
	bl sub_080657F8
	ldrh r2, [r4]
	add r2, #0x38
	and r2, r5
	asr r2, r2, #3
	add r0, r7, #0
	mov r1, #0xB
	mov r3, sl
	bl sub_08065AB4
	ldrh r2, [r4]
	add r2, #0x38
	and r2, r5
	asr r2, r2, #3
	add r0, r7, #0
	mov r1, #0x11
	mov r3, #6
	bl sub_08065E6C
	b _0806B63C
_0806B5E4: .4byte 0x03000040
_0806B5E8: .4byte 0x000003FF
_0806B5EC: .4byte 0x0201E148
_0806B5F0: .4byte 0x000010E8
_0806B5F4: .4byte 0x0201DB20
_0806B5F8: .4byte 0x00001710
_0806B5FC: .4byte 0x0600D000
_0806B600: .4byte 0x01000200
_0806B604: .4byte 0x0201F73C
_0806B608: .4byte 0x0201E140
_0806B60C: .4byte 0x0000063A
_0806B610: .4byte 0x00001494
_0806B614: .4byte 0x0600C000
_0806B618: .4byte 0x0000063E
_0806B61C:
	ldr r6, _0806B688 @ =0x0201E140
	add r0, r2, r6
	ldrh r2, [r0]
	add r0, r3, #0
	bl sub_08068D1C
	ldrh r3, [r4]
	add r3, #0x20
	and r3, r5
	asr r3, r3, #3
	mov r1, sl
	str r1, [sp, #0]
	add r1, r7, #0
	mov r2, #0
	bl sub_08065384
_0806B63C:
	ldr r5, _0806B68C @ =0x0201DB20
	ldr r2, _0806B690 @ =0x0000062A
	add r7, r5, r2
	mov r3, #0
	ldsh r0, [r7, r3]
	mov r4, #0xC5
	lsl r4, r4, #3
	add r6, r5, r4
	ldrb r1, [r6]
	add r2, #0xB
	add r2, r2, r5
	mov r8, r2
	ldrb r2, [r2]
	ldr r4, _0806B694 @ =0x000018B0
	add r3, r5, r4
	ldr r4, _0806B698 @ =0x00001BB8
	add r4, r4, r5
	str r4, [sp, #0]
	bl sub_08066260
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #1
	bge _0806B66E
	b _0806BA12
_0806B66E:
	cmp r0, #2
	ble _0806B69C
	cmp r0, #4
	ble _0806B678
	b _0806BA12
_0806B678:
	ldrb r0, [r6]
	cmp r0, #1
	bne _0806B680
	b _0806B860
_0806B680:
	cmp r0, #2
	bne _0806B686
	b _0806B95C
_0806B686:
	b _0806B9BE
_0806B688: .4byte 0x0201E140
_0806B68C: .4byte 0x0201DB20
_0806B690: .4byte 0x0000062A
_0806B694: .4byte 0x000018B0
_0806B698: .4byte 0x00001BB8
_0806B69C:
	ldrb r0, [r6]
	cmp r0, #1
	beq _0806B6A8
	cmp r0, #2
	beq _0806B758
	b _0806B7F2
_0806B6A8:
	ldr r1, _0806B730 @ =0x0400001C
	mov r2, #0xC6
	lsl r2, r2, #3
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806B734 @ =0x080875D2
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	ldr r2, _0806B738 @ =0x0400001E
	ldr r6, _0806B73C @ =0x00000632
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806B740 @ =0x04000014
	mov r2, #0xC7
	lsl r2, r2, #3
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #5
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	ldr r2, _0806B744 @ =0x04000016
	add r6, #8
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806B748 @ =0x04000010
	ldr r2, _0806B74C @ =0x0000063C
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #6
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	ldr r2, _0806B750 @ =0x04000012
	ldr r4, _0806B754 @ =0x0000063E
	add r1, r5, r4
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	b _0806BA12
	.align 2, 0
_0806B730: .4byte 0x0400001C
_0806B734: .4byte gUnk_080875D2
_0806B738: .4byte 0x0400001E
_0806B73C: .4byte 0x00000632
_0806B740: .4byte 0x04000014
_0806B744: .4byte 0x04000016
_0806B748: .4byte 0x04000010
_0806B74C: .4byte 0x0000063C
_0806B750: .4byte 0x04000012
_0806B754: .4byte 0x0000063E
_0806B758:
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806B838 @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	ldr r2, _0806B83C @ =0x00000632
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #5
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	ldr r2, _0806B840 @ =0x0000063A
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #6
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	ldr r2, _0806B844 @ =0x0000063E
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	ldr r4, _0806B848 @ =0x00001BB5
	add r2, r5, r4
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806B7D2
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _0806B84C @ =0x00001BB4
	add r1, r5, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0806B7D2:
	ldr r3, _0806B850 @ =0x00001BB7
	add r2, r5, r3
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806B7EC
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r4, _0806B854 @ =0x00001BB6
	add r1, r5, r4
	ldrb r5, [r1]
	orr r0, r5
	strb r0, [r1]
_0806B7EC:
	mov r0, #0
	mov r6, r8
	strb r0, [r6]
_0806B7F2:
	ldr r2, _0806B858 @ =0x0400001C
	ldr r1, _0806B85C @ =0x0201DB20
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r4, _0806B83C @ =0x00000632
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #0xA
	mov r5, #0xC7
	lsl r5, r5, #3
	add r0, r1, r5
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r6, _0806B840 @ =0x0000063A
	add r0, r1, r6
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #6
	add r3, #0xC
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	add r4, #0xC
	add r1, r1, r4
	ldrh r0, [r1]
	strh r0, [r2]
	b _0806BA12
	.align 2, 0
_0806B838: .4byte gUnk_080875D2
_0806B83C: .4byte 0x00000632
_0806B840: .4byte 0x0000063A
_0806B844: .4byte 0x0000063E
_0806B848: .4byte 0x00001BB5
_0806B84C: .4byte 0x00001BB4
_0806B850: .4byte 0x00001BB7
_0806B854: .4byte 0x00001BB6
_0806B858: .4byte 0x0400001C
_0806B85C: .4byte 0x0201DB20
_0806B860:
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806B8EC @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	ldr r2, _0806B8F0 @ =0x0400001C
	mov r3, #0xC6
	lsl r3, r3, #3
	add r1, r5, r3
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806B8F4 @ =0x0400001E
	ldr r6, _0806B8F8 @ =0x00000632
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #7
	mov r2, #0
	ldsh r1, [r7, r2]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl sub_0807B4D0
	asr r0, r0, #8
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	mov r3, #0
	ldsh r0, [r7, r3]
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	lsr r0, r0, #3
	lsl r3, r0, #0x18
	lsr r4, r3, #0x18
	ldr r1, _0806B8FC @ =0x04000050
	ldr r6, _0806B900 @ =0x00003F43
	add r0, r6, #0
	strh r0, [r1]
	mov r1, #0
	ldsh r0, [r7, r1]
	cmp r0, #3
	bgt _0806B918
	ldr r0, _0806B904 @ =0x04000014
	strh r2, [r0]
	ldr r1, _0806B908 @ =0x04000016
	ldr r4, _0806B90C @ =0x0000063A
	add r0, r5, r4
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _0806B910 @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _0806B914 @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	lsr r0, r3, #0x19
	bl sub_0807B4A8
	b _0806BA12
	.align 2, 0
_0806B8EC: .4byte gUnk_080875D2
_0806B8F0: .4byte 0x0400001C
_0806B8F4: .4byte 0x0400001E
_0806B8F8: .4byte 0x00000632
_0806B8FC: .4byte 0x04000050
_0806B900: .4byte 0x00003F43
_0806B904: .4byte 0x04000014
_0806B908: .4byte 0x04000016
_0806B90C: .4byte 0x0000063A
_0806B910: .4byte 0x04000010
_0806B914: .4byte 0x0000063E
_0806B918:
	ldr r0, _0806B944 @ =0x04000014
	ldr r1, _0806B948 @ =0x0000FFC0
	add r2, r2, r1
	strh r2, [r0]
	ldr r1, _0806B94C @ =0x04000016
	ldr r3, _0806B950 @ =0x0000063A
	add r0, r5, r3
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _0806B954 @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _0806B958 @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x20
	sub r0, r0, r4
	asr r0, r0, #1
	bl sub_0807B4A8
	b _0806BA12
_0806B944: .4byte 0x04000014
_0806B948: .4byte 0x0000FFC0
_0806B94C: .4byte 0x04000016
_0806B950: .4byte 0x0000063A
_0806B954: .4byte 0x04000010
_0806B958: .4byte 0x0000063E
_0806B95C:
	add r4, r5, #0
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r2, _0806BA3C @ =0x080875D2
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r2
	ldrh r1, [r1]
	bl sub_0807B4D0
	mov r5, #0xC6
	lsl r5, r5, #3
	add r1, r4, r5
	asr r0, r0, #8
	ldrh r6, [r1]
	add r0, r6, r0
	strh r0, [r1]
	ldr r0, _0806BA40 @ =0x00001BB5
	add r2, r4, r0
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806B99E
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r2, _0806BA44 @ =0x00001BB4
	add r1, r4, r2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_0806B99E:
	ldr r5, _0806BA48 @ =0x00001BB7
	add r2, r4, r5
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806B9B8
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _0806BA4C @ =0x00001BB6
	add r1, r4, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0806B9B8:
	mov r0, #0
	mov r3, r8
	strb r0, [r3]
_0806B9BE:
	ldr r2, _0806BA50 @ =0x0400001C
	ldr r1, _0806BA54 @ =0x0201DB20
	mov r4, #0xC6
	lsl r4, r4, #3
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r5, _0806BA58 @ =0x00000632
	add r0, r1, r5
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #0xA
	mov r6, #0xC7
	lsl r6, r6, #3
	add r0, r1, r6
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r3, _0806BA5C @ =0x0000063A
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #6
	add r4, #0xC
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	add r5, #0xC
	add r1, r1, r5
	ldrh r0, [r1]
	strh r0, [r2]
	ldr r1, _0806BA60 @ =0x04000050
	ldr r6, _0806BA64 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #2
	mov r2, #0x80
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
_0806BA12:
	ldr r3, _0806BA54 @ =0x0201DB20
	ldr r4, _0806BA68 @ =0x0000061E
	add r0, r3, r4
	ldrb r0, [r0]
	add r6, r3, #0
	cmp r0, #0
	beq _0806BA24
	bl _0806C2CE @ far jump
_0806BA24:
	ldr r5, _0806BA6C @ =0x00001C48
	add r4, r3, r5
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, #0
	beq _0806BA70
	cmp r0, #1
	bne _0806BA38
	b _0806BF0C
_0806BA38:
	bl _0806C2CE @ far jump
_0806BA3C: .4byte gUnk_080875D2
_0806BA40: .4byte 0x00001BB5
_0806BA44: .4byte 0x00001BB4
_0806BA48: .4byte 0x00001BB7
_0806BA4C: .4byte 0x00001BB6
_0806BA50: .4byte 0x0400001C
_0806BA54: .4byte 0x0201DB20
_0806BA58: .4byte 0x00000632
_0806BA5C: .4byte 0x0000063A
_0806BA60: .4byte 0x04000050
_0806BA64: .4byte 0x00003FC8
_0806BA68: .4byte 0x0000061E
_0806BA6C: .4byte 0x00001C48
_0806BA70:
	mov r2, #0xC5
	lsl r2, r2, #3
	add r0, r3, r2
	ldrb r0, [r0]
	cmp r0, #1
	bne _0806BA7E
	b _0806BECA
_0806BA7E:
	ldr r5, [sp, #0x28]
	cmp r5, #0x40
	beq _0806BABA
	cmp r5, #0x40
	bhi _0806BA8E
	cmp r5, #2
	beq _0806BA96
	b _0806BACA
_0806BA8E:
	ldr r0, [sp, #0x28]
	cmp r0, #0x80
	beq _0806BAC2
	b _0806BACA
_0806BA96:
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r3, r2
	mov r0, #0
	mov r2, #0
	bl sub_080787F4
	mov r0, #0x1F
	neg r0, r0
	ldrb r3, [r4]
	and r0, r3
	strb r0, [r4]
	mov r0, #2
	bl sub_08077AEC
	b _0806BECA
_0806BABA:
	add r0, sp, #0x24
	bl sub_080679E0
	b _0806BCD8
_0806BAC2:
	add r0, sp, #0x24
	bl sub_08067DA4
	b _0806BCD8
_0806BACA:
	ldr r4, _0806BB00 @ =0x00001C1C
	add r0, r6, r4
	ldrb r1, [r0]
	lsl r2, r1, #1
	mov r5, #0xA5
	lsl r5, r5, #5
	add r0, r6, r5
	add r1, r1, r0
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r4, _0806BB04 @ =0x00001494
	add r1, r6, r4
	add r4, r0, r1
	ldrh r5, [r4]
	cmp r5, #5
	bhi _0806BAF2
	b _0806BECA
_0806BAF2:
	ldr r0, [sp, #0x28]
	cmp r0, #0x10
	beq _0806BB08
	cmp r0, #0x20
	bne _0806BAFE
	b _0806BCF4
_0806BAFE:
	b _0806BECA
_0806BB00: .4byte 0x00001C1C
_0806BB04: .4byte 0x00001494
_0806BB08:
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r6, r1
	add r1, r2, r0
	ldrh r2, [r1]
	add r2, #5
	ldrh r0, [r4]
	sub r0, #1
	cmp r2, r0
	ble _0806BB22
	mov r0, #0
	strh r0, [r1]
	b _0806BB24
_0806BB22:
	strh r2, [r1]
_0806BB24:
	ldr r6, _0806BC68 @ =0x0201DB20
	ldr r2, _0806BC6C @ =0x000018AC
	add r1, r6, r2
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r4, #0xC5
	lsl r4, r4, #3
	add r3, r6, r4
	mov r0, #0
	mov r1, #6
	mov r2, #1
	bl sub_0807B100
	ldr r5, _0806BC70 @ =0x00000634
	add r4, r6, r5
	mov r0, #1
	mov r9, r0
	mov r0, r9
	ldrb r1, [r4]
	eor r0, r1
	strb r0, [r4]
	ldr r2, _0806BC74 @ =0x00001C1C
	add r2, r2, r6
	mov r8, r2
	ldrb r0, [r2]
	mov r3, #0xA5
	lsl r3, r3, #5
	add r5, r6, r3
	add r1, r0, r5
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r3, #0xC4
	lsl r3, r3, #3
	add r7, r6, r3
	add r2, r2, r7
	ldrh r2, [r2]
	bl sub_08068D1C
	ldrb r2, [r4]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _0806BC78 @ =0x06008000
	add r1, r1, r3
	bl sub_0807AFFC
	mov r0, #0xC6
	lsl r0, r0, #3
	add r2, r6, r0
	mov r1, #0xFF
	add r0, r1, #0
	ldrh r2, [r2]
	and r0, r2
	lsr r0, r0, #3
	add r0, #0x1D
	ldr r3, _0806BC7C @ =0x00000632
	add r2, r6, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldrb r2, [r4]
	mov r3, #1
	bl sub_08068E20
	ldr r4, _0806BC80 @ =0x00000635
	add r1, r6, r4
	mov r0, #3
	strb r0, [r1]
	ldr r0, _0806BC84 @ =0x00001710
	add r1, r6, r0
	mov r0, r9
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	ldrb r1, [r3]
	lsl r2, r1, #1
	add r1, r1, r5
	ldrb r4, [r1]
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r5, _0806BC88 @ =0x00001494
	add r1, r6, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r2, r7
	ldrh r1, [r2]
	ldr r3, _0806BC8C @ =0x00001BB0
	add r2, r6, r3
	bl sub_08065F34
	ldr r4, _0806BC90 @ =0x00001BB7
	add r1, r6, r4
	ldrb r0, [r1]
	cmp r0, #0
	beq _0806BC00
	mov r0, #2
	strb r0, [r1]
	ldr r5, _0806BC94 @ =0x00001BB6
	add r1, r6, r5
	mov r0, r9
	ldrb r6, [r1]
	orr r0, r6
	strb r0, [r1]
_0806BC00:
	add r0, sp, #0x24
	mov r2, r8
	ldrb r2, [r2]
	lsl r1, r2, #1
	add r1, r1, r7
	ldrh r1, [r1]
	sub r1, #2
	strh r1, [r0]
	mov r5, #0
	add r7, r0, #0
_0806BC14:
	ldrh r3, [r7]
	lsl r2, r3, #0x10
	cmp r2, #0
	blt _0806BCA0
	ldr r6, _0806BC68 @ =0x0201DB20
	ldr r4, _0806BC74 @ =0x00001C1C
	add r0, r6, r4
	ldrb r4, [r0]
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r6, r1
	add r0, r4, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	add r0, r0, r4
	lsl r0, r0, #1
	sub r1, #0xC
	add r1, r1, r6
	mov r8, r1
	add r0, r8
	lsr r1, r2, #0x10
	ldrh r0, [r0]
	cmp r1, r0
	bcs _0806BCA0
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl sub_08068D1C
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _0806BC98 @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _0806BC9C @ =0x00001BB8
	add r3, r6, r4
	bl sub_0806664C
	b _0806BCAE
	.align 2, 0
_0806BC68: .4byte 0x0201DB20
_0806BC6C: .4byte 0x000018AC
_0806BC70: .4byte 0x00000634
_0806BC74: .4byte 0x00001C1C
_0806BC78: .4byte 0x06008000
_0806BC7C: .4byte 0x00000632
_0806BC80: .4byte 0x00000635
_0806BC84: .4byte 0x00001710
_0806BC88: .4byte 0x00001494
_0806BC8C: .4byte 0x00001BB0
_0806BC90: .4byte 0x00001BB7
_0806BC94: .4byte 0x00001BB6
_0806BC98: .4byte 0x000018B0
_0806BC9C: .4byte 0x00001BB8
_0806BCA0:
	ldr r0, _0806BCE0 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _0806BCE4 @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_0806BCAE:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _0806BC14
	ldr r1, _0806BCE0 @ =0x0201DB20
	ldr r0, _0806BCE8 @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _0806BCEC @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _0806BCF0 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
_0806BCD8:
	mov r0, #0
	bl sub_08077AEC
	b _0806BECA
_0806BCE0: .4byte 0x0201DB20
_0806BCE4: .4byte 0x00001BC4
_0806BCE8: .4byte 0x00001C14
_0806BCEC: .4byte 0x00001BB8
_0806BCF0: .4byte 0x00001C58
_0806BCF4:
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r6, r1
	add r1, r2, r0
	ldrh r0, [r1]
	cmp r0, #4
	bhi _0806BD06
	sub r0, r5, #1
	b _0806BD08
_0806BD06:
	sub r0, #5
_0806BD08:
	strh r0, [r1]
	ldr r7, _0806BE54 @ =0x0201DB20
	ldr r2, _0806BE58 @ =0x000018AC
	add r1, r7, r2
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r2, #1
	neg r2, r2
	mov r4, #0xC5
	lsl r4, r4, #3
	add r3, r7, r4
	mov r0, #6
	mov r1, #0
	bl sub_0807B100
	ldr r6, _0806BE5C @ =0x00000634
	add r5, r7, r6
	mov r0, #1
	mov sl, r0
	mov r0, sl
	ldrb r1, [r5]
	eor r0, r1
	strb r0, [r5]
	ldr r2, _0806BE60 @ =0x00001C1C
	add r2, r2, r7
	mov r9, r2
	ldrb r0, [r2]
	mov r3, #0xA5
	lsl r3, r3, #5
	add r6, r7, r3
	add r1, r0, r6
	ldrb r1, [r1]
	lsl r2, r0, #1
	sub r4, #8
	add r4, r4, r7
	mov r8, r4
	add r2, r8
	ldrh r2, [r2]
	bl sub_08068D1C
	ldrb r2, [r5]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _0806BE64 @ =0x06008000
	add r1, r1, r3
	bl sub_0807AFFC
	mov r0, #0xC6
	lsl r0, r0, #3
	add r4, r7, r0
	mov r1, #0xFF
	add r0, r1, #0
	ldrh r2, [r4]
	and r0, r2
	lsr r0, r0, #3
	add r0, #9
	ldr r3, _0806BE68 @ =0x00000632
	add r2, r7, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldrb r2, [r5]
	mov r3, #1
	bl sub_08068E20
	ldrh r0, [r4]
	sub r0, #0x50
	strh r0, [r4]
	ldr r4, _0806BE6C @ =0x00000635
	add r1, r7, r4
	mov r0, #4
	strb r0, [r1]
	ldr r5, _0806BE70 @ =0x00001710
	add r1, r7, r5
	mov r0, sl
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r9
	ldrb r1, [r3]
	lsl r2, r1, #1
	add r1, r1, r6
	ldrb r4, [r1]
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r5, _0806BE74 @ =0x00001494
	add r1, r7, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r8
	ldrh r1, [r2]
	ldr r6, _0806BE78 @ =0x00001BB0
	add r2, r7, r6
	bl sub_08065F34
	ldr r0, _0806BE7C @ =0x00001BB5
	add r1, r7, r0
	ldrb r0, [r1]
	cmp r0, #0
	beq _0806BDEE
	mov r0, #2
	strb r0, [r1]
	ldr r2, _0806BE80 @ =0x00001BB4
	add r1, r7, r2
	mov r0, sl
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_0806BDEE:
	add r0, sp, #0x24
	mov r4, r9
	ldrb r4, [r4]
	lsl r1, r4, #1
	add r1, r8
	ldrh r1, [r1]
	sub r1, #2
	strh r1, [r0]
	mov r5, #0
	add r7, r0, #0
_0806BE02:
	ldrh r6, [r7]
	lsl r2, r6, #0x10
	cmp r2, #0
	blt _0806BE8C
	ldr r6, _0806BE54 @ =0x0201DB20
	ldr r1, _0806BE60 @ =0x00001C1C
	add r0, r6, r1
	ldrb r4, [r0]
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r6, r3
	add r0, r4, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	add r0, r0, r4
	lsl r0, r0, #1
	ldr r1, _0806BE74 @ =0x00001494
	add r1, r1, r6
	mov r8, r1
	add r0, r8
	lsr r1, r2, #0x10
	ldrh r0, [r0]
	cmp r1, r0
	bcs _0806BE8C
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl sub_08068D1C
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _0806BE84 @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _0806BE88 @ =0x00001BB8
	add r3, r6, r4
	bl sub_0806664C
	b _0806BE9A
_0806BE54: .4byte 0x0201DB20
_0806BE58: .4byte 0x000018AC
_0806BE5C: .4byte 0x00000634
_0806BE60: .4byte 0x00001C1C
_0806BE64: .4byte 0x06008000
_0806BE68: .4byte 0x00000632
_0806BE6C: .4byte 0x00000635
_0806BE70: .4byte 0x00001710
_0806BE74: .4byte 0x00001494
_0806BE78: .4byte 0x00001BB0
_0806BE7C: .4byte 0x00001BB5
_0806BE80: .4byte 0x00001BB4
_0806BE84: .4byte 0x000018B0
_0806BE88: .4byte 0x00001BB8
_0806BE8C:
	ldr r0, _0806BEE8 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _0806BEEC @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_0806BE9A:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _0806BE02
	ldr r1, _0806BEE8 @ =0x0201DB20
	ldr r0, _0806BEF0 @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _0806BEF4 @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _0806BEF8 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
	mov r0, #0
	bl sub_08077AEC
_0806BECA:
	ldr r5, [sp, #0x28]
	cmp r5, #1
	bne _0806BF00
	ldr r0, _0806BEE8 @ =0x0201DB20
	ldr r6, _0806BEFC @ =0x00001C3D
	add r0, r0, r6
	mov r1, #8
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #1
	orr r1, r2
	strb r1, [r0]
	b _0806C266
	.align 2, 0
_0806BEE8: .4byte 0x0201DB20
_0806BEEC: .4byte 0x00001BC4
_0806BEF0: .4byte 0x00001C14
_0806BEF4: .4byte 0x00001BB8
_0806BEF8: .4byte 0x00001C58
_0806BEFC: .4byte 0x00001C3D
_0806BF00:
	ldr r0, _0806BF08 @ =0x0201F73C
	bl sub_0806ADBC
	b _0806C2CE
_0806BF08: .4byte 0x0201F73C
_0806BF0C:
	mov r4, #0xE1
	lsl r4, r4, #5
	add r0, r3, r4
	ldrb r0, [r0]
	cmp r0, #0
	beq _0806BF1A
	b _0806C2CE
_0806BF1A:
	ldr r5, _0806BF40 @ =0x00001866
	add r0, r3, r5
	mov r1, #0
	ldsb r1, [r0, r1]
	mov r0, #1
	neg r0, r0
	cmp r1, r0
	beq _0806BF2C
	b _0806C2CE
_0806BF2C:
	ldr r0, [sp, #0x28]
	cmp r0, #2
	bne _0806BF34
	b _0806C278
_0806BF34:
	cmp r0, #2
	bhi _0806BF44
	cmp r0, #1
	bne _0806BF3E
	b _0806C11C
_0806BF3E:
	b _0806C298
_0806BF40: .4byte 0x00001866
_0806BF44:
	ldr r1, [sp, #0x28]
	cmp r1, #0x10
	beq _0806BF50
	cmp r1, #0x20
	beq _0806C024
	b _0806C298
_0806BF50:
	ldr r2, _0806BFAC @ =0x00001C5A
	add r1, r3, r2
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0806BF60
	b _0806C2CE
_0806BF60:
	ldr r4, _0806BFB0 @ =0x00001C3C
	add r5, r3, r4
	ldr r2, [r5]
	lsl r1, r2, #0xE
	lsr r1, r1, #0x1D
	add r1, #1
	mov r7, #7
	add r0, r1, #0
	and r0, r7
	lsl r0, r0, #0xF
	ldr r4, _0806BFB4 @ =0xFFFC7FFF
	mov r8, r4
	and r4, r2
	orr r4, r0
	str r4, [r5]
	mov r0, #7
	and r1, r0
	cmp r1, #1
	bne _0806BF98
	lsl r0, r4, #0xE
	lsr r0, r0, #0x1D
	add r0, #1
	and r0, r7
	lsl r0, r0, #0xF
	mov r1, r8
	and r1, r4
	orr r1, r0
	str r1, [r5]
_0806BF98:
	ldr r1, _0806BFB8 @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	add r0, #1
	cmp r0, #2
	beq _0806BFBC
	cmp r0, #3
	beq _0806BFCA
	b _0806BFEC
	.align 2, 0
_0806BFAC: .4byte 0x00001C5A
_0806BFB0: .4byte 0x00001C3C
_0806BFB4: .4byte 0xFFFC7FFF
_0806BFB8: .4byte 0x00001C1C
_0806BFBC:
	ldr r2, [r5]
	mov r0, #0xE0
	lsl r0, r0, #0xA
	and r0, r2
	mov r1, #0xC0
	lsl r1, r1, #9
	b _0806BFD6
_0806BFCA:
	ldr r2, [r5]
	mov r0, #0xE0
	lsl r0, r0, #0xA
	and r0, r2
	mov r1, #0x80
	lsl r1, r1, #9
_0806BFD6:
	cmp r0, r1
	bne _0806BFEC
	lsl r0, r2, #0xE
	lsr r0, r0, #0x1D
	add r0, #1
	and r0, r7
	lsl r0, r0, #0xF
	mov r1, r8
	and r1, r2
	orr r1, r0
	str r1, [r5]
_0806BFEC:
	ldr r2, _0806C018 @ =0x00001C3C
	add r4, r6, r2
	ldr r2, [r4]
	mov r1, #0xE0
	lsl r1, r1, #0xA
	add r0, r2, #0
	and r0, r1
	cmp r0, r1
	bne _0806C004
	ldr r0, _0806C01C @ =0xFFFC7FFF
	and r2, r0
	str r2, [r4]
_0806C004:
	ldr r3, _0806C020 @ =0x00001C3D
	add r0, r6, r3
	mov r1, #8
	neg r1, r1
	ldrb r4, [r0]
	and r1, r4
	mov r2, #3
	orr r1, r2
	strb r1, [r0]
	b _0806C10E
_0806C018: .4byte 0x00001C3C
_0806C01C: .4byte 0xFFFC7FFF
_0806C020: .4byte 0x00001C3D
_0806C024:
	ldr r5, _0806C054 @ =0x00001C5A
	add r1, r3, r5
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0806C034
	b _0806C2CE
_0806C034:
	ldr r0, _0806C058 @ =0x00001C3C
	add r5, r3, r0
	ldr r4, [r5]
	mov r7, #0xE0
	lsl r7, r7, #0xA
	add r0, r4, #0
	and r0, r7
	cmp r0, #0
	bne _0806C060
	ldr r0, _0806C05C @ =0xFFFC7FFF
	and r0, r4
	mov r1, #0xC0
	lsl r1, r1, #0xA
	orr r0, r1
	str r0, [r5]
	b _0806C0FC
_0806C054: .4byte 0x00001C5A
_0806C058: .4byte 0x00001C3C
_0806C05C: .4byte 0xFFFC7FFF
_0806C060:
	lsl r0, r4, #0xE
	lsr r0, r0, #0x1D
	sub r0, #1
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	mov r2, #7
	mov r8, r2
	and r1, r2
	lsl r1, r1, #0xF
	ldr r2, _0806C0B0 @ =0xFFFC7FFF
	mov ip, r2
	and r2, r4
	orr r2, r1
	str r2, [r5]
	mov r1, #7
	and r0, r1
	cmp r0, #1
	bne _0806C09C
	lsl r0, r2, #0xE
	lsr r0, r0, #0x1D
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r4, r8
	and r0, r4
	lsl r0, r0, #0xF
	mov r1, ip
	and r1, r2
	orr r1, r0
	str r1, [r5]
_0806C09C:
	ldr r1, _0806C0B4 @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	add r0, #1
	cmp r0, #2
	beq _0806C0B8
	cmp r0, #3
	beq _0806C0D6
	b _0806C0FC
	.align 2, 0
_0806C0B0: .4byte 0xFFFC7FFF
_0806C0B4: .4byte 0x00001C1C
_0806C0B8:
	ldr r2, [r5]
	add r0, r2, #0
	and r0, r7
	mov r1, #0xC0
	lsl r1, r1, #9
	cmp r0, r1
	bne _0806C0FC
	lsl r0, r2, #0xE
	lsr r0, r0, #0x1D
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r3, r8
	and r0, r3
	b _0806C0F2
_0806C0D6:
	ldr r2, [r5]
	add r0, r2, #0
	and r0, r7
	mov r1, #0x80
	lsl r1, r1, #9
	cmp r0, r1
	bne _0806C0FC
	lsl r0, r2, #0xE
	lsr r0, r0, #0x1D
	sub r0, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r4, r8
	and r0, r4
_0806C0F2:
	lsl r0, r0, #0xF
	mov r1, ip
	and r1, r2
	orr r1, r0
	str r1, [r5]
_0806C0FC:
	ldr r0, _0806C118 @ =0x00001C3D
	add r2, r6, r0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
_0806C10E:
	mov r0, #0
	bl sub_08077AEC
	b _0806C2CE
	.align 2, 0
_0806C118: .4byte 0x00001C3D
_0806C11C:
	ldr r2, _0806C138 @ =0x00001C3C
	add r0, r3, r2
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1D
	cmp r0, #6
	bls _0806C12C
	b _0806C2CE
_0806C12C:
	lsl r0, r0, #2
	ldr r1, _0806C13C @ =0x0806C140
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0806C138: .4byte 0x00001C3C
_0806C13C: .4byte 0x0806C140
_0806C140:
	.4byte _0806C15C
	.4byte _0806C2CE
	.4byte _0806C188
	.4byte _0806C1BC
	.4byte _0806C1F0
	.4byte _0806C21C
	.4byte _0806C248
_0806C15C:
	ldr r3, _0806C184 @ =0x00001C48
	add r2, r6, r3
	mov r0, #0x1F
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	mov r1, #4
	orr r0, r1
	strb r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r5, #0xC3
	lsl r5, r5, #3
	add r3, r6, r5
	mov r0, #0
	mov r2, #0
	bl sub_080787F4
	b _0806C266
	.align 2, 0
_0806C184: .4byte 0x00001C48
_0806C188:
	bl sub_08068434
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0806C194
	b _0806C2CE
_0806C194:
	ldr r2, _0806C1B4 @ =0x0201DB20
	ldr r6, _0806C1B8 @ =0x00001C5A
	add r2, r2, r6
	ldrb r3, [r2]
	lsl r1, r3, #0x1B
	lsr r1, r1, #0x1D
	add r1, #1
	mov r0, #7
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0x1D
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _0806C266
_0806C1B4: .4byte 0x0201DB20
_0806C1B8: .4byte 0x00001C5A
_0806C1BC:
	bl sub_08068434
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0806C1C8
	b _0806C2CE
_0806C1C8:
	ldr r2, _0806C1E8 @ =0x0201DB20
	ldr r0, _0806C1EC @ =0x00001C5A
	add r2, r2, r0
	ldrb r3, [r2]
	lsl r1, r3, #0x1B
	lsr r1, r1, #0x1D
	add r1, #1
	mov r0, #7
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0x1D
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _0806C266
_0806C1E8: .4byte 0x0201DB20
_0806C1EC: .4byte 0x00001C5A
_0806C1F0:
	ldr r1, _0806C218 @ =0x00001C48
	add r2, r6, r1
	mov r0, #0x1F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	mov r1, #2
	orr r0, r1
	strb r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r4, #0xC3
	lsl r4, r4, #3
	add r3, r6, r4
	mov r0, #0
	mov r2, #0
	bl sub_080787F4
	b _0806C266
	.align 2, 0
_0806C218: .4byte 0x00001C48
_0806C21C:
	ldr r5, _0806C244 @ =0x00001C48
	add r2, r6, r5
	mov r0, #0x1F
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #6
	orr r0, r1
	strb r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r6, r2
	mov r0, #0
	mov r2, #0
	bl sub_080787F4
	b _0806C266
	.align 2, 0
_0806C244: .4byte 0x00001C48
_0806C248:
	mov r1, #0xC0
	lsl r1, r1, #1
	ldr r4, _0806C270 @ =0x0201E138
	mov r0, #0
	mov r2, #0
	add r3, r4, #0
	bl sub_080787F4
	ldr r3, _0806C274 @ =0x00001630
	add r4, r4, r3
	mov r0, #0x1F
	neg r0, r0
	ldrb r5, [r4]
	and r0, r5
	strb r0, [r4]
_0806C266:
	mov r0, #1
	bl sub_08077AEC
	b _0806C2CE
	.align 2, 0
_0806C270: .4byte 0x0201E138
_0806C274: .4byte 0x00001630
_0806C278:
	ldr r6, _0806C294 @ =0x00001C5A
	add r1, r3, r6
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #8
	bne _0806C2CE
	bl sub_0806B2F8
	mov r0, #2
	bl sub_08077AEC
	b _0806C2CE
	.align 2, 0
_0806C294: .4byte 0x00001C5A
_0806C298:
	ldr r4, _0806C2B8 @ =0x0201F73C
	add r0, r4, #0
	bl sub_0806ADBC
	ldr r0, _0806C2BC @ =0xFFFFEA0C
	add r4, r4, r0
	ldrb r4, [r4]
	cmp r4, #1
	beq _0806C2CE
	ldr r1, [sp, #0x28]
	cmp r1, #0x40
	beq _0806C2C0
	cmp r1, #0x80
	beq _0806C2C8
	b _0806C2CE
	.align 2, 0
_0806C2B8: .4byte 0x0201F73C
_0806C2BC: .4byte 0xFFFFEA0C
_0806C2C0:
	add r0, sp, #0x24
	bl sub_080679E0
	b _0806C2CE
_0806C2C8:
	add r0, sp, #0x24
	bl sub_08067DA4
_0806C2CE:
	bl sub_0806B190
	ldr r0, _0806C414 @ =0x081A6524
	mov r3, #1
	neg r3, r3
	str r3, [sp, #0]
	mov r1, #3
	str r1, [sp, #4]
	mov r1, #2
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	str r1, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r4, _0806C418 @ =0x0201DB20
	str r4, [sp, #0x1C]
	mov r1, #5
	mov r2, #0xC
	bl sub_08077EF4
	ldr r2, _0806C41C @ =0x000017DA
	add r1, r4, r2
	mov r0, #1
	strb r0, [r1]
	ldr r3, _0806C420 @ =0x00001C1C
	add r0, r4, r3
	ldrb r2, [r0]
	lsl r3, r2, #1
	mov r5, #0xC4
	lsl r5, r5, #3
	add r0, r4, r5
	add r0, r3, r0
	ldrh r0, [r0]
	mov r6, #0xA5
	lsl r6, r6, #5
	add r1, r4, r6
	add r2, r2, r1
	ldrb r5, [r2]
	lsl r1, r5, #1
	add r1, r1, r5
	lsl r1, r1, #1
	add r3, r3, r1
	sub r6, #0xC
	add r1, r4, r6
	add r3, r3, r1
	ldrh r1, [r3]
	ldr r3, _0806C424 @ =0x00001BB0
	add r2, r4, r3
	bl sub_08065F78
	ldr r5, _0806C428 @ =0x00001BBC
	add r0, r4, r5
	add r1, r4, #0
	bl sub_080665D4
	mov r5, #1
	ldr r6, _0806C42C @ =0x000018B0
	add r4, r4, r6
_0806C344:
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #3
	add r0, r0, r4
	bl sub_0807B5A0
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #6
	bls _0806C344
	ldr r0, _0806C430 @ =0x0201F740
	mov r8, r0
	bl sub_0806699C
	mov r6, r8
	sub r6, #4
	mov r1, r8
	sub r1, #3
	ldr r5, _0806C434 @ =0xFFFFFAF8
	add r5, r8
	add r0, r6, #0
	add r2, r5, #0
	bl sub_0806704C
	mov r4, r8
	add r4, #0x1C
	add r0, r4, #0
	bl sub_08067908
	add r0, r4, #0
	bl sub_08067660
	ldr r0, _0806C438 @ =0x081A6EA4
	mov r3, #1
	neg r3, r3
	str r3, [sp, #0]
	mov r7, #0
	str r7, [sp, #4]
	str r7, [sp, #8]
	mov r1, #1
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	ldr r4, _0806C43C @ =0xFFFFE3E0
	add r4, r8
	str r4, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	bl sub_08077EF4
	ldrb r0, [r6]
	bl sub_08068180
	add r0, r5, #0
	bl sub_0807871C
	str r7, [sp, #0]
	str r7, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	str r7, [sp, #0xC]
	str r7, [sp, #0x10]
	str r4, [sp, #0x14]
	add r0, r5, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_08078534
	bl sub_08068B90
	bl sub_08068C48
	add r0, r4, #0
	bl sub_0807A298
	add r0, r4, #0
	bl sub_0807A2EC
	ldr r0, _0806C440 @ =0xFFFFE9F8
	add r0, r8
	bl sub_0807883C
	ldr r0, _0806C444 @ =0xFFFFE9FE
	add r0, r8
	ldrb r1, [r0]
	cmp r1, #2
	beq _0806C450
	cmp r1, #3
	bne _0806C454
	strb r7, [r0]
	ldr r1, _0806C448 @ =0x04000050
	ldr r2, _0806C44C @ =0x00003FC8
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r3, #0x80
	lsl r3, r3, #5
	add r0, r3, #0
	strh r0, [r1]
	b _0806C4D2
	.align 2, 0
_0806C414: .4byte gUnk_081A6524
_0806C418: .4byte 0x0201DB20
_0806C41C: .4byte 0x000017DA
_0806C420: .4byte 0x00001C1C
_0806C424: .4byte 0x00001BB0
_0806C428: .4byte 0x00001BBC
_0806C42C: .4byte 0x000018B0
_0806C430: .4byte 0x0201F740
_0806C434: .4byte 0xFFFFFAF8
_0806C438: .4byte gUnk_081A6EA4
_0806C43C: .4byte 0xFFFFE3E0
_0806C440: .4byte 0xFFFFE9F8
_0806C444: .4byte 0xFFFFE9FE
_0806C448: .4byte 0x04000050
_0806C44C: .4byte 0x00003FC8
_0806C450:
	mov r0, #1
	b _0806C4D4
_0806C454:
	ldr r0, _0806C47C @ =0xFFFFEA08
	add r0, r8
	ldrb r0, [r0]
	cmp r0, #0
	bne _0806C4D2
	cmp r1, #0
	bne _0806C4D2
	ldr r4, _0806C480 @ =0xFFFFFC8C
	add r4, r8
	ldrh r5, [r4]
	lsl r2, r5, #0x10
	cmp r2, #0
	blt _0806C48C
	ldr r1, _0806C484 @ =0x04000050
	ldr r6, _0806C488 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #4
	asr r0, r2, #0x18
	b _0806C49E
_0806C47C: .4byte 0xFFFFEA08
_0806C480: .4byte 0xFFFFFC8C
_0806C484: .4byte 0x04000050
_0806C488: .4byte 0x00003FC8
_0806C48C:
	ldr r1, _0806C4B8 @ =0x04000050
	ldr r2, _0806C4BC @ =0x00003F88
	add r0, r2, #0
	strh r0, [r1]
	add r1, #4
	mov r3, #0
	ldsh r0, [r4, r3]
	neg r0, r0
	asr r0, r0, #8
_0806C49E:
	strh r0, [r1]
	ldr r0, _0806C4C0 @ =0x0201DB20
	ldr r4, _0806C4C4 @ =0x000018AC
	add r2, r0, r4
	ldrh r3, [r2]
	mov r5, #0
	ldsh r1, [r2, r5]
	ldr r0, _0806C4C8 @ =0x000003FF
	cmp r1, r0
	ble _0806C4CC
	add r0, #1
	b _0806C4D0
	.align 2, 0
_0806C4B8: .4byte 0x04000050
_0806C4BC: .4byte 0x00003F88
_0806C4C0: .4byte 0x0201DB20
_0806C4C4: .4byte 0x000018AC
_0806C4C8: .4byte 0x000003FF
_0806C4CC:
	add r0, r3, #0
	add r0, #0x30
_0806C4D0:
	strh r0, [r2]
_0806C4D2:
	mov r0, #0
_0806C4D4:
	add sp, #0x2C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0806B3B0

