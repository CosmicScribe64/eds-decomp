	thumb_func_start sub_08029B0C
sub_08029B0C: @ 0x08029B0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	ldr r5, _08029B3C @ =0x02020310
	ldr r1, _08029B40 @ =0x00000AF5
	add r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #2
	bne _08029BDE
	ldr r1, _08029B44 @ =0x03000040
	ldrh r2, [r1, #6]
	mov r0, #0x20
	and r0, r2
	add r3, r1, #0
	cmp r0, #0
	beq _08029B4C
	ldr r2, _08029B48 @ =0x00000ABF
	add r1, r5, r2
	mov r0, #0
	b _08029B5A
	.align 2, 0
_08029B3C: .4byte 0x02020310
_08029B40: .4byte 0x00000AF5
_08029B44: .4byte 0x03000040
_08029B48: .4byte 0x00000ABF
_08029B4C:
	mov r0, #0x10
	and r0, r2
	cmp r0, #0
	beq _08029B5C
	ldr r7, _08029BA0 @ =0x00000ABF
	add r1, r5, r7
	mov r0, #1
_08029B5A:
	strb r0, [r1]
_08029B5C:
	mov r1, #1
	add r0, r1, #0
	ldrh r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _08029BAE
	mov r0, #3
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r4, _08029BA4 @ =0x02020DEC
	str r4, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xC]
	mov r1, #0
	mov r2, #0x40
	mov r3, #0xF
	bl sub_0807B9D4
	add r0, r4, #0
	add r0, #0x32
	ldrb r0, [r0]
	cmp r0, #1
	bne _08029BA8
	add r0, r4, #0
	add r0, #0x31
	mov r1, #1
	strb r1, [r0]
	mov r1, #0xB1
	lsl r1, r1, #4
	add r0, r5, r1
	bl sub_0807BCF4
	b _08029BAE
	.align 2, 0
_08029BA0: .4byte 0x00000ABF
_08029BA4: .4byte 0x02020DEC
_08029BA8:
	ldrb r0, [r4, #0x19]
	add r0, #1
	strb r0, [r4, #0x19]
_08029BAE:
	ldr r4, _08029C54 @ =0x02020310
	ldr r2, _08029C58 @ =0x00000B0D
	add r6, r4, r2
	ldrb r0, [r6]
	cmp r0, #0
	beq _08029BDE
	ldr r3, _08029C5C @ =0x00000ABF
	add r0, r4, r3
	ldrb r1, [r0]
	mov r7, #0xB1
	lsl r7, r7, #4
	add r2, r5, r7
	mov r0, #0x51
	bl sub_0807BCFC
	cmp r0, #0
	beq _08029BDE
	ldr r1, _08029C60 @ =0x00000AF5
	add r0, r4, r1
	ldrb r1, [r0]
	add r1, #1
	mov r2, #0
	strb r1, [r0]
	strb r2, [r6]
_08029BDE:
	ldr r5, _08029C64 @ =0x02020E08
	add r0, r5, #0
	bl sub_0807883C
	ldrb r2, [r5, #6]
	cmp r2, #2
	bne _08029BFA
	ldr r0, _08029C68 @ =0x03000040
	ldr r3, _08029C6C @ =0x00004859
	add r0, r0, r3
	ldrb r7, [r0]
	ldrb r2, [r5, #7]
	add r1, r7, r2
	strb r1, [r0]
_08029BFA:
	mov r4, #0
	ldr r3, _08029C70 @ =0xFFFFF508
	add r6, r5, r3
	ldr r7, _08029C74 @ =0x0819A6D0
	mov r9, r7
	mov r8, r6
	ldr r0, _08029C78 @ =0x0000061C
	mov ip, r0
	mov r2, #0x80
	mov sl, r4
	mov r5, #0xC3
	lsl r5, r5, #3
	ldr r3, _08029C7C @ =0x0000061A
_08029C14:
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #3
	add r0, r8
	mov r7, ip
	add r1, r0, r7
	mov r7, sl
	strh r7, [r1]
	add r1, r0, r5
	strh r2, [r1]
	add r0, r0, r3
	strh r2, [r0]
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #3
	bls _08029C14
	ldr r0, _08029C60 @ =0x00000AF5
	add r4, r6, r0
	ldrb r1, [r4]
	lsl r0, r1, #2
	add r0, r9
	ldr r1, [r0]
	add r0, r4, #0
	bl _call_via_r1
	lsl r0, r0, #0x18
	cmp r0, #0
	beq _08029C80
	mov r0, #1
	b _08029CE6
	.align 2, 0
_08029C54: .4byte 0x02020310
_08029C58: .4byte 0x00000B0D
_08029C5C: .4byte 0x00000ABF
_08029C60: .4byte 0x00000AF5
_08029C64: .4byte 0x02020E08
_08029C68: .4byte 0x03000040
_08029C6C: .4byte 0x00004859
_08029C70: .4byte 0xFFFFF508
_08029C74: .4byte gUnk_0819A6D0
_08029C78: .4byte 0x0000061C
_08029C7C: .4byte 0x0000061A
_08029C80:
	ldrb r4, [r4]
	cmp r4, #3
	bhi _08029CB2
	ldr r2, _08029CF8 @ =0x00000ABF
	add r5, r6, r2
	ldrb r0, [r5]
	cmp r0, #0xFF
	beq _08029CB2
	ldr r3, _08029CFC @ =0x00000AC4
	add r4, r6, r3
	add r1, r4, #0
	bl sub_0802899C
	ldrb r0, [r5]
	ldr r7, _08029D00 @ =0x00000AF4
	add r1, r6, r7
	ldrb r1, [r1]
	mov r3, #0xAC
	lsl r3, r3, #4
	add r2, r6, r3
	ldrh r2, [r2]
	str r0, [sp, #0]
	add r3, r4, #0
	bl sub_08028238
_08029CB2:
	mov r4, #0
	ldr r5, _08029D04 @ =0x02020928
_08029CB6:
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #3
	add r0, r0, r5
	bl sub_0807B5A0
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #4
	bls _08029CB6
	ldr r4, _08029D08 @ =0x02020310
	add r0, r4, #0
	bl sub_0807A298
	add r0, r4, #0
	bl sub_0807A2EC
	ldr r7, _08029D00 @ =0x00000AF4
	add r4, r4, r7
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_08029CE6:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08029CF8: .4byte 0x00000ABF
_08029CFC: .4byte 0x00000AC4
_08029D00: .4byte 0x00000AF4
_08029D04: .4byte 0x02020928
_08029D08: .4byte 0x02020310
	thumb_func_end sub_08029B0C

