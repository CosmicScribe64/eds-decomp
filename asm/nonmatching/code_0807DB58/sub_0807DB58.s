	thumb_func_start sub_0807DB58
sub_0807DB58: @ 0x0807DB58
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x70
	mov r8, r0
	ldr r0, _0807DBA8 @ =0x00000193
	add r0, r8
	ldrb r0, [r0]
	lsl r4, r0, #8
	mov r0, #0xC8
	lsl r0, r0, #1
	add r0, r8
	ldrh r0, [r0]
	sub r5, r4, r0
	mov r2, #0xC4
	lsl r2, r2, #1
	add r2, r8
	ldrh r0, [r2]
	ldr r3, _0807DBAC @ =0x0000FBF7
	and r3, r0
	strh r3, [r2]
	ldr r0, _0807DBB0 @ =0x00000191
	add r0, r8
	ldrb r6, [r0]
	cmp r5, #0
	beq _0807DBEC
	cmp r5, #0
	ble _0807DBB4
	mov r0, #0xC9
	lsl r0, r0, #1
	add r0, r8
	ldrb r0, [r0]
	lsl r0, r0, #4
	sub r5, r5, r0
	cmp r5, #0
	bge _0807DBC6
	b _0807DBC4
	.align 2, 0
_0807DBA8: .4byte 0x00000193
_0807DBAC: .4byte 0x0000FBF7
_0807DBB0: .4byte 0x00000191
_0807DBB4:
	mov r0, #0xC9
	lsl r0, r0, #1
	add r0, r8
	ldrb r0, [r0]
	lsl r0, r0, #4
	add r5, r5, r0
	cmp r5, #0
	ble _0807DBC6
_0807DBC4:
	mov r5, #0
_0807DBC6:
	mov r1, #0xC8
	lsl r1, r1, #1
	add r1, r8
	sub r0, r4, r5
	strh r0, [r1]
	ldr r0, _0807DBE8 @ =0x00000191
	add r0, r8
	ldrb r0, [r0]
	cmp r6, r0
	beq _0807DC00
	mov r2, #0xC4
	lsl r2, r2, #1
	add r2, r8
	ldrh r1, [r2]
	mov r0, #8
	b _0807DBFC
	.align 2, 0
_0807DBE8: .4byte 0x00000191
_0807DBEC:
	mov r0, #0xF
	and r0, r6
	cmp r0, #0
	beq _0807DC00
	mov r0, #0x80
	lsl r0, r0, #3
	add r1, r0, #0
	add r0, r3, #0
_0807DBFC:
	orr r0, r1
	strh r0, [r2]
_0807DC00:
	mov r0, #0xC4
	lsl r0, r0, #1
	add r0, r8
	ldrh r1, [r0]
	mov r0, #0x80
	lsl r0, r0, #1
	and r0, r1
	cmp r0, #0
	beq _0807DC32
	cmp r6, #0
	bne _0807DC32
	add r7, sp, #0x48
	mov r1, #9
	mov r9, r1
	mov r0, #0
