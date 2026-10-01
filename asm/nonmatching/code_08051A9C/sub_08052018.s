	thumb_func_start sub_08052018
sub_08052018: @ 0x08052018
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	ldr r0, _0805208C @ =0x020192E0
	ldr r1, _08052090 @ =0x00001B62
	add r4, r0, r1
	ldrb r2, [r4]
	add r7, r0, #0
	cmp r2, #0
	beq _0805209C
	cmp r2, #1
	bne _08052038
	b _08052144
_08052038:
	ldr r1, _08052094 @ =0x0201D810
	ldrb r2, [r1, #5]
	lsl r4, r2, #0x1E
	lsr r0, r4, #0x1E
	ldrh r3, [r1, #6]
	add r0, r0, r3
	lsl r0, r0, #2
	add r6, r1, #0
	add r6, #0xC
	add r5, r0, r6
	ldr r0, _08052098 @ =0x00001B12
	add r1, r7, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805205C
	b _08052170
_0805205C:
	lsr r2, r4, #0x1E
	add r2, r2, r3
	lsl r2, r2, #2
	add r2, r2, r6
	lsr r0, r4, #0x1E
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #4
	mov r0, #0x11
	neg r0, r0
	ldrb r3, [r2, #1]
	and r0, r3
	orr r0, r1
	strb r0, [r2, #1]
	b _08052170
	.align 2, 0
_0805208C: .4byte 0x020192E0
_08052090: .4byte 0x00001B62
_08052094: .4byte 0x0201D810
_08052098: .4byte 0x00001B12
_0805209C:
	cmp r5, #0
	beq _080520B4
	add r0, r3, #0
	bl sub_08056ECC
	lsl r0, r0, #2
	ldr r1, _080520B0 @ =0x0201D81C
	add r5, r0, r1
	b _08052170
	.align 2, 0
_080520B0: .4byte 0x0201D81C
_080520B4:
	ldr r0, _080520D4 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _080520D8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080520DC @ =0x00000487
	cmp r1, r0
	beq _08052100
	cmp r1, r0
	bgt _080520E0
	sub r0, #0x2B
	cmp r1, r0
	beq _080520EA
	b _08052180
	.align 2, 0
_080520D4: .4byte 0x000007FF
_080520D8: .4byte gUnk_08622AB4
_080520DC: .4byte 0x00000487
_080520E0:
	mov r0, #0xBE
	lsl r0, r0, #3
	cmp r1, r0
	beq _08052114
	b _08052180
_080520EA:
	ldr r0, _080520F4 @ =0x00000206
	ldr r1, _080520F8 @ =0x00000712
	ldr r3, _080520FC @ =0x08086058
	b _0805211A
	.align 2, 0
_080520F4: .4byte 0x00000206
_080520F8: .4byte 0x00000712
_080520FC: .4byte gUnk_08086058
_08052100:
	ldr r0, _08052108 @ =0x00000206
	ldr r1, _0805210C @ =0x00000712
	ldr r3, _08052110 @ =0x080860A4
	b _0805211A
_08052108: .4byte 0x00000206
_0805210C: .4byte 0x00000712
_08052110: .4byte gUnk_080860A4
_08052114:
	ldr r0, _08052130 @ =0x00000206
	ldr r1, _08052134 @ =0x00000712
	ldr r3, _08052138 @ =0x080860FC
_0805211A:
	mov r2, #0xB
	bl sub_080602A4
	ldr r0, _0805213C @ =0x020192E0
	ldr r2, _08052140 @ =0x00001B62
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	mov r0, #0
	b _08052182
_08052130: .4byte 0x00000206
_08052134: .4byte 0x00000712
_08052138: .4byte gUnk_080860FC
_0805213C: .4byte 0x020192E0
_08052140: .4byte 0x00001B62
_08052144:
	mov r1, #1
	neg r1, r1
	ldr r0, _08052168 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r3, _0805216C @ =0x08622AB4
	add r0, r0, r3
	ldrh r2, [r0]
	add r0, r5, #0
	add r3, r6, #0
	bl sub_0802AF34
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
	b _08052182
	.align 2, 0
_08052168: .4byte 0x000007FF
_0805216C: .4byte gUnk_08622AB4
_08052170:
	ldr r1, [r5]
	ldr r2, _08052188 @ =0x00001B64
	add r0, r7, r2
	strh r1, [r0]
	ldrh r1, [r5, #2]
	ldr r3, _0805218C @ =0x00001B66
	add r0, r7, r3
	strh r1, [r0]
_08052180:
	mov r0, #1
_08052182:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08052188: .4byte 0x00001B64
_0805218C: .4byte 0x00001B66
	thumb_func_end sub_08052018

