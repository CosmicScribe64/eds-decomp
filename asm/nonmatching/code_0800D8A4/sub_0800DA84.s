	thumb_func_start sub_0800DA84
sub_0800DA84: @ 0x0800DA84
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r5, _0800DAB4 @ =0x020185C0
	ldrh r0, [r5]
	lsr r6, r0, #0xF
	ldrh r7, [r5, #2]
	ldr r1, _0800DAB8 @ =0x0000080A
	add r1, r1, r5
	mov sl, r1
	ldrb r2, [r1]
	lsl r0, r2, #0x19
	lsr r4, r0, #0x19
	cmp r4, #1
	beq _0800DB08
	cmp r4, #1
	bgt _0800DABC
	cmp r4, #0
	beq _0800DAC2
	b _0800DCCC
	.align 2, 0
_0800DAB4: .4byte 0x020185C0
_0800DAB8: .4byte 0x0000080A
_0800DABC:
	cmp r4, #2
	beq _0800DBA0
	b _0800DCCC
_0800DAC2:
	mov r0, #0x94
	mul r0, r7
	ldr r1, _0800DAFC @ =0x00000D64
	mul r1, r6
	add r0, r0, r1
	ldr r1, _0800DB00 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800DAEA
	bl sub_080611AC
	ldr r0, _0800DB04 @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800DAEA:
	add r0, r7, #0
	bl sub_08062354
	add r1, r0, #0
	add r0, r6, #0
	bl sub_080240A8
	b _0800DC8E
	.align 2, 0
_0800DAFC: .4byte 0x00000D64
_0800DB00: .4byte 0x0201930C
_0800DB04: .4byte 0x0000080D
_0800DB08:
	ldrb r0, [r5, #4]
	cmp r0, #0
	beq _0800DB7A
	mov r0, #8
	bl sub_08077AEC
	add r0, r6, #0
	and r0, r4
	mov r1, #2
	neg r1, r1
	ldr r2, [sp, #0]
	and r2, r1
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	ldr r0, _0800DB84 @ =0x000001FF
	and r0, r7
	lsl r0, r0, #5
	ldr r1, _0800DB88 @ =0xFFFFC01F
	and r2, r1
	orr r2, r0
	str r2, [sp, #0]
	add r1, r6, #0
	and r1, r4
	mov r0, #0x94
	add r3, r7, #0
	mul r3, r0
	ldr r0, _0800DB8C @ =0x00000D64
	mul r0, r1
	add r3, r3, r0
	ldr r0, _0800DB90 @ =0x0201930C
	add r3, r3, r0
	ldrb r1, [r3, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r4
	lsl r0, r0, #0xE
	ldr r1, _0800DB94 @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	and r0, r4
	lsl r0, r0, #0xF
	ldr r2, _0800DB98 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldr r1, _0800DB9C @ =0x0868CAC0
	mov r0, sp
	mov r2, #0
	mov r3, #0
	bl sub_08024380
_0800DB7A:
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08060FD0
	b _0800DC8E
_0800DB84: .4byte 0x000001FF
_0800DB88: .4byte 0xFFFFC01F
_0800DB8C: .4byte 0x00000D64
_0800DB90: .4byte 0x0201930C
_0800DB94: .4byte 0xFFFFBFFF
_0800DB98: .4byte 0xFFFF7FFF
_0800DB9C: .4byte gUnk_0868CAC0
_0800DBA0:
	ldr r0, _0800DCA8 @ =0x00000814
	add r0, r0, r5
	mov r8, r0
	add r0, r6, #0
	mov r1, #1
	and r0, r1
	ldr r1, _0800DCAC @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r2, _0800DCB0 @ =0x0201930C
	mov r9, r2
	add r1, r5, r2
	mov r0, #0x94
	add r4, r7, #0
	mul r4, r0
	add r1, r1, r4
	mov r0, r8
	bl sub_08007558
	add r4, r4, r5
	add r9, r4
	mov r0, #5
	neg r0, r0
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	mov r2, r9
	strb r0, [r2, #2]
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08008E44
	mov r0, r8
	ldr r0, [r0]
	mov r8, r0
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov ip, r0
	ldr r0, _0800DCB4 @ =0x000007FF
	mov r1, ip
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0800DCB8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r1, _0800DCBC @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0800DC8E
	mov r1, sp
	mov r0, #2
	neg r0, r0
	ldrb r1, [r1]
	and r0, r1
	orr r0, r6
	mov r1, sp
	strb r0, [r1]
	mov r5, #0x1F
	neg r5, r5
	and r0, r5
	strb r0, [r1]
	ldr r2, _0800DCC0 @ =0x000001FF
	add r0, r2, #0
	and r7, r0
	lsl r2, r7, #5
	ldr r4, _0800DCC4 @ =0xFFFFC01F
	add r0, r4, #0
	ldrh r1, [r1]
	and r0, r1
	orr r0, r2
	mov r1, sp
	strh r0, [r1]
	mov r6, r9
	ldrb r6, [r6, #6]
	lsl r1, r6, #0x1F
	mov r3, sp
	lsr r1, r1, #0x1F
	lsl r1, r1, #6
	ldrb r2, [r3, #1]
	mov r0, #0x41
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #1]
	mov r2, r9
	ldrb r2, [r2, #6]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1F
	lsl r1, r1, #7
	mov r2, #0x7F
	and r0, r2
	orr r0, r1
	strb r0, [r3, #1]
	mov r6, r8
	lsl r1, r6, #0x13
	lsr r1, r1, #0x1F
	mov r0, #1
	and r1, r0
	ldr r0, [sp, #4]
	sub r2, #0x81
	and r0, r2
	orr r0, r1
	and r0, r5
	mov r1, #0x1C
	orr r0, r1
	and r0, r4
	ldr r1, _0800DCC8 @ =0xFFFFBFFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #4]
	add r2, sp, #4
	mov r0, ip
	mov r1, sp
	bl sub_080242C4
_0800DC8E:
	mov r6, sl
	ldrb r2, [r6]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r6]
	b _0800DCEA
_0800DCA8: .4byte 0x00000814
_0800DCAC: .4byte 0x00000D64
_0800DCB0: .4byte 0x0201930C
_0800DCB4: .4byte 0x000007FF
_0800DCB8: .4byte gUnk_08622AB4
_0800DCBC: .4byte 0xFFFFF880
_0800DCC0: .4byte 0x000001FF
_0800DCC4: .4byte 0xFFFFC01F
_0800DCC8: .4byte 0xFFFFBFFF
_0800DCCC:
	bl sub_080611AC
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_08024134
	ldr r1, _0800DCFC @ =0x020185C0
	ldr r0, _0800DD00 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800DCEA:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800DCFC: .4byte 0x020185C0
_0800DD00: .4byte 0x0000080D
	thumb_func_end sub_0800DA84

