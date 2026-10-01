	thumb_func_start sub_08026E84
sub_08026E84: @ 0x08026E84
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x44
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0x20]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x24]
	mov r0, #0
	str r0, [sp, #0x40]
	mov r1, #0
	str r1, [sp, #0x28]
_08026EA4:
	mov r2, #7
	str r2, [sp, #0x3C]
	ldr r3, [sp, #0x28]
	cmp r3, #4
	bls _08026EB0
	b _0802748C
_08026EB0:
	lsl r0, r3, #2
	ldr r1, _08026EBC @ =0x08026EC0
	add r1, r0, r1
	ldr r1, [r1]
	add r5, r0, #0
	mov pc, r1
_08026EBC: .4byte 0x08026EC0
_08026EC0:
	.4byte _08026ED4
	.4byte _08026F20
	.4byte _08026F38
	.4byte _08026F38
	.4byte _08026F38
_08026ED4:
	ldr r2, _08026F10 @ =0x08087BA4
	ldr r4, _08026F14 @ =0x02020310
	ldr r0, _08026F18 @ =0x00000B0C
	add r1, r4, r0
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r3, r0
	asr r0, r0, #1
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #0xC0
	bl sub_0807B4D0
	mov r3, #0x80
	lsl r3, r3, #1
	add r1, r3, #0
	sub r1, r1, r0
	ldr r2, _08026F1C @ =0x0000061A
	add r0, r4, r2
	strh r1, [r0]
	mov r3, #0xC3
	lsl r3, r3, #3
	add r4, r4, r3
	strh r1, [r4]
	b _08026F7C
	.align 2, 0
_08026F10: .4byte gUnk_08087BA4
_08026F14: .4byte 0x02020310
_08026F18: .4byte 0x00000B0C
_08026F1C: .4byte 0x0000061A
_08026F20:
	ldr r2, _08026F2C @ =0x08087BA4
	ldr r4, _08026F30 @ =0x02020310
	add r1, r5, r4
	ldr r0, _08026F34 @ =0x00000B0C
	add r1, r1, r0
	b _08026F42
_08026F2C: .4byte gUnk_08087BA4
_08026F30: .4byte 0x02020310
_08026F34: .4byte 0x00000B0C
_08026F38:
	ldr r2, _08026F8C @ =0x08087BA4
	ldr r4, _08026F90 @ =0x02020310
	add r1, r5, r4
	ldr r3, _08026F94 @ =0x00000B0C
	add r1, r1, r3
_08026F42:
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r3, r0
	asr r0, r0, #1
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #0xC0
	bl sub_0807B4D0
	ldr r3, [sp, #0x28]
	lsl r1, r3, #1
	add r1, r1, r3
	lsl r1, r1, #3
	add r1, r1, r4
	mov r3, #0x80
	lsl r3, r3, #1
	add r2, r3, #0
	sub r2, r2, r0
	ldr r3, _08026F98 @ =0x0000061A
	add r0, r1, r3
	strh r2, [r0]
	mov r0, #0xC3
	lsl r0, r0, #3
	add r1, r1, r0
	strh r2, [r1]
_08026F7C:
	ldr r1, [sp, #0x28]
	cmp r1, #4
	bls _08026F84
	b _0802748C
_08026F84:
	ldr r0, _08026F9C @ =0x08026FA0
	add r0, r5, r0
	ldr r0, [r0]
	mov pc, r0
_08026F8C: .4byte gUnk_08087BA4
_08026F90: .4byte 0x02020310
_08026F94: .4byte 0x00000B0C
_08026F98: .4byte 0x0000061A
_08026F9C: .4byte 0x08026FA0
_08026FA0:
	.4byte _08026FB4
	.4byte _080270AC
	.4byte _08027180
	.4byte _08027268
	.4byte _0802737C
_08026FB4:
	ldr r0, _08026FD0 @ =0x02020310
	ldr r2, _08026FD4 @ =0x00000B0D
	add r2, r2, r0
	mov r8, r2
	ldrb r1, [r2]
	mov sl, r0
	cmp r1, #1
	beq _08026FF2
	cmp r1, #1
	bgt _08026FD8
	cmp r1, #0
	beq _08026FDE
	b _080270A4
	.align 2, 0
_08026FD0: .4byte 0x02020310
_08026FD4: .4byte 0x00000B0D
_08026FD8:
	cmp r1, #2
	beq _080270A0
	b _080270A4
_08026FDE:
	mov r3, #0
	str r3, [sp, #0x30]
	mov r9, r3
	mov r0, r9
	str r0, [sp, #0x34]
	mov r1, #0
	str r1, [sp, #0x38]
	mov r2, #0
	str r2, [sp, #0x2C]
	b _080270A4
_08026FF2:
	mov r5, #0xA0
	lsl r5, r5, #4
	ldr r6, _08027098 @ =0x08087BA4
	add r0, r6, #0
	add r0, #0x60
	mov r3, #0
	ldsh r1, [r0, r3]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldr r7, _0802709C @ =0x00000B0C
	add r7, sl
	ldrb r0, [r7]
	add r0, #0x30
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #4
	asr r0, r0, #4
	sub r0, r0, r4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	mov r5, #0xC0
	lsl r5, r5, #7
	mov r3, #0x20
	ldsh r1, [r6, r3]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldrb r0, [r7]
	add r0, #0x10
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #8
	asr r0, r0, #8
	sub r4, r4, r0
	mov r3, r9
	lsl r1, r3, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xE
	add r0, r1, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x30]
	lsl r4, r4, #0x10
	add r0, r4, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x34]
	mov r3, #0x80
	lsl r3, r3, #0xD
	add r1, r1, r3
	lsr r1, r1, #0x10
	mov r9, r1
	add r4, r4, r3
	lsr r4, r4, #0x10
	str r4, [sp, #0x38]
	mov r0, #0xC0
	lsl r0, r0, #2
	str r0, [sp, #0x2C]
	ldrb r1, [r7]
	cmp r1, #0x6C
	bne _08027090
	mov r2, r8
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
_08027090:
	ldrb r0, [r7]
	add r0, #2
	strb r0, [r7]
	b _080270A4
_08027098: .4byte gUnk_08087BA4
_0802709C: .4byte 0x00000B0C
_080270A0:
	mov r3, #1
	str r3, [sp, #0x40]
_080270A4:
	mov r0, #0xC3
	lsl r0, r0, #3
	add r0, sl
	b _0802747C
_080270AC:
	ldr r0, _080270C8 @ =0x02020310
	add r7, r5, r0
	ldr r1, _080270CC @ =0x00000B0D
	add r1, r1, r7
	mov r8, r1
	ldrb r1, [r1]
	mov sl, r0
	cmp r1, #1
	beq _080270D0
	cmp r1, #1
	bgt _080270C4
	b _0802727E
_080270C4:
	b _0802728C
	.align 2, 0
_080270C8: .4byte 0x02020310
_080270CC: .4byte 0x00000B0D
_080270D0:
	mov r5, #0x80
	lsl r5, r5, #3
	ldr r6, _08027178 @ =0x08087BA4
	add r0, r6, #0
	add r0, #0x60
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldr r3, _0802717C @ =0x00000B0C
	add r7, r7, r3
	ldrb r0, [r7]
	add r0, #0x30
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #4
	asr r0, r0, #4
	sub r0, r0, r4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	mov r5, #0x80
	lsl r5, r5, #7
	mov r3, #0x20
	ldsh r1, [r6, r3]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldrb r0, [r7]
	add r0, #0x10
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #8
	asr r0, r0, #8
	sub r4, r4, r0
	mov r3, r9
	lsl r1, r3, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xE
	add r0, r1, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x30]
	lsl r4, r4, #0x10
	add r0, r4, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x34]
	mov r3, #0x80
	lsl r3, r3, #0xD
	add r1, r1, r3
	lsr r1, r1, #0x10
	mov r9, r1
	add r4, r4, r3
	lsr r4, r4, #0x10
	str r4, [sp, #0x38]
	mov r0, #0xC0
	lsl r0, r0, #2
	str r0, [sp, #0x2C]
	ldrb r1, [r7]
	cmp r1, #0x84
	bne _0802716E
_08027166:
	mov r2, r8
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
_0802716E:
	ldrb r0, [r7]
	add r0, #2
	strb r0, [r7]
	b _08027358
	.align 2, 0
_08027178: .4byte gUnk_08087BA4
_0802717C: .4byte 0x00000B0C
_08027180:
	ldr r0, _0802719C @ =0x02020310
	add r1, r5, r0
	ldr r2, _080271A0 @ =0x00000B0D
	add r2, r2, r1
	mov r8, r2
	ldrb r2, [r2]
	mov sl, r0
	cmp r2, #1
	beq _080271C0
	cmp r2, #1
	bgt _080271A4
	cmp r2, #0
	beq _080271AC
	b _08027358
_0802719C: .4byte 0x02020310
_080271A0: .4byte 0x00000B0D
_080271A4:
	cmp r2, #2
	bne _080271AA
	b _08027354
_080271AA:
	b _08027358
_080271AC:
	mov r3, #0
	str r3, [sp, #0x30]
	mov r9, r3
	mov r0, r9
	str r0, [sp, #0x34]
	mov r1, #0
	str r1, [sp, #0x38]
	mov r2, #0
	str r2, [sp, #0x2C]
	b _08027358
_080271C0:
	mov r5, #0x80
	lsl r5, r5, #2
	ldr r6, _08027260 @ =0x08087BA4
	ldr r3, _08027264 @ =0x00000B0C
	add r7, r1, r3
	ldrb r0, [r7]
	add r0, #0x30
	lsl r0, r0, #1
	mov r1, #0xFF
	and r0, r1
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	add r0, r6, #0
	add r0, #0xE0
	mov r3, #0
	ldsh r1, [r0, r3]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #4
	add r4, #0x23
	asr r0, r0, #4
	sub r4, r4, r0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	mov r9, r4
	mov r5, #0xE0
	lsl r5, r5, #7
	mov r0, #0x20
	ldsh r1, [r6, r0]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldrb r0, [r7]
	add r0, #0x10
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #8
	asr r0, r0, #8
	sub r4, r4, r0
	mov r3, r9
	lsl r1, r3, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xE
	add r0, r1, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x30]
	lsl r4, r4, #0x10
	add r0, r4, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x34]
	mov r3, #0x80
	lsl r3, r3, #0xD
	add r1, r1, r3
	lsr r1, r1, #0x10
	mov r9, r1
	add r4, r4, r3
	lsr r4, r4, #0x10
	str r4, [sp, #0x38]
	mov r0, #0xC0
	lsl r0, r0, #2
	str r0, [sp, #0x2C]
	ldrb r1, [r7]
	cmp r1, #0x96
	bne _0802716E
	b _08027166
	.align 2, 0
_08027260: .4byte gUnk_08087BA4
_08027264: .4byte 0x00000B0C
_08027268:
	ldr r0, _08027284 @ =0x02020310
	add r7, r5, r0
	ldr r1, _08027288 @ =0x00000B0D
	add r1, r1, r7
	mov r8, r1
	ldrb r1, [r1]
	mov sl, r0
	cmp r1, #1
	beq _080272A6
	cmp r1, #1
	bgt _0802728C
_0802727E:
	cmp r1, #0
	beq _08027292
	b _08027358
_08027284: .4byte 0x02020310
_08027288: .4byte 0x00000B0D
_0802728C:
	cmp r1, #2
	beq _08027354
	b _08027358
_08027292:
	mov r2, #0
	str r2, [sp, #0x30]
	mov r9, r2
	mov r3, r9
	str r3, [sp, #0x34]
	mov r0, #0
	str r0, [sp, #0x38]
	mov r1, #0
	str r1, [sp, #0x2C]
	b _08027358
_080272A6:
	mov r5, #0xA0
	lsl r5, r5, #4
	ldr r6, _0802734C @ =0x08087BA4
	add r0, r6, #0
	add r0, #0x60
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldr r3, _08027350 @ =0x00000B0C
	add r7, r7, r3
	ldrb r0, [r7]
	add r0, #0x30
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #4
	asr r0, r0, #4
	sub r4, r4, r0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	mov r9, r4
	mov r5, #0xA0
	lsl r5, r5, #8
	mov r3, #0x20
	ldsh r1, [r6, r3]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldrb r0, [r7]
	add r0, #0x10
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #8
	asr r0, r0, #8
	sub r4, r4, r0
	mov r3, r9
	lsl r1, r3, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xE
	add r0, r1, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x30]
	lsl r4, r4, #0x10
	add r0, r4, r2
	lsr r0, r0, #0x10
	str r0, [sp, #0x34]
	mov r3, #0x80
	lsl r3, r3, #0xD
	add r1, r1, r3
	lsr r1, r1, #0x10
	mov r9, r1
	add r4, r4, r3
	lsr r4, r4, #0x10
	str r4, [sp, #0x38]
	mov r0, #0xC0
	lsl r0, r0, #2
	str r0, [sp, #0x2C]
	ldrb r1, [r7]
	cmp r1, #0x6D
	bne _08027344
	mov r2, r8
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
_08027344:
	ldrb r0, [r7]
	add r0, #1
	strb r0, [r7]
	b _08027358
_0802734C: .4byte gUnk_08087BA4
_08027350: .4byte 0x00000B0C
_08027354:
	mov r3, #1
	str r3, [sp, #0x40]
_08027358:
	ldr r1, [sp, #0x28]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, sl
	mov r2, #0xC3
	lsl r2, r2, #3
	add r0, r0, r2
	mov r3, #0
	ldsh r1, [r0, r3]
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r1, r0
	bgt _08027376
	b _0802748C
_08027376:
	mov r0, #6
	str r0, [sp, #0x3C]
	b _0802748C
_0802737C:
	ldr r0, _08027398 @ =0x02020310
	add r7, r5, r0
	ldr r1, _0802739C @ =0x00000B0D
	add r1, r1, r7
	mov r8, r1
	ldrb r1, [r1]
	mov sl, r0
	cmp r1, #1
	beq _080273BA
	cmp r1, #1
	bgt _080273A0
	cmp r1, #0
	beq _080273A6
	b _0802746C
_08027398: .4byte 0x02020310
_0802739C: .4byte 0x00000B0D
_080273A0:
	cmp r1, #2
	beq _08027468
	b _0802746C
_080273A6:
	mov r2, #0
	str r2, [sp, #0x30]
	mov r9, r2
	mov r3, r9
	str r3, [sp, #0x34]
	mov r0, #0
	str r0, [sp, #0x38]
	mov r1, #0
	str r1, [sp, #0x2C]
	b _0802746C
_080273BA:
	mov r5, #0xA0
	lsl r5, r5, #4
	ldr r6, _08027460 @ =0x08087BA4
	add r0, r6, #0
	add r0, #0x60
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldr r3, _08027464 @ =0x00000B0C
	add r7, r7, r3
	ldrb r0, [r7]
	add r0, #0x30
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #4
	asr r0, r0, #4
	sub r4, r4, r0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	mov r9, r4
	mov r5, #0xC0
	lsl r5, r5, #7
	mov r3, #0x20
	ldsh r1, [r6, r3]
	add r0, r5, #0
	bl sub_0807B4D0
	add r4, r0, #0
	ldrb r0, [r7]
	add r0, #0x10
	lsl r0, r0, #1
	add r0, r0, r6
	mov r2, #0
	ldsh r1, [r0, r2]
	add r0, r5, #0
	bl sub_0807B4D0
	asr r4, r4, #8
	asr r0, r0, #8
	sub r0, r0, r4
	mov r3, r9
	lsl r2, r3, #0x10
	mov r3, #0x80
	lsl r3, r3, #0xE
	add r1, r2, r3
	lsr r1, r1, #0x10
	str r1, [sp, #0x30]
	lsl r0, r0, #0x10
	add r1, r0, r3
	lsr r1, r1, #0x10
	str r1, [sp, #0x34]
	mov r1, #0x80
	lsl r1, r1, #0xD
	add r2, r2, r1
	lsr r2, r2, #0x10
	mov r9, r2
	add r0, r0, r1
	lsr r0, r0, #0x10
	str r0, [sp, #0x38]
	mov r2, #0xC0
	lsl r2, r2, #2
	str r2, [sp, #0x2C]
	ldrb r3, [r7]
	cmp r3, #0x6E
	bne _08027458
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_08027458:
	ldrb r0, [r7]
	add r0, #2
	strb r0, [r7]
	b _0802746C
_08027460: .4byte gUnk_08087BA4
_08027464: .4byte 0x00000B0C
_08027468:
	mov r2, #1
	str r2, [sp, #0x40]
_0802746C:
	ldr r3, [sp, #0x28]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #3
	add r0, sl
	mov r1, #0xC3
	lsl r1, r1, #3
	add r0, r0, r1
_0802747C:
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r1, r0
	ble _0802748C
	mov r3, #6
	str r3, [sp, #0x3C]
_0802748C:
	ldr r0, [sp, #0x40]
	cmp r0, #0
	bne _0802754E
	ldr r1, [sp, #0x28]
	lsl r5, r1, #3
	add r1, r5, #0
	add r1, #8
	ldr r2, _08027574 @ =0x02020C64
	ldr r0, [r2, #4]
	add r0, r0, r1
	ldr r3, [sp, #0x20]
	lsl r1, r3, #0x10
	asr r6, r1, #0x10
	ldr r1, [sp, #0x30]
	lsl r3, r1, #0x10
	asr r3, r3, #0x10
	sub r3, r6, r3
	ldr r2, [sp, #0x24]
	lsl r1, r2, #0x10
	asr r4, r1, #0x10
	ldr r2, [sp, #0x34]
	lsl r1, r2, #0x10
	asr r1, r1, #0x10
	sub r1, r4, r1
	str r1, [sp, #0]
	mov r1, #8
	str r1, [sp, #4]
	mov r2, #1
	mov sl, r2
	str r2, [sp, #8]
	ldr r1, [sp, #0x40]
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	str r1, [sp, #0x14]
	ldr r2, [sp, #0x2C]
	str r2, [sp, #0x18]
	ldr r1, _08027578 @ =0x02020310
	mov r8, r1
	str r1, [sp, #0x1C]
	ldr r1, [sp, #0x3C]
	mov r2, #1
	bl sub_08077EF4
	add r2, r0, #0
	mov r7, #0xC0
	lsl r7, r7, #2
	ldr r3, [sp, #0x2C]
	cmp r3, r7
	bne _080274FC
	ldr r0, _0802757C @ =0x0000C1FF
	ldrh r1, [r2, #2]
	and r0, r1
	ldr r3, [sp, #0x28]
	lsl r1, r3, #9
	orr r0, r1
	strh r0, [r2, #2]
_080274FC:
	add r1, r5, #0
	add r1, #0x30
	ldr r2, _08027574 @ =0x02020C64
	ldr r0, [r2, #4]
	add r0, r0, r1
	mov r1, r9
	lsl r3, r1, #0x10
	asr r3, r3, #0x10
	sub r3, r6, r3
	ldr r2, [sp, #0x38]
	lsl r1, r2, #0x10
	asr r1, r1, #0x10
	sub r1, r4, r1
	str r1, [sp, #0]
	mov r1, #8
	str r1, [sp, #4]
	mov r2, sl
	str r2, [sp, #8]
	ldr r1, [sp, #0x40]
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	str r1, [sp, #0x14]
	ldr r2, [sp, #0x2C]
	str r2, [sp, #0x18]
	mov r1, r8
	str r1, [sp, #0x1C]
	ldr r1, [sp, #0x3C]
	mov r2, #1
	bl sub_08077EF4
	add r2, r0, #0
	ldr r3, [sp, #0x2C]
	cmp r3, r7
	bne _0802754E
	ldr r0, _0802757C @ =0x0000C1FF
	ldrh r1, [r2, #2]
	and r0, r1
	ldr r3, [sp, #0x28]
	lsl r1, r3, #9
	orr r0, r1
	strh r0, [r2, #2]
_0802754E:
	mov r0, #0
	str r0, [sp, #0x40]
	ldr r0, [sp, #0x28]
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x28]
	cmp r0, #4
	bhi _08027562
	b _08026EA4
_08027562:
	add sp, #0x44
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08027574: .4byte 0x02020C64
_08027578: .4byte 0x02020310
_0802757C: .4byte 0x0000C1FF
	thumb_func_end sub_08026E84

