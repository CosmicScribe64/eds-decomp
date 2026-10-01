	thumb_func_start sub_0800FC00
sub_0800FC00: @ 0x0800FC00
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC
	ldr r5, _0800FC58 @ =0x020185C0
	ldrh r0, [r5]
	lsr r7, r0, #0xF
	ldrh r1, [r5, #4]
	lsl r0, r1, #0x10
	ldrh r2, [r5, #2]
	orr r0, r2
	str r0, [sp, #0]
	ldr r0, _0800FC5C @ =0x0000080A
	add r4, r5, r0
	ldrb r1, [r4]
	lsl r0, r1, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _0800FC64
	cmp r6, #1
	beq _0800FC6E
	mov r0, sp
	bl sub_0800743C
	add r0, r7, #0
	mov r1, sp
	bl sub_08009EAC
	add r0, r7, #0
	mov r1, #0xB
	mov r2, #0
	bl sub_08024134
	bl sub_080611AC
	ldr r2, _0800FC60 @ =0x0000080D
	add r1, r5, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _0800FDB4
	.align 2, 0
_0800FC58: .4byte 0x020185C0
_0800FC5C: .4byte 0x0000080A
_0800FC60: .4byte 0x0000080D
_0800FC64:
	add r0, r7, #0
	mov r1, #0xB
	bl sub_080240A8
	b _0800FD9E
_0800FC6E:
	add r0, r7, #0
	mov r1, sp
	bl sub_08009B48
	add r1, r7, #0
	and r1, r6
	mov r4, #2
	neg r4, r4
	ldr r0, [sp, #4]
	and r0, r4
	orr r0, r1
	mov r2, #0x1F
	neg r2, r2
	and r0, r2
	mov r1, #0x1C
	orr r0, r1
	ldr r1, _0800FCF4 @ =0xFFFFC01F
	mov ip, r1
	and r0, r1
	sub r1, #0x20
	mov r8, r1
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #4]
	ldr r1, _0800FCF8 @ =0x00000814
	add r0, r5, r1
	ldr r3, [r0]
	lsl r1, r3, #0x13
	lsr r1, r1, #0x1F
	and r1, r6
	ldr r0, [sp, #8]
	and r0, r4
	orr r0, r1
	and r0, r2
	mov r1, #0x16
	orr r0, r1
	str r0, [sp, #8]
	ldr r2, _0800FCFC @ =0x020192E4
	and r7, r6
	ldr r1, _0800FD00 @ =0x00000D64
	mul r1, r7
	add r1, r1, r2
	ldrb r1, [r1, #2]
	lsl r1, r1, #5
	mov r2, ip
	and r0, r2
	orr r0, r1
	mov r1, r8
	and r0, r1
	ldr r1, _0800FD04 @ =0xFFFF7FFF
	and r0, r1
	str r0, [sp, #8]
	lsl r3, r3, #0x14
	lsr r3, r3, #0x14
	ldr r0, _0800FD08 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _0800FD0C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0800FD10 @ =0x00000776
	cmp r1, r0
	bne _0800FD14
	mov r0, #3
	b _0800FD76
_0800FCF4: .4byte 0xFFFFC01F
_0800FCF8: .4byte 0x00000814
_0800FCFC: .4byte 0x020192E4
_0800FD00: .4byte 0x00000D64
_0800FD04: .4byte 0xFFFF7FFF
_0800FD08: .4byte 0x000007FF
_0800FD0C: .4byte gUnk_08622AB4
_0800FD10: .4byte 0x00000776
_0800FD14:
	cmp r1, r0
	blt _0800FD24
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0800FD24
	mov r0, #1
	b _0800FD76
_0800FD24:
	ldr r0, _0800FD48 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0800FD4C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0800FD56
	cmp r0, #0x16
	bgt _0800FD50
	cmp r0, #0x15
	beq _0800FD5A
	b _0800FD62
	.align 2, 0
_0800FD48: .4byte 0x000007FF
_0800FD4C: .4byte gUnk_08621DE0
_0800FD50:
	cmp r0, #0x17
	beq _0800FD5E
	b _0800FD62
_0800FD56:
	mov r0, #7
	b _0800FD76
_0800FD5A:
	mov r0, #8
	b _0800FD76
_0800FD5E:
	mov r0, #9
	b _0800FD76
_0800FD62:
	ldr r0, _0800FDC0 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r2, _0800FDC4 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0800FD76:
	cmp r0, #2
	bne _0800FD8C
	mov r1, #0x1F
	neg r1, r1
	ldr r0, [sp, #8]
	and r0, r1
	mov r1, #0x18
	orr r0, r1
	ldr r1, _0800FDC8 @ =0xFFFFC01F
	and r0, r1
	str r0, [sp, #8]
_0800FD8C:
	ldr r4, _0800FDCC @ =0x02018DD4
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, sp, #4
	add r2, sp, #8
	bl sub_080242C4
	sub r4, #0xA
_0800FD9E:
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
_0800FDB4:
	add sp, #0xC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800FDC0: .4byte 0x000007FF
_0800FDC4: .4byte gUnk_08621DE0
_0800FDC8: .4byte 0xFFFFC01F
_0800FDCC: .4byte 0x02018DD4
	thumb_func_end sub_0800FC00

