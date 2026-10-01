	thumb_func_start sub_0800EE50
sub_0800EE50: @ 0x0800EE50
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r5, #0
	ldr r1, _0800EEA4 @ =0x020192E0
	ldr r0, _0800EEA8 @ =0x00001ACC
	add r2, r1, r0
	ldr r0, [r2]
	lsl r0, r0, #0xD
	lsr r0, r0, #0x1C
	ldr r7, _0800EEAC @ =0x020185C0
	cmp r5, r0
	bge _0800EF0E
	ldrh r3, [r7]
	mov ip, r3
	add r6, r2, #0
	ldr r0, _0800EEB0 @ =0x00001AD0
	mov sl, r0
	add r2, r0, #0
	add r2, r2, r1
	mov r9, r2
	mov r8, r1
_0800EE80:
	ldr r3, _0800EEAC @ =0x020185C0
	ldrb r2, [r3, #2]
	lsl r1, r2, #8
	lsl r0, r5, #1
	add r0, r9
	ldrh r4, [r0]
	mov r0, #0x80
	lsl r0, r0, #8
	mov r3, ip
	and r0, r3
	cmp r0, #0
	beq _0800EEB4
	mov r0, #1
	orr r1, r0
	cmp r4, r1
	beq _0800EEBC
	add r3, r5, #1
	b _0800EF02
_0800EEA4: .4byte 0x020192E0
_0800EEA8: .4byte 0x00001ACC
_0800EEAC: .4byte 0x020185C0
_0800EEB0: .4byte 0x00001AD0
_0800EEB4:
	lsl r0, r2, #8
	add r3, r5, #1
	cmp r4, r0
	bne _0800EF02
_0800EEBC:
	ldr r2, [r6]
	lsl r0, r2, #0xD
	lsr r0, r0, #0x1C
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #0xF
	ldr r1, _0800EF2C @ =0xFFF87FFF
	and r1, r2
	orr r1, r0
	str r1, [r6]
	add r2, r5, #0
	lsl r1, r1, #0xD
	lsr r1, r1, #0x1C
	add r3, r5, #1
	cmp r5, r1
	bge _0800EF02
	ldr r4, _0800EF30 @ =0x0201ADAC
	lsl r0, r5, #1
	add r0, sl
	mov r5, r8
	add r1, r0, r5
_0800EEEC:
	ldrh r0, [r1, #2]
	strh r0, [r1]
	ldrh r0, [r1, #0x22]
	strh r0, [r1, #0x20]
	add r1, #2
	add r2, #1
	ldr r0, [r4]
	lsl r0, r0, #0xD
	lsr r0, r0, #0x1C
	cmp r2, r0
	blt _0800EEEC
_0800EF02:
	add r5, r3, #0
	ldr r0, [r6]
	lsl r0, r0, #0xD
	lsr r0, r0, #0x1C
	cmp r5, r0
	blt _0800EE80
_0800EF0E:
	ldr r0, _0800EF34 @ =0x0000080D
	add r1, r7, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800EF2C: .4byte 0xFFF87FFF
_0800EF30: .4byte 0x0201ADAC
_0800EF34: .4byte 0x0000080D
	thumb_func_end sub_0800EE50

