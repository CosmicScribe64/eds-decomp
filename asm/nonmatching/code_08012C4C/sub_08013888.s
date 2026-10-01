	thumb_func_start sub_08013888
sub_08013888: @ 0x08013888
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldr r7, _080138D4 @ =0x020185C0
	ldrh r0, [r7]
	lsr r6, r0, #0xF
	mov r0, #0x68
	mul r0, r6
	add r0, #8
	mov ip, r0
	lsl r0, r6, #1
	add r0, r0, r6
	lsl r0, r0, #3
	mov r1, #0x58
	sub r1, r1, r0
	mov r0, #0xD
	cmp r5, #0
	beq _080138B6
	mov r0, #0xC
_080138B6:
	mov sl, r0
	ldr r2, _080138D8 @ =0x0000080A
	add r2, r2, r7
	mov r9, r2
	ldrb r2, [r2]
	lsl r0, r2, #0x19
	lsr r4, r0, #0x19
	cmp r4, #1
	beq _08013938
	cmp r4, #1
	bgt _080138DC
	cmp r4, #0
	beq _080138E4
	b _08013B84
	.align 2, 0
_080138D4: .4byte 0x020185C0
_080138D8: .4byte 0x0000080A
_080138DC:
	cmp r4, #2
	bne _080138E2
	b _080139D8
_080138E2:
	b _08013B84
_080138E4:
	ldr r0, _08013918 @ =0x050003E0
	ldr r1, _0801391C @ =0x0868247C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _08013920 @ =0x06016C80
	ldr r1, _08013924 @ =0x0868267C
	mov r2, #0xC0
	lsl r2, r2, #4
	bl sub_080752B0
	ldr r0, _08013928 @ =0x0201CFB0
	ldr r3, _0801392C @ =0x0000085C
	add r0, r0, r3
	str r4, [r0]
	mov r0, #0
	mov r1, #0
	bl sub_080240A8
	ldr r0, _08013930 @ =0x0000080C
	add r1, r7, r0
	ldr r0, _08013934 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	b _080139BA
_08013918: .4byte 0x050003E0
_0801391C: .4byte gUnk_0868247C
_08013920: .4byte 0x06016C80
_08013924: .4byte gUnk_0868267C
_08013928: .4byte 0x0201CFB0
_0801392C: .4byte 0x0000085C
_08013930: .4byte 0x0000080C
_08013934: .4byte 0xFFFFF01F
_08013938:
	ldrh r2, [r7, #2]
	cmp r5, #0
	bne _08013940
	neg r2, r2
_08013940:
	neg r3, r5
	orr r3, r5
	lsr r3, r3, #0x1F
	mov r0, ip
	bl sub_080137F8
	ldr r0, _0801399C @ =0x0000080C
	add r5, r7, r0
	ldrh r1, [r5]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x59
	bgt _080139AC
	add r0, #1
	mov r6, #0x7F
	and r0, r6
	lsl r0, r0, #5
	ldr r3, _080139A0 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r5]
	ldr r1, _080139A4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08013984
	ldr r0, _080139A8 @ =0x0201CFB0
	ldrb r0, [r0]
	and r4, r0
	cmp r4, #0
	bne _08013984
	b _08013B98
_08013984:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x4F
	ble _0801398E
	b _08013B98
_0801398E:
	add r0, #7
	and r0, r6
	lsl r0, r0, #5
	and r2, r3
	orr r2, r0
	strh r2, [r5]
	b _08013B98
_0801399C: .4byte 0x0000080C
_080139A0: .4byte 0xFFFFF01F
_080139A4: .4byte 0x03000040
_080139A8: .4byte 0x0201CFB0
_080139AC:
	mov r0, sl
	bl sub_08077AEC
	ldr r0, _080139D4 @ =0xFFFFF01F
	ldrh r1, [r5]
	and r0, r1
	strh r0, [r5]
_080139BA:
	mov r3, r9
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08013B98
_080139D4: .4byte 0xFFFFF01F
_080139D8:
	ldr r0, _080139F8 @ =0x020192E4
	mov r8, r0
	ldr r0, _080139FC @ =0x00000D64
	mul r0, r6
	mov r3, r8
	add r4, r0, r3
	ldrh r0, [r4]
	cmp r0, #0
	bne _08013A00
	mov r1, #3
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r9
	b _08013B96