_0807DC1E:
	str r0, [r7]
	str r0, [r7, #4]
	sub r7, #8
	mov r2, #1
	neg r2, r2
	add r9, r2
	mov r3, r9
	cmp r3, #0
	bge _0807DC1E
	b _0807E072
_0807DC32:
	mov r3, #0xC4
	lsl r3, r3, #1
	add r3, r8
	ldrh r1, [r3]
	mov r4, r8
	ldr r4, [r4, #4]
	str r4, [sp, #0x50]
	mov r6, r8
	add r6, #8
	mov r0, #1
	and r0, r1
	str r6, [sp, #0x58]
	cmp r0, #0
	beq _0807DC8C
	mov r7, sp
	mov r0, #9
	mov r9, r0
	mov r0, #0
	mov r3, #1
	mov r2, #0x40
_0807DC5A:
	strb r0, [r6, #0x10]
	add r6, #0x18
	str r0, [r7]
	strb r3, [r7, #4]
	strb r2, [r7, #5]
	add r7, #8
	mov r4, #1
	neg r4, r4
	add r9, r4
	mov r4, r9
	cmp r4, #0
	bge _0807DC5A
	ldr r0, _0807DC88 @ =0xFFFFBF7E
	and r1, r0
	mov r0, #0xC4
	lsl r0, r0, #1
	add r0, r8
	strh r1, [r0]
	mov r0, #1
	neg r0, r0
	mov r1, r8
	str r0, [r1]
	b _0807E072
_0807DC88: .4byte 0xFFFFBF7E
_0807DC8C:
	mov r0, #0xC0
	and r0, r1
	cmp r0, #0
	bne _0807DC96
	b _0807E30C
_0807DC96:
	mov r2, #0x80
	lsl r2, r2, #7
	add r0, r1, #0
	and r0, r2
	mov r4, sp
	add r4, #0x48
	str r4, [sp, #0x60]
	mov r4, r8
	add r4, #0x34
	str r4, [sp, #0x5C]
	add r4, #0x18
	str r4, [sp, #0x64]
	add r4, #0x18
	str r4, [sp, #0x68]
	add r4, #0x7C
	str r4, [sp, #0x54]
	cmp r0, #0
	bne _0807DD62
	orr r1, r2
	strh r1, [r3]
_0807DCBE:
	mov r0, #0
	mov r1, r8
	str r0, [r1]
	ldr r6, [sp, #0x58]
	mov r0, #0x11
	strb r0, [r6, #0x14]
	mov r0, #0x22
	ldr r2, [sp, #0x5C]
	strb r0, [r2]
	mov r0, #0x44
	ldr r3, [sp, #0x64]
	strb r0, [r3]
	mov r0, #0x88
	ldr r4, [sp, #0x68]
	strb r0, [r4]
	mov r3, r8
	add r3, #0x98
	mov r2, r8
	add r2, #0xC8
	mov r0, r8
	add r0, #0xB0
	mov r1, #0x33
	ldr r4, [sp, #0x54]
	strb r1, [r4, #0x14]
	strb r1, [r0, #0x14]
	sub r0, #0x1C
	strb r1, [r0]
	strb r1, [r2, #0x14]
	strb r1, [r3, #0x14]
	sub r0, #0x18
	strb r1, [r0]
	ldr r1, _0807DD9C @ =0x04000081
	mov r0, #0xFF
	strb r0, [r1]
	add r1, #1
	ldr r2, _0807DDA0 @ =0x0000330E
	add r0, r2, #0
	strh r0, [r1]
	mov r4, r8
	add r4, #0x22
	mov r3, #0
	mov r0, #9
	mov r9, r0
_0807DD14:
	ldrb r0, [r6, #0x10]
	mov r2, #0xFE
	and r2, r0
	strb r2, [r6, #0x10]
	mov r0, #0x40
	and r0, r2
	cmp r0, #0
	beq _0807DD2C
	mov r1, #0x80
	add r0, r2, #0
	orr r0, r1
	strb r0, [r6, #0x10]
_0807DD2C:
	ldrb r1, [r6, #0x10]
	mov r0, #0xC0
	and r0, r1
	strb r0, [r6, #0x10]
	strh r3, [r6, #6]
	strh r3, [r6, #8]
	strh r3, [r6, #2]
	strh r3, [r6, #0xE]
	add r6, #0x18
	mov r1, #1
	neg r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bge _0807DD14
	mov r7, #0
	mov r0, #0x80
	strb r0, [r4]
	mov r3, r8
	strb r0, [r3, #0xA]
	ldr r0, _0807DDA4 @ =0x04000072
	strh r7, [r0]
	mov r0, r8
	mov r1, #0
	mov r2, #0
	bl sub_0807D518
_0807DD62:
	mov r4, r8
	ldr r0, [r4]
	add r0, #1
	str r0, [r4]
	ldr r7, [sp, #0x60]
	ldr r6, [sp, #0x54]
	mov r0, #9
	mov r9, r0
	mov r1, #1
	mov sl, r1
_0807DD76:
	mov r2, #0
	str r2, [r7]
	str r2, [r7, #4]
	ldrb r1, [r6, #0x10]
	mov r0, #0x80
	and r0, r1
	cmp r0, #0
	bne _0807DD88
	b _0807DF8C
_0807DD88:
	mov r2, #1
	mov r0, sl
	and r0, r1
	cmp r0, #0
	bne _0807DDA8
	add r0, r1, #0
	orr r0, r2
	strb r0, [r6, #0x10]
	mov r3, #0
	b _0807DE70
_0807DD9C: .4byte 0x04000081
_0807DDA0: .4byte 0x0000330E
_0807DDA4: .4byte 0x04000072
_0807DDA8:
	ldrh r0, [r6, #0xC]
	sub r0, #1
	strh r0, [r6, #0xC]
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807DDB6
	b _0807DF8C
_0807DDB6:
	ldrh r3, [r6, #6]
	ldrh r1, [r6, #4]
_0807DDBA:
	ldr r4, [sp, #0x50]
	add r0, r4, r1
	add r2, r0, r3
	add r3, #1
	ldrb r4, [r2]
	cmp r4, #0xFC
	ble _0807DE06
	cmp r4, #0xFF
	bne _0807DDDC
	mov r2, #0xC4
	lsl r2, r2, #1
	add r2, r8
	ldrh r1, [r2]
	mov r0, sl
	orr r0, r1
	strh r0, [r2]
	b _0807DDF2
_0807DDDC:
	cmp r4, #0xFE
	bne _0807DDE2
	b _0807DCBE
_0807DDE2:
	ldrb r0, [r6, #0x10]
	mov r1, #0x40
	and r1, r0
	strb r1, [r6, #0x10]
	ldrh r0, [r7, #4]
	cmp r0, #0
	beq _0807DDF2
	b _0807E042
_0807DDF2:
	mov r0, #0
	strb r0, [r6, #3]
	mov r1, #0
	strh r1, [r7, #2]
	ldrh r0, [r6]
	strh r0, [r7]
	mov r0, #0x40
	strb r0, [r7, #5]
	strb r0, [r7, #4]
	b _0807E042
_0807DE06:
	cmp r4, #0xEF
	ble _0807DE6C
	cmp r4, #0xF3
	bne _0807DE16
	add r0, r1, r3
	strh r0, [r6, #4]
	mov r3, #0
	b _0807DF4C
_0807DE16:
	cmp r4, #0xF2
	bne _0807DE32
	add r3, #1
	mov r4, #0
	ldsh r0, [r6, r4]
	sub r0, #0x40
	ldrb r2, [r2, #1]
	add r5, r0, r2
	ldrb r0, [r6, #2]
	strh r0, [r7, #6]
	strh r5, [r7]
	mov r0, sl
	strb r0, [r7, #5]
	b _0807DF4C
_0807DE32:
	cmp r4, #0xF0
	ble _0807DE48
	add r3, #1
	ldrb r0, [r2, #1]
	lsr r0, r0, #1
	strb r0, [r6, #0xF]
	ldrb r0, [r6, #0x10]
	mov r1, #0x20
	orr r0, r1
	strb r0, [r6, #0x10]
	b _0807DF4C
_0807DE48:
	cmp r4, #0xF0
	beq _0807DE4E
	b _0807DF4C
_0807DE4E:
	add r3, #1
	ldrb r5, [r2, #1]
	mov r1, r9
	cmp r1, #3
	ble _0807DE68
	add r4, r5, #0
	mov r2, #0xF
	and r4, r2
	asr r0, r5, #4
	strb r0, [r7, #0xB]
	mov r0, sl
	strb r0, [r7, #0xC]
	b _0807DE7A
_0807DE68:
	strb r5, [r6, #0x14]
	b _0807DF4C
_0807DE6C:
	cmp r4, #0xDF
	ble _0807DE86
_0807DE70:
	mov r4, #0
	ldrh r0, [r6]
	strh r0, [r7]
	mov r0, #0x40
	strb r0, [r7, #5]
_0807DE7A:
	strb r4, [r6, #3]
	ldrb r0, [r6, #2]
	strb r0, [r7, #2]
	mov r1, sl
	strb r1, [r7, #4]
	b _0807DF4C
_0807DE86:
	cmp r4, #0xCF
	ble _0807DEA4
	mov r0, #0xF
	and r4, r0
	add r3, #1
	ldrb r0, [r2, #1]
	lsl r0, r0, #5
	strh r0, [r7]
	strh r0, [r6]
	mov r1, sl
	strb r1, [r7, #5]
	ldrb r2, [r6, #3]
	cmp r4, r2
	beq _0807DF4C
	b _0807DE7A
_0807DEA4:
	cmp r4, #0xBF
	ble _0807DEB6
	mov r0, #0xF
	and r4, r0
	ldrh r0, [r6]
	strh r0, [r7]
	mov r1, sl
	strb r1, [r7, #5]
	b _0807DE7A
_0807DEB6:
	cmp r4, #0x9F
	ble _0807DEE6
	add r0, r4, #0
	mov r1, #0xF
	and r0, r1
	strb r0, [r6, #3]
	ldrb r0, [r2, #1]
	strh r0, [r7, #6]
	strb r0, [r6, #2]
	add r3, #1
	mov r5, #0
	cmp r4, #0xAF
	ble _0807DED8
	mov r0, #2
	ldsb r0, [r2, r0]
	lsl r5, r0, #5
	add r3, #1
_0807DED8:
	strh r5, [r6]
	strh r5, [r7]
	ldrh r0, [r6, #2]
	strh r0, [r7, #2]
	mov r0, #0x80
	strb r0, [r7, #5]
	b _0807DF4C
_0807DEE6:
	cmp r4, #0x8F
	ble _0807DF2C
	strh r1, [r6, #0xA]
	add r0, r3, #3
	strh r0, [r6, #8]
	mov r0, #0xC7
	lsl r0, r0, #1
	add r0, r8
	mov r3, #0
	ldsh r1, [r0, r3]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r9
	add r5, r0, #2
	ldr r1, _0807DF28 @ =0x080E09D0
	lsl r0, r5, #1
	add r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r6, #4]
	ldrb r1, [r2, #3]
	strb r1, [r6, #0x13]
	ldrb r3, [r2, #1]
	ldrb r0, [r2, #2]
	lsl r0, r0, #8
	orr r3, r0
	mov r0, #0xF
	and r4, r0
	cmp r4, #0xF
	beq _0807DF64
	add r0, r1, #1
	strb r0, [r6, #0x13]
	b _0807DF34
_0807DF28: .4byte gUnk_080E09D0
_0807DF2C:
	cmp r4, #0x7F
	ble _0807DF4C
	mov r1, #0xF
	and r4, r1
_0807DF34:
	cmp r4, #3
	ble _0807DF48
	sub r4, #4
	ldrb r2, [r6, #3]
	mov r0, r8
	add r1, r4, #0
	str r3, [sp, #0x6C]
	bl sub_0807D518
	ldr r3, [sp, #0x6C]
_0807DF48:
	strb r4, [r7, #2]
	strb r4, [r6, #2]
_0807DF4C:
	ldrh r0, [r6, #8]
	cmp r0, #0
	beq _0807DF64
	ldrb r0, [r6, #0x13]
	sub r0, #1
	strb r0, [r6, #0x13]
	lsl r0, r0, #0x18
	cmp r0, #0
	bne _0807DF64
	ldrh r0, [r6, #0xA]
	strh r0, [r6, #4]
	ldrh r3, [r6, #8]
_0807DF64:
	ldrh r1, [r6, #4]
	ldr r2, [sp, #0x50]
	add r0, r2, r1
	add r2, r0, r3
	ldrb r4, [r2]
	add r3, #1
	cmp r4, #0xEF
	ble _0807DF80
	mov r0, #0xF
	and r0, r4
	lsl r4, r0, #8
	ldrb r0, [r2, #1]
	add r4, r4, r0
	add r3, #1
_0807DF80:
	strh r3, [r6, #6]
	strh r4, [r6, #0xC]
	lsl r0, r4, #0x10
	cmp r0, #0
	bne _0807DF8C
	b _0807DDBA
_0807DF8C:
	ldrb r5, [r6, #0x10]
	mov r0, #4
	and r0, r5
	cmp r0, #0
	beq _0807DFC2
	mov r0, #0x11
	ldsb r0, [r6, r0]
	sub r4, r0, #1
	cmp r4, #0
	bgt _0807DFAC
	mov r4, #0
	mov r0, #0xFB
	and r0, r5
	strb r0, [r6, #0x10]
	mov r0, #0x40
	strb r0, [r7, #5]
_0807DFAC:
	strb r4, [r6, #0x11]
	ldrb r0, [r6, #0x12]
	mul r0, r4
	asr r0, r0, #2
	strb r0, [r6, #3]
	ldrh r0, [r6]
	strh r0, [r7]
	ldrh r0, [r6, #2]
	strh r0, [r7, #2]
	mov r3, sl
	strb r3, [r7, #4]
_0807DFC2:
	mov r0, #0x20
	and r5, r0
	cmp r5, #0
	beq _0807E008
	ldrb r2, [r6, #0xF]
	cmp r2, #0
	bne _0807DFDE
	ldrb r1, [r6, #0x10]
	mov r0, #0xDF
	and r0, r1
	strb r0, [r6, #0x10]
	strb r2, [r6, #0xE]
	mov r4, #0
	b _0807DFF6
_0807DFDE:
	ldrb r0, [r6, #0xE]
	add r0, #0x18
	strb r0, [r6, #0xE]
	ldr r1, _0807E104 @ =0x081ABC4C
	ldrb r0, [r6, #0xE]
	lsl r0, r0, #1
	add r0, r0, r1
	mov r4, #0
	ldsh r1, [r0, r4]
	ldrb r0, [r6, #0xF]
	mul r0, r1
	asr r4, r0, #0xC
_0807DFF6:
	ldrh r0, [r6]
	add r0, r0, r4
	strh r0, [r7]
	ldrh r0, [r6, #2]
	strh r0, [r7, #2]
	ldrb r0, [r6, #2]
	strh r0, [r7, #6]
	mov r0, sl
	strb r0, [r7, #5]
_0807E008:
	ldrb r1, [r6, #3]
	ldr r0, _0807E108 @ =0x00000191
	add r0, r8
	ldrb r0, [r0]
	mul r0, r1
	asr r0, r0, #4
	strb r0, [r7, #3]
	mov r0, #0xC4
	lsl r0, r0, #1
	add r0, r8
	ldrh r1, [r0]
	mov r2, #0x81
	lsl r2, r2, #3
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _0807E042
	ldrb r0, [r6, #3]
	cmp r0, #0
	beq _0807E042
	ldrb r0, [r7, #5]
	cmp r0, #0
	bne _0807E03A
	ldrh r0, [r6]
	strh r0, [r7]
_0807E03A:
	ldrb r0, [r6, #2]
	strb r0, [r7, #2]
	mov r3, sl
	strb r3, [r7, #4]
_0807E042:
	mov r4, #1
	neg r4, r4
	add r9, r4
	sub r6, #0x18
	sub r7, #8
	mov r0, r9
	cmp r0, #0
	blt _0807E054
	b _0807DD76
_0807E054:
	ldr r1, [sp, #0x58]
	ldrb r0, [r1, #0x14]
	ldr r2, [sp, #0x5C]
	ldrb r1, [r2]
	orr r0, r1
	ldr r3, [sp, #0x64]
	ldrb r1, [r3]
	orr r0, r1
	ldr r4, [sp, #0x68]
	ldrb r1, [r4]
	orr r0, r1
	mov r1, #0xCC
	lsl r1, r1, #1
	add r1, r8
	strb r0, [r1]
_0807E072:
	mov r0, #0xC4
	lsl r0, r0, #1
	add r0, r8
	ldrh r1, [r0]
	mov r0, #0x40
	and r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0
	beq _0807E114
	ldr r1, _0807E10C @ =0x00000195
	add r1, r8
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	cmp r0, #0
	bge _0807E09A
	mov r0, #0
	strb r0, [r1]
_0807E09A:
	mov r4, r8
	add r4, #0xF8
	add r1, sp, #8
	mov r0, #0
	bl sub_0807D6B4
	add r1, sp, #0x18
	mov r0, #1
	bl sub_0807D6B4
	add r1, sp, #0x48
	mov r0, #2
	bl sub_0807D6B4
	add r1, sp, #0x40
	mov r0, #3
	bl sub_0807D6B4
	add r1, sp, #0x38
	mov r0, #4
	bl sub_0807D6B4
	add r1, sp, #0x30
	mov r0, #5
	bl sub_0807D6B4
	mov r5, #0
	mov r0, #5
	mov r9, r0
_0807E0D4:
	ldrb r0, [r4, #0x13]
	orr r5, r0
	add r4, #0x18
	mov r1, #1
	neg r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bge _0807E0D4
	mov r1, #0x80
	and r1, r5
	cmp r1, #0
	bne _0807E11A
	ldr r0, _0807E10C @ =0x00000195
	add r0, r8
	strb r1, [r0]
	mov r2, #0xC4
	lsl r2, r2, #1
	add r2, r8
	ldrh r1, [r2]
	ldr r0, _0807E110 @ =0x0000FFBF
	and r0, r1
	strh r0, [r2]
	b _0807E11A
_0807E104: .4byte gUnk_081ABC4C
_0807E108: .4byte 0x00000191
_0807E10C: .4byte 0x00000195
_0807E110: .4byte 0x0000FFBF
_0807E114:
	ldr r1, _0807E154 @ =0x00000195
	add r1, r8
	strb r0, [r1]
_0807E11A:
	ldr r1, _0807E158 @ =0x04000081
	mov r0, #0xCC
	lsl r0, r0, #1
	add r0, r8
	ldrb r0, [r0]
	strb r0, [r1]
	mov r7, sp
	ldrh r0, [r7, #4]
	cmp r0, #0
	beq _0807E172
	ldr r0, _0807E15C @ =0x081AA20C
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r0
	ldrh r5, [r1]
	ldrb r0, [r7, #4]
	cmp r0, #0
	beq _0807E168
	ldrb r0, [r7, #3]
	lsl r0, r0, #0xC
	ldrb r1, [r7, #2]
	orr r0, r1
	ldr r1, _0807E160 @ =0x04000062
	strh r0, [r1]
	ldr r0, _0807E164 @ =0x04000064
	strh r5, [r0]
	b _0807E172
	.align 2, 0
_0807E154: .4byte 0x00000195
_0807E158: .4byte 0x04000081
_0807E15C: .4byte gUnk_081AA20C
_0807E160: .4byte 0x04000062
_0807E164: .4byte 0x04000064
_0807E168:
	ldr r1, _0807E194 @ =0x04000064
	ldr r4, _0807E198 @ =0x000007FF
	add r0, r4, #0
	and r5, r0
	strh r5, [r1]
_0807E172:
	ldrh r5, [r7, #0xC]
	cmp r5, #0
	beq _0807E1DA
	mov r0, #8
	ldsh r5, [r7, r0]
	cmp r5, #0
	blt _0807E18A
	mov r0, #0x80
	lsl r0, r0, #7
	and r0, r5
	cmp r0, #0
	bne _0807E1A0
_0807E18A:
	ldr r1, _0807E19C @ =0x081AA20C
	lsl r0, r5, #1
	add r0, r0, r1
	ldrh r5, [r0]
	b _0807E1AA
_0807E194: .4byte 0x04000064
_0807E198: .4byte 0x000007FF
_0807E19C: .4byte gUnk_081AA20C
_0807E1A0:
	ldr r0, _0807E1C4 @ =0xFFFFBFFF
	and r5, r0
	mov r0, #0x80
	lsl r0, r0, #8
	orr r5, r0
_0807E1AA:
	ldrb r0, [r7, #0xC]
	cmp r0, #0
	beq _0807E1D0
	ldrb r0, [r7, #0xB]
	lsl r0, r0, #0xC
	ldrb r1, [r7, #0xA]
	orr r0, r1
	ldr r1, _0807E1C8 @ =0x04000068
	strh r0, [r1]
	ldr r0, _0807E1CC @ =0x0400006C
	strh r5, [r0]
	b _0807E1DA
	.align 2, 0
_0807E1C4: .4byte 0xFFFFBFFF
_0807E1C8: .4byte 0x04000068
_0807E1CC: .4byte 0x0400006C
_0807E1D0:
	ldr r1, _0807E1FC @ =0x0400006C
	ldr r2, _0807E200 @ =0x000007FF
	add r0, r2, #0
	and r5, r0
	strh r5, [r1]
_0807E1DA:
	ldrh r0, [r7, #0x14]
	cmp r0, #0
	beq _0807E224
	ldr r0, _0807E204 @ =0x081AA20C
	mov r3, #0x10
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r0
	ldrh r0, [r1]
	ldr r5, _0807E200 @ =0x000007FF
	and r5, r0
	ldrb r1, [r7, #0x13]
	cmp r1, #0
	bne _0807E20C
	ldr r0, _0807E208 @ =0x04000072
	strh r1, [r0]
	b _0807E220
_0807E1FC: .4byte 0x0400006C
_0807E200: .4byte 0x000007FF
_0807E204: .4byte gUnk_081AA20C
_0807E208: .4byte 0x04000072
_0807E20C:
	ldrb r1, [r7, #0x12]
	ldrb r2, [r7, #0x13]
	mov r0, r8
	bl sub_0807D518
	ldr r1, _0807E24C @ =0x04000072
	mov r4, #0x80
	lsl r4, r4, #6
	add r0, r4, #0
	strh r0, [r1]
_0807E220:
	ldr r0, _0807E250 @ =0x04000074
	strh r5, [r0]
_0807E224:
	ldrh r5, [r7, #0x1C]
	cmp r5, #0
	beq _0807E26E
	ldrb r0, [r7, #0x1B]
	lsl r4, r0, #0xC
	ldr r0, _0807E254 @ =0x00000202
	and r5, r0
	cmp r5, #0
	bne _0807E264
	ldr r0, _0807E258 @ =0x04000078
	strh r4, [r0]
	ldr r2, _0807E25C @ =0x0400007C
	ldr r1, _0807E260 @ =0x08139F50
	mov r3, #0x18
	ldsh r0, [r7, r3]
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2]
	b _0807E26E
_0807E24C: .4byte 0x04000072
_0807E250: .4byte 0x04000074
_0807E254: .4byte 0x00000202
_0807E258: .4byte 0x04000078
_0807E25C: .4byte 0x0400007C
_0807E260: .4byte gUnk_08139F50
_0807E264:
	ldr r0, _0807E298 @ =0x04000078
	strh r4, [r0]
	ldr r1, _0807E29C @ =0x0400007C
	ldrh r0, [r7, #0x18]
	strh r0, [r1]
_0807E26E:
	add r7, #0x48
	ldr r4, _0807E2A0 @ =0x030053FC
	mov r0, #5
	mov r9, r0
	ldr r6, _0807E2A4 @ =0x08088A20
_0807E278:
	ldrb r5, [r7, #5]
	cmp r5, #0
	beq _0807E2F2
	mov r1, #0x80
	and r1, r5
	cmp r1, #0
	beq _0807E2A8
	ldrh r1, [r7, #6]
	ldrb r2, [r7, #3]
	mov r0, #0
	ldsh r3, [r7, r0]
	add r0, r4, #0
	bl sub_0807E918
	b _0807E2F2
	.align 2, 0
_0807E298: .4byte 0x04000078
_0807E29C: .4byte 0x0400007C
_0807E2A0: .4byte 0x030053FC
_0807E2A4: .4byte gUnk_08088A20
_0807E2A8:
	mov r0, #0x40
	and r5, r0
	cmp r5, #0
	beq _0807E2B4
	strb r1, [r4, #0xE]
	b _0807E2F2
_0807E2B4:
	ldrh r1, [r7, #6]
	mov r2, #0x80
	lsl r2, r2, #8
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _0807E2D4
	ldr r3, _0807E2D0 @ =0x00003FFF
	add r0, r3, #0
	and r0, r1
	lsl r0, r0, #2
	add r0, r0, r6
	b _0807E2DC
	.align 2, 0
_0807E2D0: .4byte 0x00003FFF
_0807E2D4:
	ldr r1, _0807E31C @ =0x0811B420
	ldrh r0, [r7, #6]
	lsl r0, r0, #2
	add r0, r0, r1
_0807E2DC:
	ldr r2, [r0]
	mov r1, #0
	ldsh r0, [r7, r1]
	lsl r0, r0, #1
	ldr r1, _0807E320 @ =0x081A960C
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, [r2]
	mul r0, r1
	asr r0, r0, #0xC
	strh r0, [r4, #8]
_0807E2F2:
	ldrb r0, [r7, #4]
	cmp r0, #0
	beq _0807E2FC
	ldrb r0, [r7, #3]
	strb r0, [r4, #0xF]
_0807E2FC:
	sub r4, #0x10
	sub r7, #8
	mov r2, #1
	neg r2, r2
	add r9, r2
	mov r3, r9
	cmp r3, #0
	bge _0807E278
_0807E30C:
	add sp, #0x70
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0807E31C: .4byte gUnk_0811B420
_0807E320: .4byte gUnk_081A960C
	thumb_func_end sub_0807DB58

