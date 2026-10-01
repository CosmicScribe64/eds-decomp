	thumb_func_start sub_0803F034
sub_0803F034: @ 0x0803F034
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r6, r0, #0
	add r2, r1, #0
	mov r0, #1
	ldrb r1, [r6, #2]
	and r0, r1
	cmp r0, #0
	beq _0803F144
	mov r0, #8
	neg r0, r0
	ldrb r3, [r6, #0xA]
	and r0, r3
	strb r0, [r6, #0xA]
	add r0, r6, #0
	add r1, r2, #0
	mov r2, #0
	bl sub_0802CE38
	cmp r0, #0
	bne _0803F068
	b _0803F32C
_0803F068:
	mov r0, #0
	bl sub_08008860
	cmp r0, #0
	bgt _0803F074
	b _0803F32C
_0803F074:
	mov r7, #0
	ldr r0, _0803F134 @ =0x0000FFFF
	mov sl, r0
_0803F07A:
	ldr r5, _0803F134 @ =0x0000FFFF
	ldr r0, _0803F138 @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #0x80
	lsl r1, r1, #2
	and r0, r1
	cmp r0, #0
	beq _0803F0EA
	mov r4, #0
	ldr r1, _0803F13C @ =0x0201A070
	mov ip, r1
	mov r0, #0x94
	mul r0, r7
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r9, r0
	mov r3, ip
	mov r2, #1
	mov r8, r2
_0803F0A4:
	mov r0, r9
	cmp r0, #0
	beq _0803F0E2
	ldr r0, [r3]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0803F140 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0x14
	bgt _0803F0E2
	cmp r0, #0x10
	blt _0803F0E2
	lsl r2, r4, #0x18
	cmp r7, #0
	beq _0803F0D4
	ldrh r0, [r6, #0xC]
	str r0, [sp, #0]
	lsr r0, r2, #0x10
	mov r1, r8
	orr r0, r1
	ldr r1, [sp, #0]
	cmp r1, r0
	beq _0803F0E2
_0803F0D4:
	lsr r5, r2, #0x10
	mov r2, r8
	orr r5, r2
	mov r3, #0xB9
	lsl r3, r3, #2
	add r3, ip
	mov r4, #5
_0803F0E2:
	add r3, #0x94
	add r4, #1
	cmp r4, #4
	ble _0803F0A4
_0803F0EA:
	cmp r5, sl
	bne _0803F10A
	mov r1, #1
	neg r1, r1
	cmp r7, #0
	ble _0803F0F8
	mov r1, #0
_0803F0F8:
	mov r0, #0
	mov r2, #1
	mov r3, #1
	bl sub_0805748C
	cmp r0, #0
	blt _0803F10A
	lsl r0, r0, #0x18
	lsr r5, r0, #0x10
_0803F10A:
	cmp r7, #0
	ble _0803F116
	ldrh r3, [r6, #0xC]
	cmp r5, r3
	bne _0803F116
	ldr r5, _0803F134 @ =0x0000FFFF
_0803F116:
	cmp r5, sl
	bne _0803F11C
	b _0803F32C
_0803F11C:
	lsl r1, r5, #0x18
	lsr r1, r1, #0x18
	lsr r2, r5, #8
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	add r0, r6, #0
	bl sub_0803DDAC
	add r7, #1
	cmp r7, #1
	ble _0803F07A
	b _0803F32C
_0803F134: .4byte 0x0000FFFF
_0803F138: .4byte 0x02015EE8
_0803F13C: .4byte 0x0201A070
_0803F140: .4byte gUnk_08622AB4
_0803F144:
	ldr r1, _0803F160 @ =0x02017A40
	ldr r3, _0803F164 @ =0x000003E5
	add r0, r1, r3
	ldrb r0, [r0]
	add r3, r1, #0
	cmp r0, #5
	bls _0803F154
	b _0803F330
_0803F154:
	lsl r0, r0, #2
	ldr r1, _0803F168 @ =0x0803F16C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803F160: .4byte 0x02017A40
_0803F164: .4byte 0x000003E5
_0803F168: .4byte 0x0803F16C
_0803F16C:
	.4byte _0803F184
	.4byte _0803F1EC
	.4byte _0803F224
	.4byte _0803F2A4
	.4byte _0803F1EC
	.4byte _0803F2B8
_0803F184:
	mov r0, #8
	neg r0, r0
	ldrb r1, [r6, #0xA]
	and r0, r1
	strb r0, [r6, #0xA]
	add r0, r6, #0
	add r1, r2, #0
	mov r2, #0
	bl sub_0802CE38
	cmp r0, #0
	bne _0803F19E
	b _0803F32C
_0803F19E:
	mov r0, #0
	bl sub_08008860
	add r4, r0, #0
	mov r0, #1
	bl sub_08008860
	cmn r4, r0
	bne _0803F1B2
	b _0803F32C
_0803F1B2:
	ldr r0, _0803F1D8 @ =0x00000206
	ldr r1, _0803F1DC @ =0x00000712
	ldr r3, _0803F1E0 @ =0x0808410C
_0803F1B8:
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r0, _0803F1E4 @ =0x02017A40
	ldr r2, _0803F1E8 @ =0x000003E5
	add r0, r0, r2
_0803F1CE:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803F330
	.align 2, 0
_0803F1D8: .4byte 0x00000206
_0803F1DC: .4byte 0x00000712
_0803F1E0: .4byte gUnk_0808410C
_0803F1E4: .4byte 0x02017A40
_0803F1E8: .4byte 0x000003E5
_0803F1EC:
	ldr r0, _0803F20C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0803F1F6
	b _0803F32C
_0803F1F6:
	ldr r0, _0803F210 @ =0x00000206
	ldr r1, _0803F214 @ =0x00000712
	ldr r3, _0803F218 @ =0x0808413C
	mov r2, #0xB
	bl sub_080602A4
	ldr r0, _0803F21C @ =0x02017A40
	ldr r3, _0803F220 @ =0x000003E5
	add r0, r0, r3
	b _0803F1CE
	.align 2, 0
_0803F20C: .4byte 0x0201AE60
_0803F210: .4byte 0x00000206
_0803F214: .4byte 0x00000712
_0803F218: .4byte gUnk_0808413C
_0803F21C: .4byte 0x02017A40
_0803F220: .4byte 0x000003E5
_0803F224:
	ldr r1, _0803F288 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803F2C4
	ldr r0, _0803F28C @ =0x00F000F0
	bl sub_08052F38
	cmp r0, #0
	bne _0803F23C
	b _0803F330
_0803F23C:
	ldr r0, _0803F290 @ =0x0201CFB0
	ldr r2, _0803F294 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803F298 @ =0x00000828
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r6, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803F27E
	mov r0, #0
	bl sub_08008860
	add r4, r0, #0
	mov r0, #1
	bl sub_08008860
	add r4, r4, r0
	cmp r4, #1
	beq _0803F32C
	ldr r0, _0803F29C @ =0x02017A40
	ldr r1, _0803F2A0 @ =0x000003E5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_0803F27E:
	mov r0, #3
	bl sub_08077AEC
	b _0803F330
	.align 2, 0
_0803F288: .4byte 0x03000040
_0803F28C: .4byte 0x00F000F0
_0803F290: .4byte 0x0201CFB0
_0803F294: .4byte 0x00000824
_0803F298: .4byte 0x00000828
_0803F29C: .4byte 0x02017A40
_0803F2A0: .4byte 0x000003E5
_0803F2A4:
	ldr r0, _0803F2AC @ =0x00000206
	ldr r1, _0803F2B0 @ =0x00000712
	ldr r3, _0803F2B4 @ =0x08084178
	b _0803F1B8
_0803F2AC: .4byte 0x00000206
_0803F2B0: .4byte 0x00000712
_0803F2B4: .4byte gUnk_08084178
_0803F2B8:
	ldr r1, _0803F2D0 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803F2D8
_0803F2C4:
	ldr r0, _0803F2D4 @ =0x000003E5
	add r1, r3, r0
	mov r0, #0
	strb r0, [r1]
	b _0803F332
	.align 2, 0
_0803F2D0: .4byte 0x03000040
_0803F2D4: .4byte 0x000003E5
_0803F2D8:
	ldr r0, _0803F31C @ =0x00F000F0
	bl sub_08052F38
	cmp r0, #0
	beq _0803F330
	ldr r0, _0803F320 @ =0x0201CFB0
	ldr r1, _0803F324 @ =0x00000824
	add r2, r0, r1
	ldr r3, _0803F328 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	ldr r1, [r2]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x10
	ldrb r2, [r2]
	orr r0, r2
	ldrh r2, [r6, #0xC]
	cmp r2, r0
	beq _0803F314
	add r0, r6, #0
	add r2, r3, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803F32C
_0803F314:
	mov r0, #3
	bl sub_08077AEC
	b _0803F330
_0803F31C: .4byte 0x00F000F0
_0803F320: .4byte 0x0201CFB0
_0803F324: .4byte 0x00000824
_0803F328: .4byte 0x00000828
_0803F32C:
	mov r0, #1
	b _0803F332
_0803F330:
	mov r0, #0
_0803F332:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803F034
	.align 2, 0

