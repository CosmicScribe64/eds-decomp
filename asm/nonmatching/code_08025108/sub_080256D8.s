	thumb_func_start sub_080256D8
sub_080256D8: @ 0x080256D8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	mov r0, #0x80
	lsl r0, r0, #1
	ldr r6, _08025818 @ =0x08087BA4
	ldr r7, _0802581C @ =0x0201F820
	ldr r1, _08025820 @ =0x00000ADA
	add r4, r7, r1
	mov r5, #0xFF
	add r1, r5, #0
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	add r1, r1, r6
	mov r3, #0
	ldsh r1, [r1, r3]
	bl sub_0807B4D0
	lsl r0, r0, #0xC
	lsr r0, r0, #0x10
	mov r8, r0
	mov r1, #0
	ldsh r0, [r4, r1]
	lsl r0, r0, #1
	and r0, r5
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #0x80
	bl sub_0807B4D0
	mov r3, #0
	ldsh r1, [r4, r3]
	sub r1, #0xBE
	lsr r2, r1, #0x1F
	add r1, r1, r2
	asr r1, r1, #1
	asr r0, r0, #4
	add r1, r1, r0
	sub r1, #8
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0xC]
	ldr r0, _08025824 @ =0x00000ACC
	add r4, r7, r0
	ldrb r0, [r4]
	cmp r0, #0
	bne _08025760
	ldr r1, _08025828 @ =0x00002710
	mov r2, #0xAD
	lsl r2, r2, #4
	add r3, r7, r2
	mov r0, #0
	mov r2, #4
	bl sub_0807B100
	ldr r3, _0802582C @ =0x00000AC4
	add r1, r7, r3
	mov r0, #2
	strb r0, [r1]
	mov r0, #1
	strb r0, [r4]
