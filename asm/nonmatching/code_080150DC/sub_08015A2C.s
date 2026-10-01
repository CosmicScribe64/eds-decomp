	thumb_func_start sub_08015A2C
sub_08015A2C: @ 0x08015A2C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r4, _08015A54 @ =0x020185C0
	ldr r1, _08015A58 @ =0x0000080A
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	add r7, r4, #0
	cmp r0, #8
	bls _08015A48
	b _08015E1C
_08015A48:
	lsl r0, r0, #2
	ldr r1, _08015A5C @ =0x08015A60
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08015A54: .4byte 0x020185C0
_08015A58: .4byte 0x0000080A
_08015A5C: .4byte 0x08015A60
_08015A60:
	.4byte _08015A84
	.4byte _08015AA0
	.4byte _08015ACC
	.4byte _08015ADC
	.4byte _08015AE4
	.4byte _08015B08
	.4byte _08015C28
	.4byte _08015CE0
	.4byte _08015DFC
_08015A84:
	bl sub_0805ED9C
	mov r0, #0
	mov r1, #0
	bl sub_080240A8
	ldr r2, _08015A98 @ =0x020185C0
	ldr r3, _08015A9C @ =0x0000080A
	add r2, r2, r3
	b _08015AAA
_08015A98: .4byte 0x020185C0
_08015A9C: .4byte 0x0000080A
_08015AA0:
	bl sub_080619E8
	ldr r2, _08015AC4 @ =0x020185C0
	ldr r0, _08015AC8 @ =0x0000080A
	add r2, r2, r0
_08015AAA:
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08015E2E
	.align 2, 0
