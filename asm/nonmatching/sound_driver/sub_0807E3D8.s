	thumb_func_start sub_0807E3D8
sub_0807E3D8: @ 0x0807E3D8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r0, _0807E534 @ =0x03005210
	mov ip, r0
	mov r0, #0xC6
	lsl r0, r0, #1
	add r0, ip
	mov r1, #0
	ldsh r2, [r0, r1]
	cmp r2, #0
	bge _0807E3F8
	b _0807E524
_0807E3F8:
	ldr r0, _0807E538 @ =0x00000FFF
	and r0, r2
	lsl r1, r0, #3
	sub r1, r1, r0
	lsl r1, r1, #2
	ldr r0, _0807E53C @ =0x08087FD0
	add r1, r1, r0
	mov sl, r1
	ldr r1, [r1, #0x18]
	mov r2, #0xFF
	mov r9, r1
	mov r0, r9
	and r0, r2
	mov r9, r0
	asr r0, r1, #0x10
	cmp r0, #0
	beq _0807E428
	ldr r0, _0807E540 @ =0x00000195
	add r0, ip
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	cmp r0, #0
	bne _0807E51A
_0807E428:
	asr r5, r1, #8
	and r5, r2
	ldr r0, _0807E544 @ =0x00000197
	add r0, ip
	ldrb r4, [r0]
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #1
	ldr r1, _0807E548 @ =0x081A79F9
	add r0, r0, r1
	mov r8, r0
	cmp r4, #0
	beq _0807E44C
	add r2, r5, #0
	asr r2, r4
	mov r0, #0x30
	and r5, r0
	orr r5, r2
_0807E44C:
	mov r7, #0
	add r2, r5, #0
	mov r3, #0xB8
	lsl r3, r3, #1
	add r3, ip
	mov r6, #5
	mov r4, #1
_0807E45A:
	add r0, r2, #0
	and r0, r4
	cmp r0, #0
	beq _0807E476
	ldrb r1, [r3, #0x13]
	mov r0, #0x80
	and r0, r1
	cmp r0, #0
	beq _0807E476
	ldrb r0, [r3, #0x10]
	cmp r0, r9
	bgt _0807E51A
	ldrb r0, [r3, #0x11]
	orr r7, r0
_0807E476:
	asr r2, r2, #1
	sub r3, #0x18
	sub r6, #1
	cmp r6, #0
	bge _0807E45A
	bic r7, r5
	mov r1, sl
	ldrh r0, [r1, #0x1A]
	ldr r1, _0807E540 @ =0x00000195
	add r1, ip
	strb r0, [r1]
	mov r0, #0xC6
	lsl r0, r0, #1
	add r0, ip
	mov r1, #0
	ldsh r4, [r0, r1]
	ldr r0, _0807E538 @ =0x00000FFF
	and r4, r0
	add r2, r5, #0
	mov r6, #5
	mov r3, #0xB8
	lsl r3, r3, #1
	add r3, ip
	mov r0, #0xCB
	lsl r0, r0, #1
	add r0, ip
	str r0, [sp, #0]
_0807E4AC:
	add r0, r2, #0
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _0807E4D6
	strh r4, [r3, #0xE]
	mov r0, r9
	strb r0, [r3, #0x10]
	strb r5, [r3, #0x11]
	mov r1, r8
	ldrb r0, [r1]
	lsl r0, r0, #2
	add r0, sl
	ldr r0, [r0]
	str r0, [r3]
	ldr r1, [sp, #0]
	ldrb r0, [r1]
	strb r0, [r3, #0x14]
	mov r0, #0x80
	neg r0, r0
	strb r0, [r3, #0x13]
_0807E4D6:
	mov r1, #1
	neg r1, r1
	add r8, r1
	sub r3, #0x18
	asr r2, r2, #1
	sub r6, #1
	cmp r6, #0
	bge _0807E4AC
	mov r3, #0xB8
	lsl r3, r3, #1
	add r3, ip
	cmp r7, #0
	beq _0807E508
	mov r2, #1
	mov r1, #8
_0807E4F4:
	add r0, r7, #0
	and r0, r2
	cmp r0, #0
	beq _0807E502
	ldrb r0, [r3, #0x13]
	orr r0, r1
	strb r0, [r3, #0x13]
_0807E502:
	asr r7, r7, #1
	cmp r7, #0
	bne _0807E4F4
_0807E508:
	mov r2, #0xC4
	lsl r2, r2, #1
	add r2, ip
	ldrh r1, [r2]
	ldr r0, _0807E54C @ =0x0000FFFB
	and r0, r1
	mov r1, #0x40
	orr r0, r1
	strh r0, [r2]
_0807E51A:
	mov r1, #0xC6
	lsl r1, r1, #1
	add r1, ip
	ldr r0, _0807E550 @ =0x0000FFFF
	strh r0, [r1]
_0807E524:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0807E534: .4byte 0x03005210
_0807E538: .4byte 0x00000FFF
_0807E53C: .4byte gUnk_08087FD0
_0807E540: .4byte 0x00000195
_0807E544: .4byte 0x00000197
_0807E548: .4byte gUnk_081A79F9
_0807E54C: .4byte 0x0000FFFB
_0807E550: .4byte 0x0000FFFF
	thumb_func_end sub_0807E3D8

