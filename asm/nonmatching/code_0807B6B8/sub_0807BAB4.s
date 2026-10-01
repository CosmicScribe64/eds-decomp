	thumb_func_start sub_0807BAB4
sub_0807BAB4: @ 0x0807BAB4
	push {r4, r5, r6, lr}
	add r6, r0, #0
	ldrb r0, [r6, #0x14]
	cmp r0, #1
	beq _0807BAC0
	b _0807BCCA
_0807BAC0:
	ldrb r2, [r6, #0x15]
	cmp r2, #1
	beq _0807BB86
	cmp r2, #1
	bgt _0807BAD0
	cmp r2, #0
	beq _0807BADE
	b _0807BCCA
_0807BAD0:
	cmp r2, #2
	bne _0807BAD6
	b _0807BBFC
_0807BAD6:
	cmp r2, #3
	bne _0807BADC
	b _0807BC58
_0807BADC:
	b _0807BCCA
_0807BADE:
	mov r2, #0
	ldsh r1, [r6, r2]
	mov r3, #8
	ldsh r0, [r6, r3]
	sub r1, r1, r0
	cmp r1, #0
	bge _0807BAEE
	neg r1, r1
_0807BAEE:
	cmp r1, #7
	bgt _0807BB04
	mov r1, #0xC
	ldsh r0, [r6, r1]
	mov r2, #1
	neg r2, r2
	add r1, r2, #0
	cmp r0, #0
	ble _0807BB02
	mov r1, #1
_0807BB02:
	strh r1, [r6, #0xC]
_0807BB04:
	ldrh r0, [r6, #0xC]
	ldrh r3, [r6]
	add r1, r0, r3
	strh r1, [r6]
	lsl r0, r0, #0x10
	cmp r0, #0
	blt _0807BB1E
	lsl r1, r1, #0x10
	ldrh r2, [r6, #8]
	lsl r0, r2, #0x10
	cmp r1, r0
	blt _0807BB2A
	b _0807BB28
_0807BB1E:
	lsl r1, r1, #0x10
	ldrh r2, [r6, #8]
	lsl r0, r2, #0x10
	cmp r1, r0
	bgt _0807BB2A
_0807BB28:
	strh r2, [r6]
_0807BB2A:
	mov r0, #2
	ldsh r1, [r6, r0]
	mov r2, #0xA
	ldsh r0, [r6, r2]
	sub r1, r1, r0
	cmp r1, #0
	bge _0807BB3A
	neg r1, r1
_0807BB3A:
	cmp r1, #7
	bgt _0807BB50
	mov r3, #0xE
	ldsh r0, [r6, r3]
	mov r2, #1
	neg r2, r2
	add r1, r2, #0
	cmp r0, #0
	ble _0807BB4E
	mov r1, #1
_0807BB4E:
	strh r1, [r6, #0xE]
_0807BB50:
	ldrh r0, [r6, #0xE]
	ldrh r3, [r6, #2]
	add r1, r0, r3
	strh r1, [r6, #2]
	lsl r0, r0, #0x10
	cmp r0, #0
	blt _0807BB6A
	lsl r1, r1, #0x10
	ldrh r2, [r6, #0xA]
	lsl r0, r2, #0x10
	cmp r1, r0
	blt _0807BB76
	b _0807BB74
_0807BB6A:
	lsl r1, r1, #0x10
	ldrh r2, [r6, #0xA]
	lsl r0, r2, #0x10
	cmp r1, r0
	bgt _0807BB76
_0807BB74:
	strh r2, [r6, #2]
_0807BB76:
	ldr r1, [r6]
	ldr r0, [r6, #8]
	cmp r1, r0
	beq _0807BB80
	b _0807BCCA
_0807BB80:
	mov r0, #2
	strb r0, [r6, #0x14]
	b _0807BCCA
_0807BB86:
	ldrh r1, [r6, #0xE]
	ldrh r2, [r6, #0xC]
	add r0, r1, r2
	add r1, r0, #0
	strh r0, [r6, #0xE]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	cmp r0, #0x7F
	ble _0807BBA8
	mov r0, #0x7F
	strh r0, [r6, #0xE]
	mov r0, #2
	strb r0, [r6, #0x14]
	ldrh r0, [r6, #8]
	strh r0, [r6]
	ldrh r0, [r6, #0xA]
	b _0807BCC8
_0807BBA8:
	mov r3, #0x10
	ldsh r0, [r6, r3]
	lsl r0, r0, #4
	ldr r5, _0807BBF8 @ =0x08087BA4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x17
	add r1, #0x80
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	neg r1, r1
	mov r4, #0x80
	lsl r4, r4, #1
	add r1, r1, r4
	asr r1, r1, #1
	bl sub_0807B4D0
	asr r0, r0, #4
	ldrh r3, [r6, #4]
	add r0, r3, r0
	strh r0, [r6]
	mov r1, #0x12
	ldsh r0, [r6, r1]
	lsl r0, r0, #4
	ldrb r1, [r6, #0xE]
	add r1, #0x40
	lsl r1, r1, #1
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	sub r4, r4, r1
	asr r4, r4, #1
	add r1, r4, #0
	bl sub_0807B4D0
	asr r0, r0, #4
	ldrh r3, [r6, #6]
	add r0, r3, r0
	b _0807BCC8
	.align 2, 0
_0807BBF8: .4byte gUnk_08087BA4
_0807BBFC:
	ldrh r1, [r6, #0xE]
	ldrh r3, [r6, #0xC]
	add r0, r1, r3
	strh r0, [r6, #0xE]
	lsl r1, r0, #0x10
	asr r0, r1, #0x10
	mov r3, #0xFC
	lsl r3, r3, #6
	cmp r0, r3
	ble _0807BC1C
	strh r3, [r6, #0xE]
	strb r2, [r6, #0x14]
	ldrh r0, [r6, #8]
	strh r0, [r6]
	ldrh r0, [r6, #0xA]
	b _0807BC56
_0807BC1C:
	mov r2, #0x10
	ldsh r0, [r6, r2]
	lsl r0, r0, #4
	ldr r4, _0807BC7C @ =0x08087BA4
	lsr r1, r1, #0x18
	lsl r1, r1, #1
	add r1, r1, r4
	mov r3, #0
	ldsh r1, [r1, r3]
	bl sub_0807B4D0
	asr r0, r0, #4
	ldrh r1, [r6, #4]
	add r0, r1, r0
	strh r0, [r6]
	mov r2, #0x12
	ldsh r0, [r6, r2]
	lsl r0, r0, #4
	ldrh r3, [r6, #0xE]
	lsr r1, r3, #8
	lsl r1, r1, #1
	add r1, r1, r4
	mov r2, #0
	ldsh r1, [r1, r2]
	bl sub_0807B4D0
	asr r0, r0, #4
	ldrh r3, [r6, #6]
	add r0, r3, r0
_0807BC56:
	strh r0, [r6, #2]
_0807BC58:
	ldrh r1, [r6, #0xE]
	ldrh r2, [r6, #0xC]
	add r0, r1, r2
	strh r0, [r6, #0xE]
	lsl r1, r0, #0x10
	asr r0, r1, #0x10
	mov r2, #0xFC
	lsl r2, r2, #6
	cmp r0, r2
	ble _0807BC80
	strh r2, [r6, #0xE]
	mov r0, #2
	strb r0, [r6, #0x14]
	ldrh r0, [r6, #8]
	strh r0, [r6]
	ldrh r0, [r6, #0xA]
	b _0807BCC8
	.align 2, 0
_0807BC7C: .4byte gUnk_08087BA4
_0807BC80:
	mov r3, #0x10
	ldsh r0, [r6, r3]
	lsl r0, r0, #4
	ldr r5, _0807BCD0 @ =0x08087BA4
	lsr r1, r1, #0x18
	add r1, #0x40
	lsl r1, r1, #1
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	mov r4, #0x80
	lsl r4, r4, #1
	sub r1, r4, r1
	bl sub_0807B4D0
	asr r0, r0, #4
	ldrh r3, [r6, #4]
	add r0, r3, r0
	strh r0, [r6]
	mov r1, #0x12
	ldsh r0, [r6, r1]
	lsl r0, r0, #4
	ldrh r2, [r6, #0xE]
	lsr r1, r2, #8
	add r1, #0x40
	lsl r1, r1, #1
	add r1, r1, r5
	mov r3, #0
	ldsh r1, [r1, r3]
	sub r4, r4, r1
	add r1, r4, #0
	bl sub_0807B4D0
	asr r0, r0, #4
	ldrh r1, [r6, #6]
	add r0, r1, r0
_0807BCC8:
	strh r0, [r6, #2]
_0807BCCA:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0807BCD0: .4byte gUnk_08087BA4
	thumb_func_end sub_0807BAB4