_080139F8: .4byte 0x020192E4
_080139FC: .4byte 0x00000D64
_08013A00:
	ldrh r2, [r7, #2]
	cmp r5, #0
	bne _08013A08
	neg r2, r2
_08013A08:
	neg r3, r5
	orr r3, r5
	lsr r3, r3, #0x1F
	mov r0, ip
	bl sub_080137F8
	ldrh r0, [r7, #2]
	add r1, r0, #0
	cmp r1, #0x63
	bls _08013A94
	sub r0, #0x64
	strh r0, [r7, #2]
	cmp r5, #0
	beq _08013A2C
	ldrh r0, [r4]
	add r0, #0x64
	strh r0, [r4]
	b _08013A36
_08013A2C:
	mov r0, r8
	add r1, r6, #0
	mov r2, #0x64
	bl sub_08007418
_08013A36:
	ldr r1, _08013A80 @ =0x020192E4
	ldr r0, _08013A84 @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r6, #0
	bl sub_08060934
	ldr r0, _08013A88 @ =0x020185C0
	ldr r2, _08013A8C @ =0x0000080C
	add r4, r0, r2
	ldrh r2, [r4]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r5, _08013A90 @ =0xFFFFF01F
	add r0, r5, #0
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xA
	bgt _08013A6E
	b _08013B98
_08013A6E:
	mov r0, sl
	bl sub_08077AEC
	add r0, r5, #0
	ldrh r3, [r4]
	and r0, r3
	strh r0, [r4]
	b _08013B98
	.align 2, 0
_08013A80: .4byte 0x020192E4
_08013A84: .4byte 0x00000D64
_08013A88: .4byte 0x020185C0
_08013A8C: .4byte 0x0000080C
_08013A90: .4byte 0xFFFFF01F
_08013A94:
	cmp r1, #9
	bls _08013B0C
	sub r0, #0xA
	strh r0, [r7, #2]
	cmp r5, #0
	beq _08013AA8
	ldrh r0, [r4]
	add r0, #0xA
	strh r0, [r4]
	b _08013AB2
_08013AA8:
	mov r0, r8
	add r1, r6, #0
	mov r2, #0xA
	bl sub_08007418
_08013AB2:
	ldr r1, _08013AF8 @ =0x020192E4
	ldr r0, _08013AFC @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r6, #0
	bl sub_08060934
	ldr r0, _08013B00 @ =0x020185C0
	ldr r1, _08013B04 @ =0x0000080C
	add r4, r0, r1
	ldrh r2, [r4]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r5, _08013B08 @ =0xFFFFF01F
	add r0, r5, #0
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xA
	ble _08013B98
	mov r0, sl
	bl sub_08077AEC
	add r0, r5, #0
	ldrh r2, [r4]
	and r0, r2
	strh r0, [r4]
	b _08013B98
_08013AF8: .4byte 0x020192E4
_08013AFC: .4byte 0x00000D64
_08013B00: .4byte 0x020185C0
_08013B04: .4byte 0x0000080C
_08013B08: .4byte 0xFFFFF01F
_08013B0C:
	cmp r1, #0
	beq _08013B84
	sub r0, #1
	strh r0, [r7, #2]
	cmp r5, #0
	beq _08013B20
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
	b _08013B2A
_08013B20:
	mov r0, r8
	add r1, r6, #0
	mov r2, #1
	bl sub_08007418
_08013B2A:
	ldr r1, _08013B70 @ =0x020192E4
	ldr r0, _08013B74 @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r6, #0
	bl sub_08060934
	ldr r0, _08013B78 @ =0x020185C0
	ldr r3, _08013B7C @ =0x0000080C
	add r4, r0, r3
	ldrh r2, [r4]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r5, _08013B80 @ =0xFFFFF01F
	add r0, r5, #0
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xA
	ble _08013B98
	mov r0, sl
	bl sub_08077AEC
	add r0, r5, #0
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	b _08013B98
_08013B70: .4byte 0x020192E4
_08013B74: .4byte 0x00000D64
_08013B78: .4byte 0x020185C0
_08013B7C: .4byte 0x0000080C
_08013B80: .4byte 0xFFFFF01F
_08013B84:
	bl sub_080241C4
	ldr r1, _08013BA8 @ =0x020185C0
	ldr r2, _08013BAC @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
_08013B96:
	strb r0, [r1]
_08013B98:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08013BA8: .4byte 0x020185C0
_08013BAC: .4byte 0x0000080D
	thumb_func_end sub_08013888