_08025760:
	mov r1, #0xAD
	lsl r1, r1, #4
	add r0, r7, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08025776
	ldr r2, _08025830 @ =0x00000AC5
	add r1, r7, r2
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_08025776:
	ldr r3, _0802582C @ =0x00000AC4
	add r0, r7, r3
	ldrb r0, [r0]
	cmp r0, #2
	bne _08025840
	ldr r1, _08025834 @ =0x00000AD2
	add r0, r7, r1
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #0x64
	sub r0, r0, r1
	mul r1, r0
	asr r1, r1, #8
	mov r3, #0x3C
	neg r3, r3
	add r0, r3, #0
	sub r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	mov r6, #0
	ldr r2, _08025838 @ =0x08081FD4
	ldr r0, _08025830 @ =0x00000AC5
	add r3, r7, r0
	ldrb r1, [r3]
	lsr r0, r1, #2
	mov r1, #7
	and r0, r1
	lsl r0, r0, #3
	add r0, r0, r2
	mov r1, r8
	add r1, #0x68
	str r1, [sp, #0x10]
	ldrb r0, [r0, #1]
	cmp r6, r0
	bcc _080257C0
	b _080258BA
_080257C0:
	mov r8, r2
	add r4, r7, #0
	add r7, r3, #0
	mov r5, #7
	mov r2, r9
	lsl r0, r2, #0x10
	asr r0, r0, #0x10
	mov sl, r0
_080257D0:
	ldrb r3, [r7]
	lsr r0, r3, #2
	and r0, r5
	lsl r0, r0, #3
	mov r1, r8
	add r1, #4
	add r0, r0, r1
	lsl r1, r6, #3
	ldr r0, [r0]
	add r0, r0, r1
	ldr r2, _0802583C @ =0x00000ACD
	add r1, r4, r2
	ldrb r1, [r1]
	str r1, [sp, #0]
	mov r1, #0x80
	lsl r1, r1, #2
	str r1, [sp, #4]
	str r4, [sp, #8]
	mov r1, #0
	mov r2, #0x68
	mov r3, sl
	add r3, #0x64
	bl sub_08025374
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	ldrb r3, [r7]
	lsr r0, r3, #2
	and r0, r5
	lsl r0, r0, #3
	add r0, r8
	ldrb r0, [r0, #1]
	cmp r6, r0
	bcc _080257D0
	b _080258BA
_08025818: .4byte gUnk_08087BA4
_0802581C: .4byte 0x0201F820
_08025820: .4byte 0x00000ADA
_08025824: .4byte 0x00000ACC
_08025828: .4byte 0x00002710
_0802582C: .4byte 0x00000AC4
_08025830: .4byte 0x00000AC5
_08025834: .4byte 0x00000AD2
_08025838: .4byte gUnk_08081FD4
_0802583C: .4byte 0x00000ACD
_08025840:
	ldr r1, _08025864 @ =0x00000AD2
	add r0, r7, r1
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #0x96
	sub r0, r0, r1
	mul r0, r1
	asr r0, r0, #8
	neg r0, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	mov r6, #0
	mov r3, r8
	add r3, #0x68
	str r3, [sp, #0x10]
	b _08025894
	.align 2, 0
_08025864: .4byte 0x00000AD2
_08025868:
	lsl r1, r6, #3
	ldr r0, [r0, #4]
	add r0, r0, r1
	mov r1, r9
	lsl r3, r1, #0x10
	asr r3, r3, #0x10
	add r3, #0x64
	ldr r2, _0802594C @ =0x00000ACD
	add r1, r5, r2
	ldrb r1, [r1]
	str r1, [sp, #0]
	mov r1, #0x80
	lsl r1, r1, #2
	str r1, [sp, #4]
	str r5, [sp, #8]
	mov r1, #0
	mov r2, #0x68
	bl sub_08025374
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
_08025894:
	ldr r1, _08025950 @ =0x081999EC
	ldr r5, _08025954 @ =0x0201F820
	ldr r3, _08025958 @ =0x00000AC6
	add r0, r5, r3
	ldrb r0, [r0]
	lsl r2, r0, #2
	add r2, r2, r1
	ldr r1, _0802595C @ =0x00000AC5
	add r0, r5, r1
	ldrb r0, [r0]
	lsr r1, r0, #2
	mov r0, #7
	and r1, r0
	ldr r0, [r2]
	lsl r1, r1, #3
	add r0, r1, r0
	ldrb r2, [r0, #1]
	cmp r6, r2
	bcc _08025868
_080258BA:
	ldr r5, _08025960 @ =0x02020138
	mov r0, #1
	strb r0, [r5, #0xE]
	add r0, r5, #0
	bl sub_080254A4
	mov r0, #0x80
	lsl r0, r0, #1
	ldr r3, _08025964 @ =0x08087BA4
	ldr r2, _08025968 @ =0x000001AD
	add r1, r5, r2
	ldrb r1, [r1]
	lsl r1, r1, #1
	add r1, #0x20
	mov r2, #0xFF
	and r1, r2
	lsl r1, r1, #1
	add r1, r1, r3
	mov r3, #0
	ldsh r1, [r1, r3]
	bl sub_0807B4D0
	add r2, r0, #0
	asr r2, r2, #4
	add r2, #8
	ldr r0, [sp, #0xC]
	sub r2, r0, r2
	add r0, r5, #0
	ldr r1, [sp, #0x10]
	mov r3, #0
	bl sub_08025430
	mov r1, #0xDC
	lsl r1, r1, #1
	add r6, r5, r1
	add r0, r6, #0
	bl sub_0807B114
	mov r2, r9
	lsl r0, r2, #0x10
	cmp r0, #0
	ble _080259C8
	mov r3, #0xDD
	lsl r3, r3, #1
	add r2, r5, r3
	mov r0, #0
	mov r1, #0
	strh r1, [r2]
	strb r0, [r6]
	mov r1, #0xD6
	lsl r1, r1, #1
	add r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #2
	bne _08025970
	ldr r1, _0802596C @ =0x00002710
	mov r0, #0
	mov r2, #4
	add r3, r6, #0
	bl sub_0807B100
	bl sub_08076F9C
	lsr r1, r0, #0x1F
	add r1, r0, r1
	asr r1, r1, #1
	lsl r1, r1, #1
	sub r0, r0, r1
	mov r2, #0xD7
	lsl r2, r2, #1
	add r1, r5, r2
	strb r0, [r1]
	b _08025998
_0802594C: .4byte 0x00000ACD
_08025950: .4byte gUnk_081999EC
_08025954: .4byte 0x0201F820
_08025958: .4byte 0x00000AC6
_0802595C: .4byte 0x00000AC5
_08025960: .4byte 0x02020138
_08025964: .4byte gUnk_08087BA4
_08025968: .4byte 0x000001AD
_0802596C: .4byte 0x00002710
_08025970:
	ldr r3, _080259BC @ =0x000001B1
	add r0, r5, r3
	ldrb r0, [r0]
	mov r2, #0xD8
	lsl r2, r2, #1
	add r1, r5, r2
	strb r0, [r1]
	sub r3, #2
	add r0, r5, r3
	ldrb r0, [r0]
	sub r3, #1
	add r2, r5, r3
	strb r0, [r2]
	ldrb r0, [r1]
	add r1, r0, #0
	add r1, #0xA
	mov r2, #1
	add r3, r6, #0
	bl sub_0807B100
_08025998:
	mov r0, #0x1F
	bl sub_08077AEC
	ldr r1, _080259C0 @ =0x04000052
	ldr r2, _080259C4 @ =0x00001010
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r0, #8
	strh r0, [r1]
	sub r1, #4
	mov r3, #0xFD
	lsl r3, r3, #6
	add r0, r3, #0
	strh r0, [r1]
	mov r0, #1
	b _080259CA
	.align 2, 0
_080259BC: .4byte 0x000001B1
_080259C0: .4byte 0x04000052
_080259C4: .4byte 0x00001010
_080259C8:
	mov r0, #0
_080259CA:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_080256D8
	.align 2, 0

