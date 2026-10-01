	thumb_func_start sub_0805DA1C
sub_0805DA1C: @ 0x0805DA1C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, _0805DB78 @ =0x0201D7F0
	mov r0, #8
	neg r0, r0
	add r0, r0, r5
	mov sl, r0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0805DA3C
	mov r0, #7
	bl sub_08077AEC
_0805DA3C:
	mov r1, sl
	ldrb r1, [r1]
	cmp r1, #0xF
	bls _0805DA46
	b _0805DB4E
_0805DA46:
	ldr r2, [r5]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl sub_080623AC
	mov r9, r0
	ldr r2, [r5]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl sub_080623EC
	add r4, r0, #0
	ldr r2, [r5, #4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl sub_080623AC
	mov r8, r0
	ldr r2, [r5, #4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl sub_080623EC
	add r5, r0, #0
	mov r3, r8
	mov r0, r9
	sub r2, r3, r0
	sub r1, r5, r4
	sub r6, r0, r3
	sub r7, r4, r5
	mov r3, sl
	ldrb r0, [r3]
	lsl r0, r0, #1
	mov ip, r0
	ldr r0, _0805DB7C @ =0x081A4454
	add r0, ip
	ldrh r0, [r0]
	mul r2, r0
	mul r1, r0
	add r3, r2, #0
	cmp r2, #0
	bge _0805DABC
	add r3, #0xFF
_0805DABC:
	asr r2, r3, #8
	add r3, r1, #0
	cmp r1, #0
	bge _0805DAC6
	add r3, #0xFF
_0805DAC6:
	asr r1, r3, #8
	mul r6, r0
	mul r7, r0
	add r0, r6, #0
	cmp r6, #0
	bge _0805DAD4
	add r0, #0xFF
_0805DAD4:
	asr r6, r0, #8
	add r0, r7, #0
	cmp r7, #0
	bge _0805DADE
	add r0, #0xFF
_0805DADE:
	asr r7, r0, #8
	mov r3, r9
	add r0, r3, r2
	add r1, r4, r1
	lsl r1, r1, #0x10
	orr r0, r1
	mov r4, #0x88
	lsl r4, r4, #3
	mov r9, r4
	ldr r4, _0805DB80 @ =0x081A43E4
	mov r2, ip
	add r1, r2, r4
	ldrh r1, [r1]
	lsl r3, r1, #0x10
	mov r1, #0x80
	mov r2, r9
	bl sub_08076714
	mov r3, r8
	add r0, r3, r6
	add r1, r5, r7
	lsl r1, r1, #0x10
	orr r0, r1
	mov r2, sl
	ldrb r2, [r2]
	lsl r1, r2, #1
	add r1, r1, r4
	ldrh r1, [r1]
	lsl r3, r1, #0x10
	mov r1, #0x80
	mov r2, r9
	bl sub_08076714
	mov r4, sl
	ldrb r3, [r4]
	add r2, r3, #1
	strb r2, [r4]
	ldr r1, _0805DB84 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805DB40
	mov r0, #1
	ldr r1, _0805DB88 @ =0x0201CFB0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805DB4E
_0805DB40:
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xB
	bhi _0805DB4E
	add r0, r3, #4
	mov r2, sl
	strb r0, [r2]
_0805DB4E:
	ldr r1, _0805DB88 @ =0x0201CFB0
	ldr r3, _0805DB8C @ =0x00000838
	add r0, r1, r3
	ldrb r0, [r0]
	cmp r0, #0x10
	bne _0805DB6A
	mov r4, #0x83
	lsl r4, r4, #4
	add r1, r1, r4
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0805DB6A:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0805DB78: .4byte 0x0201D7F0
_0805DB7C: .4byte gUnk_081A4454
_0805DB80: .4byte gUnk_081A43E4
_0805DB84: .4byte 0x03000040
_0805DB88: .4byte 0x0201CFB0
_0805DB8C: .4byte 0x00000838
	thumb_func_end sub_0805DA1C

