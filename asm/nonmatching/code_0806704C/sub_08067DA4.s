	thumb_func_start sub_08067DA4
sub_08067DA4: @ 0x08067DA4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	str r0, [sp, #0xC]
	ldr r6, _08067F24 @ =0x0201DB20
	ldr r0, _08067F28 @ =0x0201F73C
	ldrb r1, [r0]
	lsl r2, r1, #1
	ldr r4, _08067F2C @ =0x0201E140
	add r3, r2, r4
	mov r5, #0xA5
	lsl r5, r5, #5
	add r5, r5, r6
	mov sl, r5
	add r1, sl
	ldrb r7, [r1]
	lsl r0, r7, #1
	add r1, r7, #0
	add r0, r0, r1
	lsl r0, r0, #1
	add r2, r2, r0
	ldr r0, _08067F30 @ =0x0201EFB4
	add r2, r2, r0
	ldrh r0, [r2]
	sub r0, #1
	ldrh r3, [r3]
	cmp r3, r0
	blt _08067DE4
	b _0806816C
_08067DE4:
	ldr r2, _08067F34 @ =0x00001C58
	add r1, r6, r2
	mov r5, #0
	mov r0, #0x1E
	strh r0, [r1]
	ldr r3, _08067F38 @ =0x000018AC
	add r1, r6, r3
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
	ldr r7, _08067F3C @ =0x00000634
	add r4, r6, r7
	mov r0, #1
	ldrb r1, [r4]
	eor r0, r1
	strb r0, [r4]
	ldr r2, _08067F28 @ =0x0201F73C
	ldrb r0, [r2]
	mov r3, sl
	add r1, r0, r3
	ldrb r1, [r1]
	lsl r3, r0, #1
	ldr r7, _08067F2C @ =0x0201E140
	add r3, r3, r7
	ldrh r2, [r3]
	add r2, #1
	strh r2, [r3]
	ldr r7, _08067F40 @ =0x0000FFFF
	mov r3, sp
	strh r7, [r3, #0x10]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrb r2, [r4]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _08067F44 @ =0x06008000
	add r1, r1, r3
	bl sub_0807AFFC
	mov r7, #0xC6
	lsl r7, r7, #3
	add r1, r6, r7
	mov r0, #0xFF
	mov r8, r0
	ldrh r1, [r1]
	and r0, r1
	lsr r0, r0, #3
	add r0, #0x13
	ldr r1, _08067F48 @ =0x00000632
	add r2, r6, r1
	mov r1, r8
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #0xC
	ldrb r2, [r4]
	str r5, [sp, #0]
	mov r3, #1
	bl sub_08064E28
	ldr r2, _08067F4C @ =0x00000635
	add r1, r6, r2
	mov r0, #2
	strb r0, [r1]
	ldr r0, _08067F50 @ =0x08087494
	mov r3, #4
	ldsh r7, [r0, r3]
	ldr r4, _08067F54 @ =0x0000063A
	add r4, r4, r6
	mov r9, r4
	ldrh r5, [r4]
	add r3, r5, r7
	mov r0, r8
	and r3, r0
	asr r3, r3, #3
	mov r0, #0x1B
	str r0, [sp, #0]
	mov r0, #3
	str r0, [sp, #4]
	mov r1, #0xC8
	lsl r1, r1, #3
	add r4, r6, r1
	str r4, [sp, #8]
	mov r0, #0
	ldr r1, _08067F58 @ =0x0600D000
	mov r2, #3
	bl sub_08079834
	ldr r2, _08067F28 @ =0x0201F73C
	ldrb r5, [r2]
	lsl r1, r5, #1
	ldr r3, _08067F2C @ =0x0201E140
	add r0, r1, r3
	ldrh r2, [r0]
	add r2, #2
	mov r6, sl
	add r0, r5, r6
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	ldr r0, _08067F30 @ =0x0201EFB4
	add r1, r1, r0
	ldrh r1, [r1]
	cmp r2, r1
	bge _08067F5C
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r5, #0
	add r1, r3, #0
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, r9
	ldrh r1, [r1]
	add r3, r1, r7
	mov r2, r8
	and r3, r2
	lsr r3, r3, #3
	str r4, [sp, #0]
	mov r1, #6
	str r1, [sp, #4]
	ldr r1, _08067F58 @ =0x0600D000
	mov r2, #0
	bl sub_08065108
	ldr r3, _08067F28 @ =0x0201F73C
	ldrb r0, [r3]
	add r1, r0, r6
	ldrb r1, [r1]
	lsl r2, r0, #1
	ldr r4, _08067F2C @ =0x0201E140
	add r2, r2, r4
	ldrh r2, [r2]
	add r2, #2
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	bl sub_08068D1C
	ldr r5, [sp, #0xC]
	strh r0, [r5]
	b _08067F64
	.align 2, 0
_08067F24: .4byte 0x0201DB20
_08067F28: .4byte 0x0201F73C
_08067F2C: .4byte 0x0201E140
_08067F30: .4byte 0x0201EFB4
_08067F34: .4byte 0x00001C58
_08067F38: .4byte 0x000018AC
_08067F3C: .4byte 0x00000634
_08067F40: .4byte 0x0000FFFF
_08067F44: .4byte 0x06008000
_08067F48: .4byte 0x00000632
_08067F4C: .4byte 0x00000635
_08067F50: .4byte gUnk_08087494
_08067F54: .4byte 0x0000063A
_08067F58: .4byte 0x0600D000
_08067F5C:
	mov r6, sp
	ldrh r7, [r6, #0x10]
	ldr r6, [sp, #0xC]
	strh r7, [r6]
_08067F64:
	ldr r0, _080680F8 @ =0x0600D000
	mov sl, r0
	ldr r0, _080680FC @ =0x08087494
	mov r1, #6
	ldsh r4, [r0, r1]
	ldr r5, _08068100 @ =0x0201DB20
	ldr r2, _08068104 @ =0x0000063A
	add r2, r2, r5
	mov r8, r2
	ldrh r6, [r2]
	add r3, r6, r4
	mov r6, #0xFF
	and r3, r6
	asr r3, r3, #3
	mov r0, #0x1B
	str r0, [sp, #0]
	mov r7, #3
	str r7, [sp, #4]
	mov r0, #0xC8
	lsl r0, r0, #3
	add r0, r0, r5
	mov r9, r0
	str r0, [sp, #8]
	mov r0, #0
	mov r1, sl
	mov r2, #3
	bl sub_08079834
	ldr r1, _08068108 @ =0x0201F73C
	ldrb r3, [r1]
	lsl r0, r3, #1
	ldr r1, _0806810C @ =0x0201E140
	add r2, r0, r1
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #1
	cmp r0, #0
	blt _08067FE4
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r5, r1
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r2, r8
	ldrh r2, [r2]
	add r3, r2, r4
	and r3, r6
	lsr r3, r3, #3
	mov r4, r9
	str r4, [sp, #0]
	str r7, [sp, #4]
	mov r1, sl
	mov r2, #0
	bl sub_08065108
_08067FE4:
	ldr r7, _08068110 @ =0x0600C000
	mov r8, r7
	ldr r0, _08068114 @ =0x0808749C
	mov r1, #2
	ldsh r4, [r0, r1]
	ldr r2, _08068118 @ =0x0000063E
	add r7, r5, r2
	ldrh r0, [r7]
	add r3, r0, r4
	and r3, r6
	asr r3, r3, #3
	mov r0, #0x1E
	str r0, [sp, #0]
	mov r0, #6
	str r0, [sp, #4]
	mov r1, r9
	str r1, [sp, #8]
	mov r0, #0
	mov r1, r8
	mov r2, #0
	bl sub_08079834
	ldr r2, _08068108 @ =0x0201F73C
	ldrb r3, [r2]
	lsl r2, r3, #1
	ldr r1, _0806811C @ =0x0201EFC0
	add r0, r3, r1
	ldrb r1, [r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	mov ip, r0
	ldr r0, _08068120 @ =0x00001494
	add r0, r0, r5
	mov sl, r0
	mov r0, ip
	add r0, sl
	ldrh r0, [r0]
	cmp r0, #0
	bne _08068038
	b _0806813C
_08068038:
	ldr r0, _0806810C @ =0x0201E140
	add r2, r2, r0
	ldrh r2, [r2]
	add r0, r3, #0
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrh r1, [r7]
	add r3, r1, r4
	and r3, r6
	lsr r3, r3, #3
	mov r2, r9
	str r2, [sp, #0]
	mov r1, r8
	mov r2, #0
	bl sub_0806518C
	ldrh r2, [r7]
	add r2, #0x60
	and r2, r6
	lsr r2, r2, #3
	mov r0, r8
	mov r1, #0xB
	mov r3, r9
	bl sub_08065AB4
	ldrh r2, [r7]
	add r2, #0x60
	and r2, r6
	lsr r2, r2, #3
	mov r0, r8
	mov r1, #0x11
	mov r3, #6
	bl sub_08065E6C
	mov r0, #5
	bl sub_080657F8
	ldr r3, _08068108 @ =0x0201F73C
	ldrb r1, [r3]
	lsl r2, r1, #1
	ldr r4, _0806811C @ =0x0201EFC0
	add r1, r1, r4
	ldrb r6, [r1]
	lsl r0, r6, #1
	add r0, r0, r6
	lsl r0, r0, #1
	add r0, r2, r0
	add r0, sl
	ldrh r0, [r0]
	ldr r7, _0806810C @ =0x0201E140
	add r2, r2, r7
	ldrh r1, [r2]
	ldr r3, _08068124 @ =0x00001BB0
	add r2, r5, r3
	bl sub_08065F34
	ldr r4, _08068128 @ =0x00001BB7
	add r1, r5, r4
	ldrb r0, [r1]
	cmp r0, #0
	beq _080680C6
	mov r0, #2
	strb r0, [r1]
	ldr r6, _0806812C @ =0x00001BB6
	add r1, r5, r6
	mov r0, #1
	ldrb r7, [r1]
	orr r0, r7
	strb r0, [r1]
_080680C6:
	ldr r1, _08068130 @ =0x00000635
	add r0, r5, r1
	ldrb r0, [r0]
	ldr r2, [sp, #0xC]
	ldrh r1, [r2]
	ldr r3, _08068108 @ =0x0201F73C
	ldrb r4, [r3]
	ldr r6, _0806811C @ =0x0201EFC0
	add r3, r4, r6
	ldrb r7, [r3]
	lsl r2, r7, #1
	add r3, r7, #0
	add r2, r2, r3
	add r2, r2, r4
	lsl r2, r2, #1
	add r2, sl
	ldrh r2, [r2]
	ldr r4, _08068134 @ =0x00001BB8
	add r3, r5, r4
	ldr r6, _08068138 @ =0x000018B0
	add r4, r5, r6
	str r4, [sp, #0]
	bl sub_08066478
	b _08068160
_080680F8: .4byte 0x0600D000
_080680FC: .4byte gUnk_08087494
_08068100: .4byte 0x0201DB20
_08068104: .4byte 0x0000063A
_08068108: .4byte 0x0201F73C
_0806810C: .4byte 0x0201E140
_08068110: .4byte 0x0600C000
_08068114: .4byte gUnk_0808749C
_08068118: .4byte 0x0000063E
_0806811C: .4byte 0x0201EFC0
_08068120: .4byte 0x00001494
_08068124: .4byte 0x00001BB0
_08068128: .4byte 0x00001BB7
_0806812C: .4byte 0x00001BB6
_08068130: .4byte 0x00000635
_08068134: .4byte 0x00001BB8
_08068138: .4byte 0x000018B0
_0806813C:
	ldr r5, _0806817C @ =0x0201E140
	add r0, r2, r5
	ldrh r2, [r0]
	add r0, r3, #0
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrh r7, [r7]
	add r3, r7, r4
	and r3, r6
	lsr r3, r3, #3
	mov r6, r9
	str r6, [sp, #0]
	mov r1, r8
	mov r2, #0
	bl sub_08065384
_08068160:
	mov r0, #2
	bl sub_08065058
	mov r0, #0
	bl sub_08077AEC
_0806816C:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0806817C: .4byte 0x0201E140
	thumb_func_end sub_08067DA4

