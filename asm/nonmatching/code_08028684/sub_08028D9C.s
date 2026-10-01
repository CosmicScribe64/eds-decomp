	thumb_func_start sub_08028D9C
sub_08028D9C: @ 0x08028D9C
	push {r4, r5, r6, lr}
	ldr r6, _08028DE4 @ =0x02020310
	ldr r0, _08028DE8 @ =0x00000B0D
	add r2, r6, r0
	ldrb r0, [r2]
	add r3, r6, #0
	cmp r0, #0
	beq _08028DAE
	b _08028F02
_08028DAE:
	ldr r1, _08028DEC @ =0x03000040
	mov r4, #1
	add r0, r4, #0
	ldrh r5, [r1, #6]
	and r0, r5
	cmp r0, #0
	beq _08028EAC
	ldr r5, _08028DF0 @ =0x00000AAD
	add r0, r6, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	cmp r0, #0
	bne _08028EAC
	ldr r1, _08028DF4 @ =0x00000B0E
	add r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #1
	bne _08028DF8
	mov r0, #1
	strb r0, [r2]
	mov r2, #0xB1
	lsl r2, r2, #4
	add r0, r6, r2
	bl sub_0807BCF4
	b _08028E86
_08028DE4: .4byte 0x02020310
_08028DE8: .4byte 0x00000B0D
_08028DEC: .4byte 0x03000040
_08028DF0: .4byte 0x00000AAD
_08028DF4: .4byte 0x00000B0E
_08028DF8:
	bl sub_08076F9C
	mov r1, #0xFA
	lsl r1, r1, #1
	bl __modsi3
	cmp r0, #0x63
	bgt _08028E20
	ldr r5, _08028E18 @ =0x00000AAE
	add r0, r6, r5
	ldrb r1, [r0]
	ldr r2, _08028E1C @ =0x00000ABC
	add r0, r6, r2
	strb r1, [r0]
	b _08028E4C
	.align 2, 0
_08028E18: .4byte 0x00000AAE
_08028E1C: .4byte 0x00000ABC
_08028E20:
	bl sub_08076F9C
	and r0, r4
	cmp r0, #0
	beq _08028E38
	ldr r5, _08028E34 @ =0x00000AAE
	add r0, r6, r5
	ldrb r0, [r0]
	add r0, #1
	b _08028E40
_08028E34: .4byte 0x00000AAE
_08028E38:
	ldr r5, _08028E90 @ =0x00000AAE
	add r0, r6, r5
	ldrb r0, [r0]
	add r0, #2
_08028E40:
	mov r1, #3
	bl __modsi3
	ldr r2, _08028E94 @ =0x00000ABC
	add r1, r6, r2
	strb r0, [r1]
_08028E4C:
	ldr r4, _08028E98 @ =0x02020310
	ldr r5, _08028E9C @ =0x00000ABD
	add r1, r4, r5
	mov r5, #0
	mov r0, #1
	strb r0, [r1]
	ldr r1, _08028E90 @ =0x00000AAE
	add r0, r4, r1
	ldrb r0, [r0]
	ldr r2, _08028E94 @ =0x00000ABC
	add r1, r4, r2
	ldrb r1, [r1]
	bl sub_08028930
	ldr r2, _08028EA0 @ =0x00000ABE
	add r1, r4, r2
	strb r0, [r1]
	ldr r0, _08028EA4 @ =0x00000AF5
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	sub r2, #0xD
	add r1, r4, r2
	mov r0, #4
	strb r0, [r1]
	ldr r0, _08028EA8 @ =0x00000B1E
	add r4, r4, r0
	strb r5, [r4]
_08028E86:
	mov r0, #1
	bl sub_08077AEC
	b _08028EF6
	.align 2, 0
_08028E90: .4byte 0x00000AAE
_08028E94: .4byte 0x00000ABC
_08028E98: .4byte 0x02020310
_08028E9C: .4byte 0x00000ABD
_08028EA0: .4byte 0x00000ABE
_08028EA4: .4byte 0x00000AF5
_08028EA8: .4byte 0x00000B1E
_08028EAC:
	mov r0, #0x20
	ldrh r2, [r1, #6]
	and r0, r2
	cmp r0, #0
	beq _08028ED4
	ldr r5, _08028ED0 @ =0x00000AAD
	add r2, r3, r5
	mov r0, #0
	ldsb r0, [r2, r0]
	cmp r0, #0
	bne _08028ED4
	mov r0, #0xFC
	strb r0, [r2]
	mov r0, #0
	bl sub_08077AEC
	b _08028EF6
	.align 2, 0
_08028ED0: .4byte 0x00000AAD
_08028ED4:
	mov r0, #0x10
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08028EF6
	ldr r0, _08028F70 @ =0x02020310
	ldr r2, _08028F74 @ =0x00000AAD
	add r1, r0, r2
	mov r0, #0
	ldsb r0, [r1, r0]
	cmp r0, #0
	bne _08028EF6
	mov r0, #4
	strb r0, [r1]
	mov r0, #0
	bl sub_08077AEC
_08028EF6:
	ldr r0, _08028F70 @ =0x02020310
	ldr r5, _08028F78 @ =0x00000B0D
	add r0, r0, r5
	ldrb r0, [r0]
	cmp r0, #0
	beq _08028FA0
_08028F02:
	ldr r1, _08028F7C @ =0x00000AAE
	add r0, r6, r1
	ldrb r1, [r0]
	mov r2, #0xB1
	lsl r2, r2, #4
	add r6, r6, r2
	mov r0, #0x51
	add r2, r6, #0
	bl sub_0807BCFC
	cmp r0, #0
	beq _08028F9C
	ldr r4, _08028F70 @ =0x02020310
	ldr r5, _08028F80 @ =0x00000AB1
	add r1, r4, r5
	mov r5, #0
	mov r0, #4
	strb r0, [r1]
	ldr r0, _08028F84 @ =0x00000ABD
	add r1, r4, r0
	mov r0, #1
	strb r0, [r1]
	ldr r1, _08028F88 @ =0x00000B16
	add r0, r4, r1
	ldrh r1, [r0]
	ldr r2, _08028F8C @ =0x00000ABC
	add r0, r4, r2
	strb r1, [r0]
	sub r2, #0xE
	add r0, r4, r2
	ldrb r0, [r0]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl sub_08028930
	ldr r2, _08028F90 @ =0x00000ABE
	add r1, r4, r2
	strb r0, [r1]
	ldr r0, _08028F94 @ =0x00000AF5
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	ldr r1, _08028F78 @ =0x00000B0D
	add r0, r4, r1
	strb r5, [r0]
	add r0, r6, #0
	bl sub_0807BCF4
	bl sub_08027CA4
	ldr r2, _08028F98 @ =0x00000B1E
	add r4, r4, r2
	strb r5, [r4]
	b _08028FA0
_08028F70: .4byte 0x02020310
_08028F74: .4byte 0x00000AAD
_08028F78: .4byte 0x00000B0D
_08028F7C: .4byte 0x00000AAE
_08028F80: .4byte 0x00000AB1
_08028F84: .4byte 0x00000ABD
_08028F88: .4byte 0x00000B16
_08028F8C: .4byte 0x00000ABC
_08028F90: .4byte 0x00000ABE
_08028F94: .4byte 0x00000AF5
_08028F98: .4byte 0x00000B1E
_08028F9C:
	bl sub_08027C90
_08028FA0:
	ldr r1, _08028FC8 @ =0x02020310
	ldr r5, _08028FCC @ =0x00000AB1
	add r0, r1, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	cmp r0, #0
	bne _08028FBE
	mov r2, #0xAC
	lsl r2, r2, #4
	add r0, r1, r2
	ldrh r1, [r0]
	mov r0, #0
	bl sub_080280D0
_08028FBE:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08028FC8: .4byte 0x02020310
_08028FCC: .4byte 0x00000AB1
	thumb_func_end sub_08028D9C

