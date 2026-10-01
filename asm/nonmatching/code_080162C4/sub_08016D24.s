	thumb_func_start sub_08016D24
sub_08016D24: @ 0x08016D24
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _08016D60 @ =0x020185C0
	ldrh r0, [r4]
	lsr r0, r0, #0xF
	mov r8, r0
	ldrh r1, [r4, #2]
	mov ip, r1
	ldrb r6, [r4, #4]
	ldrh r2, [r4, #4]
	lsr r3, r2, #8
	ldr r0, _08016D64 @ =0x0000080A
	add r7, r4, r0
	ldrb r2, [r7]
	lsl r0, r2, #0x19
	lsr r5, r0, #0x19
	cmp r5, #0
	beq _08016D6C
	cmp r5, #1
	beq _08016DB0
	ldr r2, _08016D68 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	b _08016E68
	.align 2, 0
_08016D60: .4byte 0x020185C0
_08016D64: .4byte 0x0000080A
_08016D68: .4byte 0x0000080D
_08016D6C:
	ldr r1, _08016DA0 @ =0x0201CFB0
	ldr r0, _08016DA4 @ =0x00000808
	add r1, r1, r0
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r0, ip
	add r1, r6, #0
	add r2, r3, #0
	bl sub_08024134
	ldr r3, _08016DA8 @ =0x0000080C
	add r1, r4, r3
	ldr r0, _08016DAC @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	b _08016E5E
	.align 2, 0
_08016DA0: .4byte 0x0201CFB0
_08016DA4: .4byte 0x00000808
_08016DA8: .4byte 0x0000080C
_08016DAC: .4byte 0xFFFFF01F
_08016DB0:
	ldr r6, _08016E44 @ =0x0201CFB0
	ldr r3, _08016E48 @ =0x00000808
	add r0, r6, r3
	mov r1, #9
	neg r1, r1
	ldrb r3, [r0]
	and r1, r3
	strb r1, [r0]
	ldr r0, _08016E4C @ =0x0000080C
	add r4, r4, r0
	ldrh r1, [r4]
	lsl r0, r1, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0x3F
	bgt _08016E5C
	ldr r2, _08016E4C @ =0x0000080C
	add r0, r6, r2
	ldr r0, [r0]
	add r0, #8
	add r2, #4
	add r1, r6, r2
	ldr r1, [r1]
	ldrb r2, [r6, #4]
	sub r1, r1, r2
	lsl r1, r1, #0x10
	orr r0, r1
	ldr r2, _08016E50 @ =0x081A4444
	mov r1, #7
	and r3, r1
	lsl r1, r3, #1
	add r1, r1, r2
	ldrh r1, [r1]
	lsl r3, r1, #0x10
	mov r2, r8
	lsl r1, r2, #6
	orr r3, r1
	mov r1, #0x80
	mov r2, #0
	bl sub_08076714
	ldrh r1, [r4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r7, #0x7F
	and r0, r7
	lsl r0, r0, #5
	ldr r3, _08016E54 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r4]
	ldr r1, _08016E58 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08016E2C
	ldrb r6, [r6]
	and r5, r6
	cmp r5, #0
	beq _08016E68
_08016E2C:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x37
	bgt _08016E68
	add r0, #7
	and r0, r7
	lsl r0, r0, #5
	and r2, r3
	orr r2, r0
	strh r2, [r4]
	b _08016E68
	.align 2, 0
_08016E44: .4byte 0x0201CFB0
_08016E48: .4byte 0x00000808
_08016E4C: .4byte 0x0000080C
_08016E50: .4byte gUnk_081A4444
_08016E54: .4byte 0xFFFFF01F
_08016E58: .4byte 0x03000040
_08016E5C:
	mov r1, #2
_08016E5E:
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
_08016E68:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08016D24
	.align 2, 0