_08015AC4: .4byte 0x020185C0
_08015AC8: .4byte 0x0000080A
_08015ACC:
	ldrh r0, [r7, #2]
	bl sub_08061A1C
	ldr r1, _08015AD8 @ =0x0000080A
	add r3, r7, r1
	b _08015E00
_08015AD8: .4byte 0x0000080A
_08015ADC:
	ldrh r0, [r7, #2]
	bl sub_08061D24
	b _08015DFC
_08015AE4:
	ldrh r0, [r7, #2]
	bl sub_08061E54
	ldr r3, _08015AFC @ =0x0000080C
	add r1, r7, r3
	ldr r0, _08015B00 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r0, _08015B04 @ =0x0000080A
	add r3, r7, r0
	b _08015E00
_08015AFC: .4byte 0x0000080C
_08015B00: .4byte 0xFFFFF01F
_08015B04: .4byte 0x0000080A
_08015B08:
	mov r0, #0
	ldr r1, _08015B84 @ =0x02018DCC
	mov r9, r1
_08015B0E:
	mov r5, #0
	lsl r4, r0, #5
	lsl r1, r0, #1
	add r0, #1
	mov r8, r0
	mov r6, #0x44
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0xB
_08015B20:
	add r1, r4, #2
	mov r2, r9
	ldrh r2, [r2]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xF
	bgt _08015B3A
	mul r1, r0
	add r0, r1, #0
	cmp r1, #0
	bge _08015B38
	add r0, #0xF
_08015B38:
	asr r1, r0, #4
_08015B3A:
	lsl r0, r1, #0x10
	orr r0, r6
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r7
	mov r1, #0x80
	bl sub_080763D0
	add r6, #0x20
	add r5, #1
	cmp r5, #3
	ble _08015B20
	mov r0, r8
	cmp r0, #4
	ble _08015B0E
	ldr r0, _08015B88 @ =0x020185C0
	ldr r3, _08015B8C @ =0x0000080C
	add r1, r0, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x19
	add r7, r0, #0
	cmp r4, #0xF
	bgt _08015B94
	ldr r1, _08015B90 @ =0x04000050
	mov r2, #0xF4
	lsl r2, r2, #4
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r0, #0x10
	sub r0, r0, r4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r4, r0
	strh r4, [r1]
	b _08015B9E
_08015B84: .4byte 0x02018DCC
_08015B88: .4byte 0x020185C0
_08015B8C: .4byte 0x0000080C
_08015B90: .4byte 0x04000050
_08015B94:
	ldr r0, _08015C10 @ =0x04000050
	mov r1, #0
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
_08015B9E:
	ldr r0, _08015C14 @ =0x0000080C
	add r3, r7, r0
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08015C18 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _08015C1C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08015BD2
	ldr r1, _08015C20 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08015BE6
_08015BD2:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xB
	bgt _08015BE6
	add r0, #3
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	strh r2, [r3]
_08015BE6:
	ldr r1, _08015C14 @ =0x0000080C
	add r4, r7, r1
	ldrh r2, [r4]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xF
	bgt _08015BF6
	b _08015E2E
_08015BF6:
	bl sub_0805ED9C
	ldrh r0, [r7, #2]
	bl sub_0805F00C
	ldr r0, _08015C18 @ =0xFFFFF01F
	ldrh r3, [r4]
	and r0, r3
	strh r0, [r4]
	ldr r0, _08015C24 @ =0x0000080A
	add r3, r7, r0
	b _08015E00
	.align 2, 0
_08015C10: .4byte 0x04000050
_08015C14: .4byte 0x0000080C
_08015C18: .4byte 0xFFFFF01F
_08015C1C: .4byte 0x03000040
_08015C20: .4byte 0x0201CFB0
_08015C24: .4byte 0x0000080A
_08015C28:
	mov r0, #0
_08015C2A:
	mov r5, #0
	lsl r4, r0, #5
	lsl r1, r0, #1
	add r0, #1
	mov r8, r0
	add r0, r4, #2
	lsl r7, r0, #0x10
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0xB
	mov r4, #0x44
_08015C40:
	add r0, r4, #0
	orr r0, r7
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r6
	mov r1, #0x80
	bl sub_080762D0
	add r4, #0x20
	add r5, #1
	cmp r5, #3
	ble _08015C40
	mov r0, r8
	cmp r0, #4
	ble _08015C2A
	ldr r2, _08015CC8 @ =0x020185C0
	ldr r1, _08015CCC @ =0x0000080C
	add r4, r2, r1
	ldrh r1, [r4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r6, #0x7F
	and r0, r6
	lsl r0, r0, #5
	ldr r5, _08015CD0 @ =0xFFFFF01F
	add r3, r5, #0
	and r3, r1
	orr r3, r0
	strh r3, [r4]
	ldr r1, _08015CD4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	add r7, r2, #0
	cmp r0, #0
	bne _08015C96
	ldr r1, _08015CD8 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08015CAA
_08015C96:
	lsl r0, r3, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x17
	bgt _08015CAA
	add r0, #7
	and r0, r6
	lsl r0, r0, #5
	and r3, r5
	orr r3, r0
	strh r3, [r4]
_08015CAA:
	ldr r3, _08015CCC @ =0x0000080C
	add r2, r7, r3
	ldrh r1, [r2]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1F
	bgt _08015CBA
	b _08015E2E
_08015CBA:
	ldr r0, _08015CD0 @ =0xFFFFF01F
	and r0, r1
	strh r0, [r2]
	ldr r0, _08015CDC @ =0x0000080A
	add r3, r7, r0
	b _08015E00
	.align 2, 0
_08015CC8: .4byte 0x020185C0
_08015CCC: .4byte 0x0000080C
_08015CD0: .4byte 0xFFFFF01F
_08015CD4: .4byte 0x03000040
_08015CD8: .4byte 0x0201CFB0
_08015CDC: .4byte 0x0000080A
_08015CE0:
	mov r0, #0
	ldr r1, _08015D68 @ =0x02018DCC
	mov r9, r1
_08015CE6:
	mov r5, #0
	lsl r4, r0, #5
	lsl r1, r0, #1
	add r0, #1
	mov r8, r0
	mov r6, #0x44
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0xB
_08015CF8:
	add r1, r4, #2
	mov r2, r9
	ldrh r2, [r2]
	lsl r0, r2, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x27
	ble _08015D16
	mov r0, #0x38
	sub r0, r0, r2
	mul r1, r0
	add r0, r1, #0
	cmp r1, #0
	bge _08015D14
	add r0, #0xF
_08015D14:
	asr r1, r0, #4
_08015D16:
	lsl r0, r1, #0x10
	orr r0, r6
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r7
	mov r1, #0x80
	bl sub_080763D0
	add r6, #0x20
	add r5, #1
	cmp r5, #3
	ble _08015CF8
	mov r0, r8
	cmp r0, #4
	ble _08015CE6
	ldr r0, _08015D6C @ =0x020185C0
	ldr r3, _08015D70 @ =0x0000080C
	add r1, r0, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x19
	add r7, r0, #0
	cmp r4, #0x27
	ble _08015D7C
	ldr r1, _08015D74 @ =0x04000050
	mov r2, #0xF4
	lsl r2, r2, #4
	add r0, r2, #0
	strh r0, [r1]
	ldr r2, _08015D78 @ =0x04000052
	mov r1, #0x38
	sub r1, r1, r4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r4, #0
	sub r0, #0x28
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	strh r1, [r2]
	b _08015D86
_08015D68: .4byte 0x02018DCC
_08015D6C: .4byte 0x020185C0
_08015D70: .4byte 0x0000080C
_08015D74: .4byte 0x04000050
_08015D78: .4byte 0x04000052
_08015D7C:
	ldr r0, _08015DDC @ =0x04000050
	mov r1, #0
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
_08015D86:
	ldr r3, _08015DE0 @ =0x0000080C
	add r4, r7, r3
	ldrh r3, [r4]
	lsl r0, r3, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x37
	bgt _08015DF0
	ldr r1, _08015DE4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08015DAC
	ldr r1, _08015DE8 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08015DC0
_08015DAC:
	cmp r2, #0x2F
	bgt _08015DC0
	add r0, r2, #7
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _08015DEC @ =0xFFFFF01F
	and r1, r3
	orr r1, r0
	strh r1, [r4]
_08015DC0:
	ldr r0, _08015DE0 @ =0x0000080C
	add r3, r7, r0
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08015DEC @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _08015E2E
_08015DDC: .4byte 0x04000050
_08015DE0: .4byte 0x0000080C
_08015DE4: .4byte 0x03000040
_08015DE8: .4byte 0x0201CFB0
_08015DEC: .4byte 0xFFFFF01F
_08015DF0:
	ldr r1, _08015DF8 @ =0x0000080A
	add r3, r7, r1
	b _08015E00
	.align 2, 0
_08015DF8: .4byte 0x0000080A
_08015DFC:
	ldr r2, _08015E18 @ =0x0000080A
	add r3, r7, r2
_08015E00:
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
	b _08015E2E
_08015E18: .4byte 0x0000080A
_08015E1C:
	bl sub_08060578
	ldr r3, _08015E3C @ =0x0000080D
	add r1, r4, r3
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08015E2E:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08015E3C: .4byte 0x0000080D
	thumb_func_end sub_08015A2C

