	thumb_func_start sub_0807382C
sub_0807382C: @ 0x0807382C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x48
	str r0, [sp, #0x30]
	str r1, [sp, #0x34]
	mov r0, #1
	str r0, [sp, #0x40]
	mov r7, #0
	mov r1, #0
	str r1, [sp, #0x44]
	mov r0, sp
	bl sub_080740BC
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x38]
	mov r0, #0x30
	ldr r2, [sp, #0x38]
	and r0, r2
	cmp r0, #0
	beq _0807386A
	add r0, sp, #0x20
	mov r1, #0xA0
	lsl r1, r1, #7
	strh r1, [r0]
	bl sub_08074218
	b _08073B38
_0807386A:
	mov r0, #0xF
	ldr r3, [sp, #0x38]
	and r0, r3
	cmp r0, #0
	bne _08073876
	b _08073B38
_08073876:
	mov r0, #0
	str r0, [sp, #0x3C]
	ldr r1, _080738C8 @ =0x03006654
	mov r9, r1
	add r2, sp, #0x20
	mov sl, r2
_08073882:
	mov r0, #0xF
	ldr r3, [sp, #0x38]
	and r0, r3
	ldr r1, [sp, #0x40]
	and r0, r1
	cmp r0, #0
	bne _08073892
	b _08073B1A
_08073892:
	lsl r1, r7, #4
	mov r2, sp
	add r3, r2, r1
	ldrh r4, [r3]
	mov r2, #0xF0
	lsl r2, r2, #8
	add r0, r2, #0
	add r2, r0, #0
	and r2, r4
	mov r6, #0xC0
	lsl r6, r6, #6
	cmp r2, r6
	bne _080738AE
	b _080739DC
_080738AE:
	cmp r2, r6
	bgt _080738CC
	mov r0, #0x80
	lsl r0, r0, #5
	lsl r5, r7, #1
	cmp r2, r0
	bne _080738BE
	b _08073ADC
_080738BE:
	mov r0, #0x80
	lsl r0, r0, #6
	cmp r2, r0
	beq _080738E4
	b _08073ADC
_080738C8: .4byte 0x03006654
_080738CC:
	mov r3, #0x80
	lsl r3, r3, #7
	mov r8, r3
	lsl r5, r7, #1
	cmp r2, r8
	beq _08073910
	mov r0, #0xA0
	lsl r0, r0, #7
	cmp r2, r0
	bne _080738E2
	b _08073A58
_080738E2:
	b _08073ADC
_080738E4:
	ldr r2, _08073974 @ =0x03005B60
	ldr r3, _08073978 @ =0x00000AF8
	add r0, r2, r3
	add r3, r5, r0
	ldr r2, _0807397C @ =0x000001FF
	add r0, r2, #0
	and r0, r4
	mov r6, #0
	strh r0, [r3]
	ldr r0, _08073980 @ =0x03006650
	add r4, r5, r0
	ldrh r2, [r3]
	add r0, r2, #0
	add r0, #0x10
	asr r0, r0, #4
	strh r0, [r4]
	ldrh r2, [r3]
	add r0, r2, r0
	strh r0, [r3]
	mov r3, r9
	add r0, r5, r3
	strh r6, [r0]
_08073910:
	mov r2, sp
	add r0, r2, r1
	add r0, #2
	lsl r1, r7, #8
	add r1, r1, r7
	lsl r1, r1, #2
	ldr r6, _08073974 @ =0x03005B60
	mov r3, r9
	add r4, r5, r3
	ldrh r3, [r4]
	lsl r2, r3, #3
	sub r2, r2, r3
	lsl r3, r2, #8
	add r2, r2, r3
	lsl r2, r2, #1
	mov r3, #0x83
	lsl r3, r3, #2
	add r3, r3, r6
	mov r8, r3
	add r2, r8
	add r1, r1, r2
	mov r2, #7
	bl CpuSet
	ldrh r2, [r4]
	add r2, #1
	mov r3, #0
	strh r2, [r4]
	ldr r0, _08073984 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x30
	and r0, r1
	lsr r0, r0, #4
	cmp r7, r0
	bne _080739CA
	lsl r0, r2, #0x10
	cmp r0, #0
	bne _08073988
	ldr r0, _08073978 @ =0x00000AF8
	add r1, r6, r0
	add r1, r5, r1
	mov r2, #0x80
	lsl r2, r2, #6
	add r0, r2, #0
	ldrh r1, [r1]
	orr r0, r1
	mov r3, sl
	strh r0, [r3]
	b _080739AC
	.align 2, 0
_08073974: .4byte 0x03005B60
_08073978: .4byte 0x00000AF8
_0807397C: .4byte 0x000001FF
_08073980: .4byte 0x03006650
_08073984: .4byte 0x04000128
_08073988:
	ldrh r0, [r4]
	ldr r2, _080739A0 @ =0x03006650
	add r1, r5, r2
	ldrh r1, [r1]
	sub r1, #1
	cmp r0, r1
	bne _080739A4
	mov r0, #0xC0
	lsl r0, r0, #6
	mov r3, sl
	strh r0, [r3]
	b _080739AC
_080739A0: .4byte 0x03006650
_080739A4:
	mov r0, #0x80
	lsl r0, r0, #7
	mov r1, sl
	strh r0, [r1]
_080739AC:
	add r4, sp, #0x20
	mov r2, r9
	add r0, r5, r2
	ldrh r0, [r0]
	lsl r0, r0, #4
	ldr r3, _080739D8 @ =0x03005B68
	add r0, r0, r3
	mov r1, sp
	add r1, #0x22
	mov r2, #7
	bl CpuSet
	add r0, r4, #0
	bl sub_08074218
_080739CA:
	mov r0, r9
	add r1, r5, r0
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
	b _08073ADC
	.align 2, 0
_080739D8: .4byte 0x03005B68
_080739DC:
	ldr r6, _08073A44 @ =0x03005B60
	lsl r0, r7, #1
	ldr r2, _08073A48 @ =0x03006650
	add r1, r0, r2
	ldrh r1, [r1]
	add r5, r0, #0
	cmp r1, #0
	bne _080739FA
	ldr r1, _08073A4C @ =0x00000AF8
	add r0, r6, r1
	add r0, r5, r0
	ldr r2, _08073A50 @ =0x000001FF
	add r1, r2, #0
	and r1, r4
	strh r1, [r0]
_080739FA:
	ldr r0, _08073A54 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x30
	and r0, r1
	lsr r0, r0, #4
	cmp r7, r0
	bne _08073A14
	mov r0, #0xA4
	lsl r0, r0, #4
	add r1, r6, r0
	mov r0, #0x80
	lsl r0, r0, #5
	strh r0, [r1]
_08073A14:
	add r0, r3, #2
	lsl r1, r7, #8
	add r1, r1, r7
	lsl r1, r1, #2
	mov r2, r9
	add r4, r5, r2
	ldrh r3, [r4]
	lsl r2, r3, #3
	sub r2, r2, r3
	lsl r3, r2, #8
	add r2, r2, r3
	lsl r2, r2, #1
	mov r3, #0x83
	lsl r3, r3, #2
	add r6, r6, r3
	add r2, r2, r6
	add r1, r1, r2
	mov r2, #7
	bl CpuSet
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
	b _08073ADC
_08073A44: .4byte 0x03005B60
_08073A48: .4byte 0x03006650
_08073A4C: .4byte 0x00000AF8
_08073A50: .4byte 0x000001FF
_08073A54: .4byte 0x04000128
_08073A58:
	ldr r4, _08073A94 @ =0x03005B60
	mov r0, r9
	add r2, r5, r0
	ldrh r3, [r2]
	sub r3, #1
	mov r1, #0
	mov ip, r1
	strh r3, [r2]
	ldr r0, _08073A98 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x30
	and r0, r1
	lsr r0, r0, #4
	cmp r7, r0
	bne _08073ADC
	lsl r0, r3, #0x10
	cmp r0, #0
	bne _08073AA0
	ldr r2, _08073A9C @ =0x00000AF8
	add r1, r4, r2
	add r1, r5, r1
	mov r3, #0x80
	lsl r3, r3, #6
	add r0, r3, #0
	ldrh r1, [r1]
	orr r0, r1
	mov r1, sl
	strh r0, [r1]
	b _08073ABE
	.align 2, 0
_08073A94: .4byte 0x03005B60
_08073A98: .4byte 0x04000128
_08073A9C: .4byte 0x00000AF8
_08073AA0:
	ldrh r0, [r2]
	ldr r2, _08073AB4 @ =0x03006650
	add r1, r5, r2
	ldrh r1, [r1]
	sub r1, #1
	cmp r0, r1
	bne _08073AB8
	mov r3, sl
	strh r6, [r3]
	b _08073ABE
_08073AB4: .4byte 0x03006650
_08073AB8:
	mov r1, r8
	mov r0, sl
	strh r1, [r0]
_08073ABE:
	add r4, sp, #0x20
	mov r2, r9
	add r0, r5, r2
	ldrh r0, [r0]
	lsl r0, r0, #4
	ldr r3, _08073B44 @ =0x03005B68
	add r0, r0, r3
	mov r1, sp
	add r1, #0x22
	mov r2, #7
	bl CpuSet
	add r0, r4, #0
	bl sub_08074218
_08073ADC:
	ldr r2, _08073B48 @ =0x03005B60
	mov r0, r9
	add r4, r5, r0
	ldrh r3, [r4]
	lsl r1, r3, #4
	sub r1, r1, r3
	ldr r3, _08073B4C @ =0x00000AF8
	add r0, r2, r3
	add r5, r5, r0
	ldrh r0, [r5]
	cmp r1, r0
	blt _08073B1A
	ldr r1, [sp, #0x30]
	cmp r7, r1
	bne _08073B14
	str r0, [sp, #0x44]
	lsl r0, r7, #8
	add r0, r0, r7
	lsl r0, r0, #2
	mov r3, #0x83
	lsl r3, r3, #2
	add r1, r2, r3
	add r0, r0, r1
	ldr r1, [sp, #0x44]
	lsr r2, r1, #1
	ldr r1, [sp, #0x34]
	bl CpuSet
_08073B14:
	mov r0, #0
	strh r0, [r5]
	strh r0, [r4]
_08073B1A:
	ldr r2, [sp, #0x40]
	lsl r0, r2, #0x19
	lsr r0, r0, #0x18
	str r0, [sp, #0x40]
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	ldr r0, [sp, #0x3C]
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x3C]
	cmp r0, #1
	bhi _08073B38
	b _08073882
_08073B38:
	ldr r3, [sp, #0x44]
	cmp r3, #0
	bne _08073B50
	mov r0, #0
	b _08073B52
	.align 2, 0
_08073B44: .4byte 0x03005B68
_08073B48: .4byte 0x03005B60
_08073B4C: .4byte 0x00000AF8
_08073B50:
	ldr r0, [sp, #0x44]
_08073B52:
	add sp, #0x48
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0807382C
	.align 2, 0

